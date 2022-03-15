import java.util.*;

class Ass8b {
	public static void main(String[] args) {
		Scanner conin = new Scanner(System.in);
		int n = conin.nextInt();
		int m = conin.nextInt();
		int r = conin.nextInt()-1;
		int[] inD = new int[n];
		int[] outD = new int[n];
		HashMap<Integer, List<Integer>> adjList = new HashMap<Integer,List<Integer>>();
		for(int i = 0; i < n; i++) {
			adjList.put(i, new LinkedList<Integer>());
			inD[i] = outD[i] = 0;
		}
		
		for(int i = 0; i < m; i++) {
			int u = conin.nextInt() - 1;
			int v = conin.nextInt() - 1;
			outD[u]++;
			inD[v]++;
			adjList.get(u).add(v);
		}
		conin.close();
		
		Queue<Integer> inD0V = new LinkedList<>();
		for(int i = 0; i < n; i++)
			if(inD[i] == 0)
				inD0V.add(i);

		double[] prob = new double[n];
		prob[r] = 1;
		while(!inD0V.isEmpty()) {
			int cur = inD0V.remove();
			for (Integer i : adjList.get(cur)) {
				prob[i] += (prob[cur]*(1.0/outD[cur]));
				inD[i]--;
				if (inD[i] == 0) {
					inD0V.add(i);
				}
			}
		}
		
		double max = 0;
		for(int i = 0; i < n; i++) {
			if (outD[i] == 0 && (prob[i] > max)) {
				max = prob[i];
			}
		}
		
		for(int i=0; i<n; i++) {
			if(outD[i]==0 && Math.abs(prob[i]-max) < 1e-9) {
				System.out.print(i+1+" ");
			}
		}
		System.out.println();
	}
}
