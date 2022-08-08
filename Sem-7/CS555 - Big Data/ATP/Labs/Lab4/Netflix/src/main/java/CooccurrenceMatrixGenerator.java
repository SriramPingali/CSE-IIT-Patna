import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;

public class CooccurrenceMatrixGenerator {

    public static class CooccurrenceMatrixMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            // read the aggregated rating data
            // input: <offset, userId \t movieId1:rating1,movieId2:rating2,...>
            // output: <movieIdi:movieIdj, 1>

            // split the line
            String[] user_movieRatings = value.toString().split("\t");
            if (user_movieRatings.length < 2) {
                return;
            }
            String[] movieRatings = user_movieRatings[1].split(",");

            // emit each movieId pair
            for (String item1 : movieRatings) {
                String movie1 = item1.split(":")[0];

                for (String item2 : movieRatings) {
                    String movie2 = item2.split(":")[0];

                    String outputKey = movie1 + ":" + movie2;
                    context.write(new Text(outputKey), new IntWritable(1));
                }
            }
        }
    }

    public static class CooccurrenceMatrixReducer extends Reducer<Text, IntWritable, Text, IntWritable> {
        @Override
        public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
            // generate the co-occurrence of each movieId pair
            //  input: <movieIdi:movieIdj, (1, 1, ...)>
            // output: <movieIdi:movieIdj, 1+1+...>

            int sum = 0;
            for (IntWritable value : values) {
                sum += value.get();
            }
            context.write(key, new IntWritable(sum));
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Cooccurrence Matrix Generator");

        job.setJarByClass(CooccurrenceMatrixGenerator.class);
        job.setMapperClass(CooccurrenceMatrixMapper.class);
        job.setReducerClass(CooccurrenceMatrixReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        job.waitForCompletion(true);
    }

}
