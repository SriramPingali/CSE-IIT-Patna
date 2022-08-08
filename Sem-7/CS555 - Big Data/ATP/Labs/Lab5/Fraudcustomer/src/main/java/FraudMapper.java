/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.IOException;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;


public class FraudMapper extends Mapper<LongWritable, Text, Text, FraudWritable> {
    public void map(LongWritable key, Text value, Context con)throws IOException, InterruptedException
    {
        String[] fields = value.toString().split(",");
        FraudWritable details = new FraudWritable();
        details.set(fields[1],fields[5],fields[6],fields[7]);
        con.write(new Text(fields[0]),details);
    }
}
