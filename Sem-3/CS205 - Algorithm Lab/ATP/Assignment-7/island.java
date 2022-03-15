import java.util.*;

//https://www.hackerearth.com/practice/algorithms/graphs/topological-sort/practice-problems/algorithm/lonelyisland-49054110/description/

class TestClass {

	public static void main(String[] args) {

		Scanner s = new Scanner(System.in);

		int numberOfIslands = s.nextInt();

		int numberOfBridges = s.nextInt();

		int startingIsland = s.nextInt();

		startingIsland--;

		int[] indegree = new int[numberOfIslands];

		int[] outdegree = new int[numberOfIslands];

		HashMap<Integer, List<Integer>> adjList = new HashMap<Integer,List<Integer>>();

		for (int i = 0; i < numberOfIslands; i++) {

			adjList.put(i, new LinkedList<Integer>());

		}

		int start;

		int end;

		for (int i = 0; i < numberOfBridges; i++) {

			start = s.nextInt() - 1;

			end = s.nextInt() - 1;

			indegree[end]++;

			outdegree[start]++;

			adjList.get(start).add(end);

		}

		s.close();

		Queue<Integer> islandWithZeroIndegree = new LinkedList<>();

		for (int i = 0; i < numberOfIslands; i++) {

			if (indegree[i] == 0) {

				islandWithZeroIndegree.add(i);

			}

		}

		double[] probabilty = new double[numberOfIslands];

		probabilty[startingIsland] = 1;

		while (!islandWithZeroIndegree.isEmpty()) {

			int current = islandWithZeroIndegree.remove();

			for (Integer i : adjList.get(current)) {

				probabilty[i] += (probabilty[current] * ((double)1 / (double)outdegree[current]));

				indegree[i]--;

				if (indegree[i] == 0) {

					islandWithZeroIndegree.add(i);

				}

			}

		}

		double max = 0;

		for (int i = 0; i < numberOfIslands; i++) {

			if (outdegree[i] == 0 && (probabilty[i] > max)) {

				max = probabilty[i];

			}

		}

		for(int i=0;i<numberOfIslands;i++) {

			if(outdegree[i]==0 && Math.abs(probabilty[i]-max) < 1e-9) {

				System.out.print(i+1+" ");

			}

		}

	}

}

