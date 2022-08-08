

import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class CustomerMapper extends Mapper<Object, Text, IntWritable, Text> {

	private static final String fileTag = "customer~";

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
		String inputData = value.toString();
		String customerData[] = inputData.split(",");
		if (customerData.length == 6) {
			IntWritable customerId = new IntWritable(0);
			customerId = (customerData[0].isEmpty() ? customerId : new IntWritable(Integer.parseInt(customerData[0])));
			Text state = new Text();
			state = (customerData[4].isEmpty() ? null : new Text(customerData[4]));

			// Tagging the Mapper output to show that it is coming from Customer file
			Text stateName = new Text(fileTag + state);

			// Validating the data before writing
			if (!(customerId.toString().isEmpty()) && !(stateName.toString().isEmpty())) {
				context.write(customerId, stateName);
			}
		} 
	}
}
