import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.log4j.Logger;

/**
 *This class contains the MR job for calculating the intial page rank
 *The first MR job calculates the total number of documents in the file
 *The second MR job emits page rank along with the outlinks for that node 
 */
public class LinkGraph extends Configured implements Tool {

	private static final Logger LOG = Logger.getLogger(LinkGraph.class);

	/**
	 * This function is used to launch the map reduce job
	 * This launches 2 MR jobs
	 * @param args
	 * @return
	 * @throws Exception
	 */
	public int run(String[] args) throws Exception {

		Job job = Job.getInstance(getConf(), "initial PageRank");
		FileSystem fs=FileSystem.get(getConf());
		if(fs.exists(new Path(args[1]))){
			fs.delete(new Path(args[1]),true);
		}
		job.setJarByClass(this.getClass());
		// Use TextInputFormat, the default unless job.setInputFormatClass is used
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		job.setMapperClass(Map.class);
		job.setReducerClass(Reduce.class);
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		job.setOutputFormatClass(SequenceFileOutputFormat.class);
		job.setInputFormatClass(TextInputFormat.class);

		job.setNumReduceTasks(1);

		int val= job.waitForCompletion(true) ? 0 : 1;

		long count=job.getCounters().findCounter("TotalDocuments","TotalDocuments").getValue();

		Configuration config = new Configuration();
		//LOG.info("userInput"+USER_INPUT);
		config.set("count", String.valueOf(count));


		Job pageRankJob = Job.getInstance(config, "initial PageRank");

		FileSystem fs1=FileSystem.get(getConf());
		if(fs1.exists(new Path(args[1]))){
			fs1.delete(new Path(args[1]),true);
		}
		pageRankJob.setJarByClass(this.getClass());
		// Use TextInputFormat, the default unless job.setInputFormatClass is used
		FileInputFormat.addInputPath(pageRankJob, new Path(args[0]));
		FileOutputFormat.setOutputPath(pageRankJob, new Path(args[1]));
		pageRankJob.setMapperClass(PageRankMap.class);
		pageRankJob.setReducerClass(PageRankReduce.class);
		pageRankJob.setOutputKeyClass(Text.class);
		pageRankJob.setOutputValueClass(Text.class);
		pageRankJob.setOutputFormatClass(SequenceFileOutputFormat.class);
		pageRankJob.setInputFormatClass(TextInputFormat.class);

		pageRankJob.setNumReduceTasks(6);
		int value= pageRankJob.waitForCompletion(true) ? 0 : 1;


		return value;
	}
	/**
	 * This is the first map job to count the number of lines in the files
	 */
	public static class Map extends Mapper<LongWritable, Text,  Text, Text>{

		public void map(LongWritable offset, Text lineText, Context context)
				throws IOException, InterruptedException {
			context.write(new Text("1"), new Text("1"));
		}

	}
	/**
	 * This is the first reduce job to sum the inputs from map and set a value in 
	 * the counter with the number of lines in the files
	 */
	public static class Reduce extends Reducer<Text, Text, Text, Text>{
		@Override
		public void reduce(Text word, Iterable<Text> lines, Context context)
				throws IOException, InterruptedException {
			long count=0;
			
			for(Text line:lines){
				count++;
			}
			context.getCounter("TotalDocuments","TotalDocuments").setValue(count);
		}
	}


	/**
	 *This map job is used to extract the outlinks for each node from the file and then compute 
	 *the initial page rank
	 */
	public static class PageRankMap extends Mapper<LongWritable, Text,  Text, Text>{

		final static String title="(<title>{1})(.*?)(<\\/title>{1})";
		final static String textTag="(<text(.*?)>{1})(.*?)(<\\/text>){1}";
		final static String urlOutlinks="\\[\\[(.*?)\\]\\]";
		final static String unicodeSeparator="₩₩₩₩";
		public void map(LongWritable offset, Text lineText, Context context)
				throws IOException, InterruptedException {

			String line=lineText.toString();	
			Pattern titlePattern=Pattern.compile(title);
			Pattern textTagPattern=Pattern.compile(textTag);
			Pattern urlOutlinksPattern=Pattern.compile(urlOutlinks);
			Matcher titleMatcher=titlePattern.matcher(line);
			Matcher textTagMatcher=textTagPattern.matcher(line);
			String key="";
			//extract title tag contents
			while(titleMatcher.find()){
				key=titleMatcher.group(2);
			}
			LOG.info("key "+key);
			
			String textTag="";
			StringBuffer valueBuffer=new StringBuffer();
			//extracting outlinks and then appends
			while(textTagMatcher.find()){
				textTag=textTagMatcher.group(3);
				Matcher outLinkMatcher=urlOutlinksPattern.matcher(textTag);
				while(outLinkMatcher.find()){	
					valueBuffer.append(outLinkMatcher.group(1));
					valueBuffer.append(unicodeSeparator);
				}
			}
			if(key.length()>0){
				LOG.info("Writng mapper  "+key+"  "+valueBuffer.toString());
				context.write(new Text(key), new Text(valueBuffer.toString()));
			}
		}
	}

	/**
	 *This is used to compute the initial pagerank and write it to the output
	 *file
	 */
	public static class PageRankReduce extends Reducer<Text, Text, Text, Text>{
		@Override
		public void reduce(Text word, Iterable<Text> lines, Context context)
				throws IOException, InterruptedException {	

			String value="";

			Configuration conf = context.getConfiguration();
			long totalCount = Long.valueOf(conf.get("count"));
			//computing initial page rank and the writing it to a file
			for(Text line:lines){
				double initialPageRank=1/(double)totalCount;
				value=line.toString()+String.valueOf(initialPageRank);
				context.write(word, new Text(value));
			}		
		}
	}
}
