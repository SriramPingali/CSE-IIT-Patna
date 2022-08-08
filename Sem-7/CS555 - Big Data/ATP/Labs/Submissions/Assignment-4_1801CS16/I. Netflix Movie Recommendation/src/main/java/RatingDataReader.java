import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;

public class RatingDataReader {

    public static class RatingDataMapper extends Mapper<LongWritable, Text, Text, Text> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            // read the raw rating data which is in the format of "userId, movieId, rating"
            // input: <offset, userId,movieId,rating>
            // output: <userId, movieId:rating>

            // split the line
            String[] userId_movieId_rating = value.toString().trim().split(",");
            if (userId_movieId_rating.length < 3) {
                return;
            }

            // emit data using userId as the key
            String outputKey = userId_movieId_rating[0];
            String outputValue = userId_movieId_rating[1] + ":" + userId_movieId_rating[2];
            context.write(new Text(outputKey), new Text(outputValue));
        }
    }

    public static class RatingDataReducer extends Reducer<Text, Text, Text, Text> {
        @Override
        public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
            // aggregate the rating data by userId
            // input: <userId, (movieId1:rating1, movieId2:rating2, ...)>
            // output: <userId, movieId1:rating1,movieID2:rating2,...>

            // append all the values to a string
            StringBuilder outputValue = new StringBuilder();
            for (Text value : values) {
                String val = value.toString();
                outputValue.append(val).append(',');
            }
            outputValue.deleteCharAt(outputValue.length() - 1);

            context.write(key, new Text(outputValue.toString()));
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Rating Data Reader");

        job.setJarByClass(RatingDataReader.class);
        job.setMapperClass(RatingDataMapper.class);
        job.setReducerClass(RatingDataReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        job.waitForCompletion(true);
    }
}
