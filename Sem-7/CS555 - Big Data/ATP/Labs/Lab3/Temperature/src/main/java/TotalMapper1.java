

import java.io.IOException;

import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Mapper;

/**
 * Mapper first data format
 */
class TotalMapper1 extends Mapper<
                    LongWritable, // Input key type
                    Text,         // Input value type
                    Text,         // Output key type
                    FloatWritable> {// Output value type
	
	

    protected void map(
            LongWritable key,   // Input key type
            Text value,         // Input value type
            Context context) throws IOException, InterruptedException {

    	

    		String record=value.toString();
    		
            // Split each record by using the field separator
    		// fields[0]= sensor id
    		// fields[1]= date
    		// fields[2]= hour:minute
    		// fields[3]= temperature
			String[] fields = record.split(",");
			
			String date=fields[1];
			float temperature=Float.parseFloat(fields[3]);
			
            context.write(new Text(date), new FloatWritable(temperature));
    }
    

    
}
