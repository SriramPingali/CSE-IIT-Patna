

import java.io.IOException;

import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.Reducer;

/**
 * Reducer
 */
class TotalReducer extends Reducer<
                Text,           // Input key type
                FloatWritable,  // Input value type
                Text,           // Output key type
                FloatWritable> {  // Output value type
    
    @Override
    protected void reduce(
        Text key, // Input key type
        Iterable<FloatWritable> values, // Input value type
        Context context) throws IOException, InterruptedException {

 
    	float max=Float.MIN_VALUE;
        
        // Iterate over the set of values and find the maximum value
        for (FloatWritable value : values) {
        	if (value.get()>max)
        		max=value.get();
        }

        // Emit pair (date, maximum temperature)
        context.write(new Text(key), new FloatWritable(max));
    }
}
