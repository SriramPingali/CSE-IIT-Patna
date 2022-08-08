import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;

public class UserRatingAveraging {

    public static class RatingAveragingMapper extends Mapper<LongWritable, Text, Text, DoubleWritable> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            // read the raw rating data which is in the format of "userId,movieId,rating"
            // input: <offset, userId,movieId,rating>
            // output: <userId, rating>

            // split the line
            String[] user_movie_rating = value.toString().trim().split(",");
            String userId = user_movie_rating[0];
            double rating = Double.parseDouble(user_movie_rating[2]);
            context.write(new Text(userId), new DoubleWritable(rating));
        }
    }

    public static class RatingAveragingReducer extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {
        @Override
        public void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {
            // calculate the average rating given by a user
            // input: <userId, (rating1, rating2, ...)>
            // output: <userId, (rating1 + rating2 + ...) / #rating>

            int count = 0;
            double sum = 0.0;
            for (DoubleWritable value : values) {
                sum += value.get();
                ++count;
            }
            context.write(key, new DoubleWritable(sum / count));
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "User Rating Averaging");

        job.setJarByClass(UserRatingAveraging.class);
        job.setMapperClass(RatingAveragingMapper.class);
        job.setReducerClass(RatingAveragingReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(DoubleWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        job.waitForCompletion(true);
    }
}
