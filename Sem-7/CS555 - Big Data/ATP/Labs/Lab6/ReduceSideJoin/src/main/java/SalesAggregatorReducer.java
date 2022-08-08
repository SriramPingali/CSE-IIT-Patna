import java.io.IOException;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;


public class SalesAggregatorReducer extends Reducer<Text, DoubleWritable, Text, DoubleWritable> {
	public void reduce(Text key, Iterable<DoubleWritable> values, Context context)
			throws IOException, InterruptedException {
		double totalSalesPrice = 0.0;
		for (DoubleWritable val : values) {
			totalSalesPrice = totalSalesPrice + Double.parseDouble(val.toString());
		}
		DoubleWritable totalSales = new DoubleWritable(totalSalesPrice);

		// Validating the data before writing
		if (!(key.toString().isEmpty()) && !(totalSales == null))
			context.write(new Text(key), totalSales);
	}
}
