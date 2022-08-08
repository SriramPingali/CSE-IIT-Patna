import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.LongWritable;
import java.io.IOException;


public class KMeansMap extends Mapper<LongWritable,Text,DoubleWritable,IntWritable>{
public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
    double ar[]={10,20,30,40,50};
        double min_dist=Integer.MAX_VALUE;
        int index=-1;
        int P=0;
        double dist;
        String line = value.toString();
        for(int i=0;i<ar.length;i++)
        {
                dist=Math.abs(Integer.parseInt(line)-ar[i]);
                if (dist<=min_dist)
                {
                        min_dist=dist;
                        index =i;
                        P=Math.abs(Integer.parseInt(line));
                }
                }
        context.write(new DoubleWritable(ar[index]),new IntWritable(P));
        }
        }

