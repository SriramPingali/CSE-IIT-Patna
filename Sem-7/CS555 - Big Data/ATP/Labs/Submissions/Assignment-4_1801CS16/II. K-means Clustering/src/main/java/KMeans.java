import java.util.*;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;


public class KMeans extends Configured implements Tool {
public int run(String[] args) throws Exception {


                if(args.length!=2)
                {
                System.err.println("<input><output>");
                System.exit(1);
                }

                FileSystem fs = FileSystem.get(new Configuration());
                @SuppressWarnings("deprecation")
                Job job=new Job(getConf(),"KMeans");
                FileOutputFormat.setOutputPath(job,new Path(args[1]));
                job.setJarByClass(KMeans.class);
                job.setMapperClass(KMeansMap.class);
                job.setReducerClass(KMeansReduce.class);
                FileInputFormat.addInputPath(job,new Path(args[0]));
                fs.delete(new Path(FileOutputFormat.getOutputPath(job).toString()), true);
                job.setInputFormatClass(TextInputFormat.class);
                job.setMapOutputKeyClass(DoubleWritable.class);
                job.setMapOutputValueClass(IntWritable.class);
                job.setOutputKeyClass(DoubleWritable.class);
                job.setOutputValueClass(Text.class);
                return job.waitForCompletion(true) ? 0:1;

}



public static void main(String[] args) throws Exception {
Configuration conf=new Configuration();
System.exit(ToolRunner.run(conf, new KMeans(),args));
}
}

