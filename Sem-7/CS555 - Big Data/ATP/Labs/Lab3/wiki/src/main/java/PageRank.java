import java.io.IOException;
import java.util.logging.Logger;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.SequenceFileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

/**
 * @author pagerank
 * This class is used to compute the page rank
 */
public class PageRank extends Configured implements Tool{

	private static String inputPath="";
	private static String outputPath="";
	static final Logger LOG =Logger.getLogger(PageRank.class.getName());
	
	public static void main(String[] args) throws Exception {
		int noOfRuns=10;
		int res=0;
		inputPath=args[0];
		for(int i=1;i<=noOfRuns;i++){
			if(i!=1){
			inputPath=outputPath;
			}
			outputPath=args[0]+i;
			res = ToolRunner.run(new Configuration(),new PageRank(), args);
		}
		System.out.println("OUTPUT PAGE RANK IS IN "+outputPath);
		System.exit(res);
	}

	/**
	 * This function is used to launch the map reduce job
	 * @param args
	 * @return
	 * @throws Exception
	 */
	public int run(String[] args) throws Exception {
		
		Job pageRankJob = Job.getInstance(getConf(), "PageRank");
		pageRankJob.setJarByClass(this.getClass());
		// Use TextInputFormat, the default unless job.setInputFormatClass is used
		/*System.out.println("INPIUY PATH"+inputPath);
		System.out.println("OUTPUT PATH"+outputPath);*/
		FileInputFormat.addInputPath(pageRankJob, new Path(args[1]));
		FileOutputFormat.setOutputPath(pageRankJob, new Path(args[2]));
		pageRankJob.setMapperClass(Map.class);
		pageRankJob.setReducerClass(Reduce.class);
		pageRankJob.setOutputKeyClass(Text.class);
		pageRankJob.setOutputValueClass(Text.class);
		pageRankJob.setOutputFormatClass(SequenceFileOutputFormat.class);
		pageRankJob.setInputFormatClass(SequenceFileInputFormat.class);
		pageRankJob.setNumReduceTasks(6);
		FileSystem fs=FileSystem.get(getConf());
		if(fs.exists(new Path(args[2]))){
			fs.delete(new Path(args[2]),true);
		}
		
		int val= pageRankJob.waitForCompletion(true) ? 0 : 1;
		System.out.println(" completed "+val);
		if(fs.exists(new Path(args[1]))){
			fs.delete(new Path(args[1]),true);
		}
		return val;
	}

	/**
	 * This map job is used to compute the page rank 
	 */
	public static class Map extends Mapper<Text, Text,  Text, Text>{

		final static String unicodeSeparator="₩₩₩₩";
		final static Pattern unicodePattern = Pattern.compile("\u20A9\u20A9\u20A9\u20A9", Pattern.LITERAL);
		public void map(Text keyText, Text lineText, Context context)
				throws IOException, InterruptedException {
		
			String key=keyText.toString();	
			String[] outLinksArr=unicodePattern.split(lineText.toString());
			int outLinks=outLinksArr.length-1;
			double pageRank=Double.valueOf(outLinksArr[outLinks]);
			double rank=pageRank/(double)outLinks;
			String value="";
			boolean outLinkFlag=false;
			//emmiting the inlink record for summation to compute the pagerank
			for(int i=0;i<outLinksArr.length-1;i++){
			
				value=outLinkFlag+unicodeSeparator+rank;
				context.write(new Text(outLinksArr[i]), new Text(value));
			}
			
			outLinkFlag=true;
			//emiting the outlink record
			context.write(new Text(key), new Text(outLinkFlag+unicodeSeparator+lineText.toString()));
		}
	}

	/**
	 *This reduce job is used to compute the page rank by creating the summation of in-links
	 */
	public static class Reduce extends Reducer<Text, Text, Text, Text>{
		final static String unicodeSeparator="₩₩₩₩";
		final static Pattern unicodePattern = Pattern.compile("\u20A9\u20A9\u20A9\u20A9", Pattern.LITERAL);
		@Override
		public void reduce(Text word, Iterable<Text> lines, Context context)
				throws IOException, InterruptedException {	
			String key=word.toString();
			String[] array;
			double sum=0;
			StringBuffer valueBuffer=new StringBuffer();
			boolean validFlag=false;
			
			for(Text line:lines){
				array=unicodePattern.split(line.toString());
				//summation if its inlink records
				if(array[0].equals("false")){
					sum+=Float.valueOf(array[1]);
				}
				//new pagerank to be updated to the outlink record
				if(array[0].equals("true")){
					validFlag=true;
					for(int i=1;i<array.length-1;i++){
						valueBuffer.append(array[i]);
						valueBuffer.append(unicodeSeparator);
					}
				}
			}
			//page rank computation
			sum=0.15+(sum*0.85);

			valueBuffer.append(sum);
			//checking whether the record is present in the title
			if(validFlag){
				context.write(new Text(key), new Text(valueBuffer.toString()));
			}
		}
	}
}
