import java.io.IOException;
import java.util.ArrayList;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;


public class PageRank {
	public static class Map extends Mapper<Object, Text, IntWritable, Text> {

		@Override
		public void map(Object key, Text value, Context context) throws IOException, InterruptedException {

			// TODO: your implementation here, use the pseudo code as instruction
			
			PRNode node = new PRNode("node," + value.toString());
			double p = node.getPagerank() / node.adjacencyListSize();
			context.write(new IntWritable(node.getNodeId()), new Text(node.toString()));
			ArrayList<Integer> adjList = node.getAdjacencyList();
			for (int i = 0; i < adjList.size(); i++) {
				PRNode temp_node = new PRNode("not," + p);
				context.write(new IntWritable(adjList.get(i)), new Text(temp_node.toString()));
			}
		}
	}
	public static class Reduce extends Reducer<IntWritable, Text, IntWritable, DoubleWritable> {

		@Override
		public void reduce(IntWritable key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
			
			// TODO: your implementation here, use the pseudo code as instruction
			
			double pagerank = 0.0;
			PRNode M = null;
			for (Text value : values) {
				PRNode node = new PRNode(value.toString());
				if (node.isNode()) { //graph structure
					M = node;
				} else {
					pagerank += node.getPagerank();
				}
			}
			M.setPagerank(pagerank);
			context.write(new IntWritable(M.getNodeId()), new DoubleWritable(M.getPagerank()));
		}
	}

	public static void main(String[] args) throws Exception {
		if (args.length != 2) {
			System.out.println("Usage: [input] [output]");
			System.exit(-1);
		}
		
		Job job = Job.getInstance(new Configuration());
		job.setJarByClass(PageRank.class);
		job.setJobName("Page Rank");
		
		job.setMapperClass(Map.class);
		job.setReducerClass(Reduce.class);
		
		job.setInputFormatClass(TextInputFormat.class);
		job.setOutputFormatClass(TextOutputFormat.class);
		
		FileInputFormat.setInputPaths(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(Text.class);
		
		job.setNumReduceTasks(1);

		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
