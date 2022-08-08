import java.io.IOException;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Logger;
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
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

/**
 * This class is used to form the inverted index for the document
 */
public class InvertedIndex extends Configured implements Tool{
	static final Logger LOG =Logger.getLogger(PageRank.class.getName());
	public static void  main(String[] args) throws Exception{
		int res = ToolRunner.run(new Configuration(),new InvertedIndex(), args);
	}

	/* 
	 * This method is used to run the inverted index Job
	 */
	@Override
	public int run(String[] args) throws Exception {
		// TODO Auto-generated method stub
		FileSystem fs=FileSystem.get(getConf());
		if(fs.exists(new Path(args[1]))){
			fs.delete(new Path(args[1]),true);
		}
		Job invertedIndexJob = Job.getInstance(getConf(), "invertedIndexJob");
		invertedIndexJob.setJarByClass(this.getClass());
		
		FileInputFormat.addInputPath(invertedIndexJob, new Path(args[0]));
		FileOutputFormat.setOutputPath(invertedIndexJob, new Path(args[1]));
		invertedIndexJob.setMapperClass(Map.class);
		invertedIndexJob.setReducerClass(Reduce.class);
		invertedIndexJob.setOutputKeyClass(Text.class);
		invertedIndexJob.setOutputValueClass(Text.class);
		invertedIndexJob.setNumReduceTasks(6);
		int val= invertedIndexJob.waitForCompletion(true) ? 0 : 1;
		return 0;
	}
	
/**
 * @author Map
 * This is used to extract all the words from the text tag
 */
public static class Map extends Mapper<LongWritable, Text,  Text, Text>{

		final static String textTag="(<text(.*?)>{1})(.*?)(<\\/text>){1}";
		final static String idTag="(<title>{1})(.*?)(<\\/title>{1})[ ]*(<id>{1})(.*?)(<\\/id>{1})";
		private static final Pattern WORD_BOUNDARY = Pattern.compile("\\W+|_|\\d+");
		public void map(LongWritable offset, Text lineText, Context context)
				throws IOException, InterruptedException {

			String text=lineText.toString();
			LOG.info("ITs the value "+text);
			Pattern textPattern = Pattern.compile(textTag);
			Pattern idPattern = Pattern.compile(idTag);
			Matcher matcherText=textPattern.matcher(lineText.toString());
			Matcher matcherID=idPattern.matcher(text);
			String textTag="";
			
			String id="";
			while(matcherID.find()){
				id=matcherID.group(5);
			}

			while(matcherText.find()){
				textTag=matcherText.group(3);
				for(String word:WORD_BOUNDARY.split(textTag)){
					//word=word.replaceAll("\\d+","");
					if(word.isEmpty() || word.matches("\\d+")){
						continue;
					}
					String query="\\[\\[.*\\]\\]";
					
					if(word.matches(query)){
						word=word.replaceFirst("\\[\\[", "");
						word=word.replaceAll("\\]\\]$", "");
					} 
					//extracting the words in the text tag and writing it to a file
					context.write(new Text(word), new Text(id));

				}
			}
		}
}
	
/**
 *This is used to form the inverted index for the document
 */
public static class Reduce extends Reducer<Text, Text, Text, Text>{
		@Override
		public void reduce(Text word, Iterable<Text> textList, Context context)
				throws IOException, InterruptedException {	
			Set<Long> ar=new TreeSet<Long>();
			for(Text value:textList){
				if(!value.toString().isEmpty()){
					long longValue=Long.valueOf(value.toString());
					ar.add(longValue);
				}
			}
			//writing the output in the file merging all the document id 
			StringBuffer valueBuffer=new StringBuffer();
			for(Long val:ar){
				valueBuffer.append(val);
				valueBuffer.append(",");
			}
			if(valueBuffer.length()>0){
				valueBuffer.deleteCharAt(valueBuffer.length()-1);
				context.write(word, new Text(valueBuffer.toString()));
			}
		}
	}
}

