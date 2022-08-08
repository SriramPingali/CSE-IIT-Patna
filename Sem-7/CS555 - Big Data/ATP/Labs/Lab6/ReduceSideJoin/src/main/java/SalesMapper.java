

import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class SalesMapper extends Mapper<Object, Text, IntWritable, Text> {

	private static final String fileTag = "sales~";

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {

		String inputData = value.toString();
		String salesData[] = inputData.split(",");
		if (salesData.length == 3) {
			IntWritable customerId = new IntWritable(1);
			customerId = (salesData[1].isEmpty() ? customerId : new IntWritable(Integer.parseInt(salesData[1])));

			Text sales = new Text();
			sales = (salesData[2].isEmpty() ? null : new Text(salesData[2]));

			// Tagging the Mapper output to show that it is coming from Sales file
			Text salesPrice = new Text(fileTag + sales);

			// Validating the data before writing
			if (!(customerId.toString().isEmpty()) && !(salesPrice.toString().isEmpty())) {
				context.write(customerId, salesPrice);
			}
		} 
	}
}
