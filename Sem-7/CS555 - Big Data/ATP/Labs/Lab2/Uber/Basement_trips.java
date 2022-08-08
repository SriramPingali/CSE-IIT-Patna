import java.io.IOException;
import java.text.ParseException;
import java.util.Date;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Basement_trips {
	public static class TokenizerMapper extends Mapper<Object, Text, Text, IntWritable>
	{
		java.text.SimpleDateFormat format = new java.text.SimpleDateFormat("MM/dd/yyyy");
		String[] days ={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
		private Text basement = new Text();
		Date date = null;
		private int trips;
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException 
		{
			String line = value.toString();
			String[] splits = line.split(",");
			basement.set(splits[0]);
			try 
			{
				date = format.parse(splits[1]);
			} 
			catch (ParseException e) 
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			trips = new Integer(splits[3]);
			String keys = basement.toString()+ " "+days[date.getDay()];
			context.write(new Text(keys), new IntWritable(trips));
		}
	}
	
	public static class IntSumReducer extends Reducer<Text,IntWritable,Text,IntWritable> 
	{
		private IntWritable result = new IntWritable();
		public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException 
		{
			int sum = 0;
			for (IntWritable val : values) 
			{
				sum += val.get();
			}
			result.set(sum);
			context.write(key, result);
		}
	} 

	public static void main(String[] args) throws Exception 
	{
		Configuration conf = new Configuration();
		Job job = Job.getInstance(conf, "Basement Trips");
		job.setJarByClass(Basement_trips.class);
		job.setMapperClass(TokenizerMapper.class);
		job.setCombinerClass(IntSumReducer.class);
		job.setReducerClass(IntSumReducer.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(IntWritable.class);
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
