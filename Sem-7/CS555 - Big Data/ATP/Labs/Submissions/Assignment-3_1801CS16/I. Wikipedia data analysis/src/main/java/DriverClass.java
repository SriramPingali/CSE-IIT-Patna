import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.util.ToolRunner;

/**
 * This is the main driver class it runs all three
 * 1. creating the Link Graph
 * 2. map reduce computation
 * 3. sorting 
 */
public class DriverClass{
	
	public static void main(String[] arg) throws Exception {
		
		//Creating the link graph & running the program
		int res = ToolRunner.run(new Configuration(), new LinkGraph(), arg);
		
		
		int noOfRuns=10;
		int resPageRank=0;
		
		//running the page rank computation for 10 times
		for(int i=1;i<=noOfRuns;i++){
			if(i!=1){
				arg[1]=arg[2];
				arg[2]=arg[2]+i;
			}
			resPageRank = ToolRunner.run(new Configuration(),new PageRank(), arg);
		}
	
		//Sorting the pages based on the page rank value
		int pageRank = ToolRunner.run(new Configuration(),new PageRankSorting(), arg);
		
		System.out.println("Final Output path Sorting ***** "+arg[3]);
	}
}
