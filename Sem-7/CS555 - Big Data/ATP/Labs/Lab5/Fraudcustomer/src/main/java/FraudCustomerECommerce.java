/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class FraudCustomerECommerce {
    public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
        // creating object of configuration class 
	Configuration conf = new Configuration();
	String[] In_Out_files = new GenericOptionsParser(conf, args).getRemainingArgs();
	
	Path input_path = new Path(In_Out_files[0]);
	Path output_directory = new Path(In_Out_files[1]);
				
	Job job_obj = new Job(conf, "FraudCustomer");
	// setting name of main class
	job_obj.setJarByClass(FraudCustomerECommerce.class);
	//name of mapper class
	job_obj.setMapperClass(FraudMapper.class);
		// name of reducer class
	job_obj.setReducerClass(FraudReducer.class);
		// name of combiner class
        job_obj.setMapOutputKeyClass(Text.class);
        job_obj.setMapOutputValueClass(FraudWritable.class);
        
	job_obj.setOutputKeyClass(Text.class);
	job_obj.setOutputValueClass(Text.class);
		
	FileInputFormat.addInputPath(job_obj, input_path);
		
	FileOutputFormat.setOutputPath(job_obj, output_directory);
		
	output_directory.getFileSystem(job_obj.getConfiguration()).delete(output_directory,true);
	System.exit(job_obj.waitForCompletion(true) ? 0 : 1);

    }
    
}
