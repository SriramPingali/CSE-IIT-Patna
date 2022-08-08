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
import java.util.HashMap;
import java.util.Map;

public class CooccurrenceMatrixNormalization {

    public static class CooccurrenceNormalizationMapper extends Mapper<LongWritable, Text, Text, Text> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            // read the unnormalized co-occurrence matrix
            // input: <offset, movieIdi:movieIdj \t count>
            // output: <movieIdi, movieIdj:count>

            // split the line
            String[] moviePair_count = value.toString().trim().split("\t");
            if (moviePair_count.length < 2) {
                return;
            }
            String[] moviePair = moviePair_count[0].split(":");

            // emit the data by the first movie
            String outputKey = moviePair[0];
            String outputValue = moviePair[1] + ":" + moviePair_count[1];
            context.write(new Text(outputKey), new Text(outputValue));
        }
    }

    public static class CooccurrenceNormalizationReducer extends Reducer<Text, Text, Text, Text> {
        @Override
        public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
            // normalize the row of movieIdi
            // input: <movieIdi, (movieIdj1:count1, movieIdj2:count2, ...)>
            // output: <movieIdj, movieIdi=countj/totalCount>

            double totalCount = 0.0;
            Map<String, Integer> movie_count = new HashMap<String, Integer>();

            // fetch each movie_count pair and sum up the count
            for (Text value : values) {
                String[] val = value.toString().split(":");
                int count = Integer.parseInt(val[1]);
                totalCount += count;
                movie_count.put(val[0], count);
            }

            // normalize the count
            // prepare for matrix multiplication at next step by switching the positions of movieIds
            for (Map.Entry<String, Integer> item : movie_count.entrySet()) {
                String outputKey = item.getKey();
                Double normalizedCount = item.getValue() / totalCount;
                String outputValue = key.toString() + "=" + normalizedCount.toString();
                context.write(new Text(outputKey), new Text(outputValue));
            }
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Cooccurrence Matrix Normalization");

        job.setJarByClass(CooccurrenceMatrixNormalization.class);
        job.setMapperClass(CooccurrenceNormalizationMapper.class);
        job.setReducerClass(CooccurrenceNormalizationReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path((args[1])));

        job.waitForCompletion(true);
    }
}
