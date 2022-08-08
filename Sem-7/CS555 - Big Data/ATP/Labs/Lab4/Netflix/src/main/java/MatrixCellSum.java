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

public class MatrixCellSum {

    public static class CellSumMapper extends Mapper<LongWritable, Text, Text, DoubleWritable> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            // read the sub product of each cell
            // input: <offset, userId:movieId \t subRating>
            // output: <userId:movieId, subRating>

            String[] userMovie_subRating = value.toString().trim().split("\t");
            String outputKey = userMovie_subRating[0];
            double outputValue = Double.parseDouble(userMovie_subRating[1]);
            context.write(new Text(outputKey), new DoubleWritable(outputValue));
        }
    }

    public static class CellSumReducer extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {
        @Override
        public void reduce(Text key, Iterable<DoubleWritable> values, Context context) throws IOException, InterruptedException {
            // sum up the sub rating to the final rating
            // input: <userId:movieId, (subRating1, subRating2, ...)>
            // output: <userId \t movieId, subRating1 + subRating2 + ...>

            double sum = 0.0;
            for (DoubleWritable value : values) {
                sum += value.get();
            }
            String[] user_movie = key.toString().trim().split(":");
            String outputKey = user_movie[0] + "\t" + user_movie[1];
            context.write(new Text(outputKey), new DoubleWritable(sum));
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Matrix Cell Sum");

        job.setJarByClass(MatrixCellSum.class);
        job.setMapperClass(CellSumMapper.class);
        job.setReducerClass(CellSumReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(DoubleWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        job.waitForCompletion(true);
    }
}
