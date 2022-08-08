
import java.io.IOException;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.SequenceFileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
import org.apache.log4j.Logger;

public class PageRankSorting extends Configured implements Tool{
	private static final Logger LOG = Logger.getLogger(PageRankSorting.class);
	public static void main(String[] args) throws Exception {
		int res = ToolRunner.run(new Configuration(),new PageRankSorting(), args);
		System.exit(res);
	}

	/**
	 * This function is used to launch the map reduce job
	 * @param args
	 * @return
	 * @throws Exception
	 */
	public int run(String[] args) throws Exception {
		
		Job pageRankSortingJob = Job.getInstance(getConf(), "PageRank");
		pageRankSortingJob.setJarByClass(this.getClass());
		
		FileSystem fs=FileSystem.get(getConf());
		// Use TextInputFormat, the default unless job.setInputFormatClass is used
		FileInputFormat.addInputPath(pageRankSortingJob, new Path(args[2]));
		FileOutputFormat.setOutputPath(pageRankSortingJob, new Path(args[3]));
		if(fs.exists(new Path(args[3]))){
			fs.delete(new Path(args[3]),true);
		}
		pageRankSortingJob.setMapperClass(Map.class);
		pageRankSortingJob.setReducerClass(Reduce.class);
		pageRankSortingJob.setMapOutputKeyClass(DoubleWritable.class);
		pageRankSortingJob.setMapOutputValueClass(Text.class);
		pageRankSortingJob.setOutputKeyClass(Text.class);
		pageRankSortingJob.setOutputValueClass(DoubleWritable.class);
		pageRankSortingJob.setSortComparatorClass(DescendingKeyComparator.class);
		pageRankSortingJob.setOutputFormatClass(TextOutputFormat.class);
		pageRankSortingJob.setInputFormatClass(SequenceFileInputFormat.class);
		pageRankSortingJob.setNumReduceTasks(1);
		System.out.println("Page rank job");
		int val= pageRankSortingJob.waitForCompletion(true) ? 0 : 1;
		System.out.println(" completed "+val);
		
		if(fs.exists(new Path(args[2]))){
			fs.delete(new Path(args[2]),true);
		}
		return val;
	}

	public static class Map extends Mapper<Text, Text,  DoubleWritable, Text>{

		final static String unicodeSeparator="₩₩₩₩";
		final static Pattern unicodePattern = Pattern.compile("\u20A9\u20A9\u20A9\u20A9", Pattern.LITERAL);
		public void map(Text keyText, Text valueText, Context context)
				throws IOException, InterruptedException {
			
			String line=valueText.toString();
			/*String[] array=line.split("\t");*/
			String key=keyText.toString();
			String[] outLinksArr=unicodePattern.split(line);
			double val=Double.valueOf(outLinksArr[outLinksArr.length-1]);
			LOG.info("key"+val+"  value"+key);
			//sending the page rank values as key so that it gets sorted using Descending key Comparator
			context.write(new DoubleWritable(val), new Text(key));
		}
	}

	public static class Reduce extends Reducer<DoubleWritable, Text, Text, DoubleWritable>{
		@Override
		public void reduce(DoubleWritable value, Iterable<Text> lines, Context context)
				throws IOException, InterruptedException {	
			/*long sum=0;
			if(context.getCounter("pageCount","pageCount")!=null){
				sum=context.getCounter("pageCount","pageCount").getValue(); 
			}*/
			
			//LOG.info("The value of sum "+sum);
			
			//if(sum<=100){
				//writing the output to the file
				for(Text line:lines){
					//sum+=1;
					//context.getCounter("pageCount","pageCount").setValue(sum);
					LOG.info("Key "+line.toString());
					LOG.info("Value "+String.valueOf(value));
					context.write(line,value);
				}
			//}
		}
	}
	public static class DescendingKeyComparator extends WritableComparator {
	    protected DescendingKeyComparator() {
	        super(DoubleWritable.class, true);
	    }
	    @SuppressWarnings("rawtypes")
	    @Override
	    public int compare(WritableComparable w1, WritableComparable w2) {
	    	DoubleWritable key1 = (DoubleWritable) w1;
	    	DoubleWritable key2 = (DoubleWritable) w2;          
	        return -1 * key1.compareTo(key2);
	    }
	}
}
