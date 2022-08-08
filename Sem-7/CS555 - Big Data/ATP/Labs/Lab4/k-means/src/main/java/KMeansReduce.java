import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.output.MultipleOutputs;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;

import java.io.IOException;



public class KMeansReduce extends Reducer<DoubleWritable,IntWritable, DoubleWritable,Text> 
{
        @SuppressWarnings("rawtypes")
        private MultipleOutputs multipleOutputs;

         @SuppressWarnings({ "unchecked", "rawtypes" })
        @Override
         public void setup(Context context) throws IOException, InterruptedException {
                 multipleOutputs  = new MultipleOutputs(context);
         }

        @SuppressWarnings("unchecked")
        public void reduce(DoubleWritable key,Iterable<IntWritable> values,Context context) throws IOException, InterruptedException 
    {
      double Sum=0.0;
      double num=0.0;
      int P=0;
      double new_center=0.0; 
      String data="";
      while(values.iterator().hasNext())
      {
          P=values.iterator().next().get();
          Sum+=P;
          ++num;
          data=data+" "+String.valueOf(P);
      }
      new_center=Sum/num;
      multipleOutputs.write(new DoubleWritable(Math.round(new_center*10)/10),"","center");
      //multipleOutputs.write(new DoubleWritable(Math.abs(key.get()-Math.round(new_center*10)/10)),"","convergene");
      multipleOutputs.write(new DoubleWritable(Math.round(new_center*10)/10), new Text(data),"clustereddata");
    }

        @Override
         protected void cleanup(Context context)
           throws IOException, InterruptedException {
          multipleOutputs.close();
         }
}


