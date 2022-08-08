import java.io.IOException;
import java.io.PrintWriter;

import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class EarthquakeMapper extends
		Mapper<LongWritable, Text, Text, DoubleWritable> {

	private PrintWriter out=null;

	@Override
	public void map(LongWritable key, Text value, Context context)
			throws IOException, InterruptedException {
		out = new PrintWriter("./logoutput.txt");
		String[] line = value.toString().split(",",12);
		for(String lines:line){
		System.out.println("here is is first"+lines);
//		out.println("here is is first"+lines);
		out.println("here is is first"+lines);
		}

         //writer.write("Hello World");
		 // Ignore invalid lines
        if (line.length != 12) {
            System.out.println("Ignore invalid lines" + line.length);
            out.println("Ignore invalid lines" + line.length);

            return;
        }

        // The output `key` is the name of the region
        String outputKey = line[11];
        System.out.println("he output `key` is the name of the region"+outputKey);
        out.println("he output `key` is the name of the region"+outputKey);


        // The output `value` is the magnitude of the earthquake
        double outputValue = Double.parseDouble(line[8]);
        System.out.println("he output `value` is the name of the region"+outputValue);
        out.println("he output `value` is the name of the region"+outputValue);
      
        
        // Record the output in the Context object
        context.write(new Text(outputKey), new DoubleWritable(outputValue));
        
    }
}
