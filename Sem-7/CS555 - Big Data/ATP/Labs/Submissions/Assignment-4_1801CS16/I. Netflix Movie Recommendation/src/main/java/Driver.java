public class Driver {
    public static void main(String[] args) throws Exception {
        RatingDataReader ratingDataReader = new RatingDataReader();
        CooccurrenceMatrixGenerator cooccurrenceMatrixGenerator = new CooccurrenceMatrixGenerator();
        CooccurrenceMatrixNormalization cooccurrenceMatrixNormalization = new CooccurrenceMatrixNormalization();
        UserRatingAveraging userRatingAveraging = new UserRatingAveraging();
        MatrixCellMultiplication matrixCellMultiplication = new MatrixCellMultiplication();
        MatrixCellSum matrixCellSum = new MatrixCellSum();

        String rawRatingDataInputPath = args[0];
        String aggregatedUserRatingPath = args[1];
        String cooccurrenceGeneratorPath = args[2];
        String cooccurrenceNormalizationPath = args[3];
        String userAverageRatingPath = args[4];
        String matrixCellMultiplicationPath = args[5];
        String matrixCellSumPath = args[6];

        // job1: RatingDataReader.java
        String[] job1Args = {rawRatingDataInputPath, aggregatedUserRatingPath};
        ratingDataReader.main(job1Args);

        // job2: CooccurrenceMatrixGenerator.java
        String[] job2Args = {aggregatedUserRatingPath, cooccurrenceGeneratorPath};
        cooccurrenceMatrixGenerator.main(job2Args);

        // job3: CoooccurrenceMatrixNormalization.java
        String[] job3Args = {cooccurrenceGeneratorPath, cooccurrenceNormalizationPath};
        cooccurrenceMatrixNormalization.main(job3Args);

        //job4: UserRatingAveraging.java
        String[] job4Args = {rawRatingDataInputPath, userAverageRatingPath};
        userRatingAveraging.main(job4Args);

        //job5: MatrixCellMultiplication.java
        String[] job5Args = {userAverageRatingPath, cooccurrenceNormalizationPath, rawRatingDataInputPath, matrixCellMultiplicationPath};
        matrixCellMultiplication.main(job5Args);

        // job6:s MatrixCellSum.java
        String[] job6Args = {matrixCellMultiplicationPath, matrixCellSumPath};
        matrixCellSum.main(job6Args);

        // execution:
        // hadoop jar recommender.jar Driver /input /output/userRating /output/cooccurrence /output/normalization /output/averageRating /output/multiplication /output/sum
    }
}
