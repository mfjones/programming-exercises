import java.util.*;

class Node<T extends Comparable<T>> {
	private T value;
	private ArrayList<Node<T>> neighbours;
	private boolean visited;
	
	public Node(T value) {
		this.value = value;
		neighbours = new ArrayList<>();
		this.visited = false;
	}
	
	public T getValue() { return this.value; }
	public ArrayList<Node<T>> getNeighbours() { return this.neighbours; }
	public void addNeighbour(Node<T> n) {
		this.neighbours.add(n);
	}
	
	public void visit() { this.visited = true; }
	public void resetVisit() { this.visited = false; }
	public boolean visited() { return this.visited; }
	
	public String toString() { return this.value.toString(); }
}

class Graph<T extends Comparable<T>> {
	private ArrayList<Node<T>> nodes;
	
	public Graph() {
		nodes = new ArrayList<Node<T>>();
	}
	
	private Node nodeWithValue(T val) {
		for (Node n : this.nodes) {
			if (n.getValue() == val) {
				return n;
			}
		}		
		return null;
	}
	
	public boolean nodeExists(T val) {
		return (null != this.nodeWithValue(val));
	}
	
	public boolean edgeExists(T i, T j) {
		if (!this.nodeExists(i) || !this.nodeExists(j)) {
			return false;
		}
		
		return this.nodeWithValue(i).getNeighbours().contains(j);
	}
	
	public void addNode(T n) {
		this.nodes.add(new Node<T>(n));
	}
	
	public void addEdge(T i, T j) {
		if (!this.nodeExists(i) || !this.nodeExists(j)) {
			return;
		}
		
		Node u = this.nodeWithValue(i);
		Node v = this.nodeWithValue(j);
		u.addNeighbour(v);
		v.addNeighbour(u);
	}
	
	private void resetVisits() {
		for (Node n : this.nodes) {
			n.resetVisit();
		}
	}
	
	public boolean visitedAll() {
		for (Node n : this.nodes) {
			if (!n.visited()) {
				return false;
			}
		}
		return true;
	}
	
	public void BFS(T source) {
		if (!this.nodeExists(source)) {
			return;
		}
		this.resetVisits();
		
		LinkedList<Node<T>> queue = new LinkedList<>();
		Node<T> src = this.nodeWithValue(source);
		while (true) {
			queue.add(src);
			for (Node n : this.nodes) {
				if (n.getNeighbours().isEmpty() && !n.visited() && !queue.contains(n)) {
					queue.add(n);
				}	
			}
			
			while (!queue.isEmpty()) {
				Node<T> top = queue.remove();
				top.visit();
				System.out.println(top);

				
				for (Node<T> n: top.getNeighbours()) {
					if (!n.visited() && !queue.contains(n)) {
						queue.add(n);
					}
				}
			}
			
			if (!this.visitedAll()) {
				for (Node n : this.nodes) {
					if (!n.visited()) {
						src = n;
						break;
					}
				}
			} else {
				break;
			}	
		}
	}
	
	public void DFS(T source) {
		if (!this.nodeExists(source)) {
			return;
		}
		this.resetVisits();
		
		LinkedList<Node<T>> stack = new LinkedList<>();
		Node<T> src = this.nodeWithValue(source);
		while (true) {
			stack.push(src);
			for (Node n : this.nodes) {
				if (n.getNeighbours().isEmpty() && !n.visited() && !stack.contains(n)) {
					stack.push(n);
				}	
			}
			
			while (!stack.isEmpty()) {
				Node<T> top = stack.peek();
				if (!top.visited()) {
					top.visit();
					System.out.println(top);	
				}
				
				
				boolean visitedAll = true;
				for (Node n : top.getNeighbours()) {
					if (!n.visited()) {
						visitedAll = false;
					}
				}
				if (visitedAll) {
					stack.pop();
				} else {
					for (Node n : top.getNeighbours()) {
						if (!n.visited()) {
							stack.push(n);
							break;
						}
					}
				}
			}
			
			if (!this.visitedAll()) {
				for (Node n : this.nodes) {
					if (!n.visited()) {
						src = n;
						break;
					}
				}
			} else {
				break;
			}
		}
		
	}
	
}

class GraphImplementation {
  public static void main(String[] args) {
//	// General graph
//  Graph<Integer> G = new Graph<>();
//	G.addNode(1);
//	G.addNode(2);
//	G.addNode(3);
//	G.addNode(4);
//	G.addNode(5);
//	G.addNode(6);
//	G.addEdge(2,3);
//	G.addEdge(5,6);
//	G.addEdge(3,5);
//	G.addEdge(4,6);
//	G.addEdge(1,6);
//	G.addEdge(4,5);
//	
//	G.BFS(1);
//	System.out.println("-------");
//	G.DFS(1);

	// Tree
	Graph<Integer> Tree = new Graph<>();
	Tree.addNode(1); Tree.addNode(2); Tree.addNode(3);
	Tree.addNode(4); Tree.addNode(5); Tree.addNode(6);
	Tree.addNode(7); Tree.addNode(8); Tree.addNode(9);
	Tree.addEdge(1,2); Tree.addEdge(1,3); Tree.addEdge(1,4);
	Tree.addEdge(2,5); Tree.addEdge(2,6); Tree.addEdge(6,7);
	Tree.addEdge(4,8); Tree.addEdge(4,9);
	
	
	Tree.BFS(1);
	System.out.println("-------");
	Tree.DFS(1);
  }
}
