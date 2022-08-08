/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.util.*;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class FraudReducer extends Reducer<Text, FraudWritable, Text, Text> 
	{
    ArrayList<String> customers = new ArrayList<String>();
	public void reduce(Text word, Iterable<FraudWritable> value_list, Context con)throws IOException, InterruptedException
	{
            //Iterator<FraudWritable> itr= value_list.iterator();
            int orderCount = 0;
            int fraudPoint=0;
            int returnCount = 0;
            String name = null;
            for(FraudWritable x:value_list)
            {
                orderCount++;
                name = x.getCustomerName();
                if(x.getreturn())
                {
                    returnCount++;
                    SimpleDateFormat sdf = new SimpleDateFormat("dd-MM-yyyy");
                    try {
                        Date recieveDate= sdf.parse(x.getrecieveDate());
                        Date returnDate= sdf.parse(x.getreturnDate());
                        
                        long diffInMillies = Math.abs(returnDate.getTime() - recieveDate.getTime());
                        long diffinDays = TimeUnit.DAYS.convert(diffInMillies, TimeUnit.MILLISECONDS);
                        
                        if(diffinDays>10)
                            fraudPoint++;
                    } catch (ParseException ex) {
                        Logger.getLogger(FraudReducer.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
            }
            double returnRatio = (returnCount/(orderCount*1.0))*100;
            if(returnRatio>=50.0)
            {
                fraudPoint+=10;
            }
            //con.write(word,new Text(orderCount+","+returnCount+","+returnRatio));
            customers.add(word.toString()+","+name+","+fraudPoint);
        }
        @Override
        protected void cleanup(Context c) throws IOException, java.lang.InterruptedException
        {
	/* sort customers based on fraudpoints */
            Collections.sort(customers, new Comparator<String>()
            {
		public int compare(String s1, String s2)
		{
		    int fp1 = Integer.parseInt(s1.split(",")[2]);
		    int fp2 = Integer.parseInt(s2.split(",")[2]);
		    
		    return -(fp1-fp2);     /*For descending order*/
		}});
        for(String x:customers)
            {
                String[] fields = x.split(",");
                c.write(new Text(fields[0]),new Text(fields[1]+"\t"+fields[2]));
            }
        }
        
}
