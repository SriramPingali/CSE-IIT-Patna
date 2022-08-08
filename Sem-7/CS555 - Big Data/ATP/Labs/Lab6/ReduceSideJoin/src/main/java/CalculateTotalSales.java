import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;


public class CalculateTotalSales {

	public static void main(String[] args) throws Exception {

		Configuration config = new Configuration();

		// Configuration settings for joining customers and sales files
		Job mr1 = new Job(config, "Sales aggregation by Customer ID");
		mr1.setJarByClass(CalculateTotalSales.class);
		mr1.setReducerClass(CustomerSalesJoinReducer.class);
		mr1.setOutputKeyClass(IntWritable.class);
		mr1.setOutputValueClass(Text.class);

		String customersFilePath = args[0];
		String salesFilePath = args[1];

		MultipleInputs.addInputPath(mr1, new Path(customersFilePath), TextInputFormat.class, CustomerMapper.class);
		MultipleInputs.addInputPath(mr1, new Path(salesFilePath), TextInputFormat.class, SalesMapper.class);

		// Specifying temporary directory name for storing mapreduce 1's output
		final String TMP_DIR = "temp_dir";

		Path tempFilePath = new Path(TMP_DIR);
		FileOutputFormat.setOutputPath(mr1, tempFilePath);

		int flag = 1;

		if (mr1.waitForCompletion(true)) {

			// Configuration settings for aggregating sales by state
			Job mr2 = new Job(config, "Sales aggregation by State");
			mr2.setJarByClass(CalculateTotalSales.class);
			mr2.setMapperClass(StateSalesMapper.class);
			mr2.setReducerClass(SalesAggregatorReducer.class);
			mr2.setOutputKeyClass(Text.class);
			mr2.setOutputValueClass(DoubleWritable.class);

			TextInputFormat.addInputPath(mr2, new Path(TMP_DIR));

			String finalFilePath = args[2];
			Path finalOutputPath = new Path(finalFilePath);
			FileOutputFormat.setOutputPath(mr2, finalOutputPath);

			if (mr2.waitForCompletion(true)) {
				tempFilePath.getFileSystem(config).deleteOnExit(tempFilePath);
				flag = 0;
			}
			System.exit(flag);
		}
	}
}
