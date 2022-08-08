import java.util.ArrayList;

public class PRNode {

	private boolean isNode = true;
	private int nodeId;
	private double pagerank = 1.0;
	private ArrayList<Integer> adjacencyList;

	public PRNode(String nodeString) {
		String[] nodes = nodeString.split(",");
		// this is a node : "node,1,2,3"
		if (nodes[0].equals("node")) {
			// ignore dangling node
			if (nodes.length > 2) {
				this.nodeId = Integer.parseInt(nodes[1]);
				this.pagerank = 1.0;
				ArrayList<Integer> list = new ArrayList<Integer>(nodes.length - 1);
				for (int i = 2; i < nodes.length; i ++) {
					list.add(Integer.parseInt(nodes[i]));
				}
				this.adjacencyList = list;
				this.isNode = true;
			}
		// this is not a node: "not,0.2"
		} else {
			this.isNode = false;
			this.pagerank = Double.parseDouble(nodes[1]);
		}
		
	}
	
	public boolean isNode() {
		return isNode;
	}

	public int getNodeId() {
		return nodeId;
	}

	public void setNodeId(int nodeId) {
		this.nodeId = nodeId;
	}

	public double getPagerank() {
		return pagerank;
	}

	public void setPagerank(double pagerank) {
		this.pagerank = pagerank;
	}

	public ArrayList<Integer> getAdjacencyList() {
		return adjacencyList;
	}

	public void setAdjacencyList(ArrayList<Integer> adjacencyList) {
		this.adjacencyList = adjacencyList;
	}

	public boolean containsList() {
		return !adjacencyList.isEmpty();
	}

	public int adjacencyListSize() {
		return adjacencyList.size();
	}

	@Override
	public String toString() {
		String result = "";
		if (isNode) {
			for (int temp:adjacencyList) {
				result = result + temp + ",";
			}
			result = result.substring(0, result.length() - 1);
			return "node," + nodeId + "," + result;
		} else {
			return "not," + pagerank;
		}
	}
}
