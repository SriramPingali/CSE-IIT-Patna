import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

public class MatrixCellMultiplication {

    public static class CooccurrenceCellMapper extends Mapper<LongWritable, Text, Text, Text> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            // read the normalized co-occurrence matrix
            // input: <offset, movieIdk \t movieIdi=weighti>
            // output: <movieIdk, movieIdi=weighti>

            String[] movie2_movie1Weight = value.toString().trim().split("\t");
            if (movie2_movie1Weight.length < 2) {
                return;
            }
            context.write(new Text(movie2_movie1Weight[0]), new Text(movie2_movie1Weight[1]));
        }
    }

    public static class UserRatingCellMapper extends Mapper<LongWritable, Text, Text, Text> {
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            // generate the rating matrix by reading the raw rating data
            // input: <offset, userId,movieId,rating>
            // output: <movieId, userId:rating>

            String[] user_movie_rating = value.toString().trim().split(",");
            String outputKey = user_movie_rating[1];
            String outputValue = user_movie_rating[0] + ":" + user_movie_rating[2];
            context.write(new Text(outputKey), new Text(outputValue));
        }
    }

    public static class CellReducer extends Reducer<Text, Text, Text, Text> {
        private Map<String, Double> user_averageRating_map = new HashMap<String, Double>();

        @Override
        public void setup(Context context) throws IOException, InterruptedException {
            // read the average rating for each user: HashMap<userId, averageRating>

            Configuration conf = context.getConfiguration();
            String userAverageRatingPath = conf.get("userAverageRating", "userAverageRating");

            // list files in the path
            FileSystem fs = FileSystem.get(conf);
            Path pathPattern = new Path(userAverageRatingPath + "/part-r-*");
            FileStatus[] statusList = fs.globStatus(pathPattern);

            for (FileStatus status : statusList) {
                BufferedReader br = new BufferedReader(new InputStreamReader(fs.open(status.getPath())));
                String line = br.readLine();
                while (line != null) {
                    String[] user_rating = line.trim().split("\t");
                    user_averageRating_map.put(user_rating[0], Double.parseDouble(user_rating[1]));
                    line = br.readLine();
                }
                br.close();
            }
        }

        @Override
        public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
            // multiply a cell of co-occurrence matrix with the corresponding cell of rating matrix
            // input: <movieIdk, (movieIdi1=weighti1, movieIdi2=weighti2, ..., userIdj1:ratingj1, userIdj2:ratingj2, ...)>
            // output: <userId:movieId, weight * rating>

            Map<String, Double> movie_weight_map = new HashMap<String, Double>();
            Map<String, Double> user_rating_map = new HashMap<String, Double>();
            for (Text value : values) {
                String val = value.toString();
                if (val.contains("=")) {
                    String[] movie_weight = val.split("=");
                    movie_weight_map.put(movie_weight[0], Double.parseDouble(movie_weight[1]));
                } else {
                    String[] user_rating = val.split(":");
                    user_rating_map.put(user_rating[0], Double.parseDouble(user_rating[1]));
                }
            }

            for (Map.Entry<String, Double> movieItem : movie_weight_map.entrySet()) {
                String movieId = movieItem.getKey();
                double weight = movieItem.getValue();
                for (Map.Entry<String, Double> userItem : user_averageRating_map.entrySet()) {
                    String userId = userItem.getKey();
                    double rating = 0.0;
                    if (user_rating_map.containsKey(userId)) {
                        rating = user_rating_map.get(userId);
                    } else {
                        rating = userItem.getValue();
                    }

                    String outputKey = userId + ":" + movieId;
                    Double outputValue = weight * rating;
                    context.write(new Text(outputKey), new Text(outputValue.toString()));
                }
            }
        }
    }

    public static void main(String[] args) throws Exception {
        if (args.length < 4) {
            System.err.println("Not enough arguments provided!");
            System.err.println("Specify 4 arguments: CooccurrenceMatrixPath UserRatingMatrixPath OutputPath UserAverageRatingPath");
            System.exit(-1);
        }

        Configuration conf = new Configuration();
        conf.set("userAverageRating", args[0]);     // user average rating data, used in reducer.setup()
        Job job = Job.getInstance(conf, "Matrix Cell Multiplication");

        job.setJarByClass(MatrixCellMultiplication.class);
        job.setReducerClass(CellReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        MultipleInputs.addInputPath(job, new Path(args[1]), TextInputFormat.class, CooccurrenceCellMapper.class);
        MultipleInputs.addInputPath(job, new Path(args[2]), TextInputFormat.class, UserRatingCellMapper.class);

        FileOutputFormat.setOutputPath(job, new Path(args[3]));

        job.waitForCompletion(true);
    }
}
