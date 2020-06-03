import java.lang.IllegalArgumentException;
import java.util.*;

class HeapNode<K extends Comparable<K>> implements Comparable<HeapNode<K>> {
	private K key;
	
	public HeapNode(K key) {
		this.key = key;
	}
	public K getKey() { return this.key; }
	public void setKey(K key) { this.key = key; }
	
	public int compareTo(HeapNode<K> node) { return this.key.compareTo(node.getKey()); }
}

class Heap<T extends Comparable<T>> implements Iterable<T> {
	private List<HeapNode<T>> nodes;
	
	public Heap() {
		nodes = new ArrayList<>();
	}
	
	public int getIndex(HeapNode<T> u) { return nodes.indexOf(u); }
	public boolean hasLeftChild(HeapNode<T> u) { return (this.getIndex(u)*2+1 < nodes.size()); }
	public HeapNode<T> getLeftChild(HeapNode<T> u) { 
		if (this.hasLeftChild(u)) {
			return nodes.get(this.getIndex(u)*2+1); 
		}
		return null;
	}
	public boolean hasRightChild(HeapNode<T> u) { return (this.getIndex(u)*2+2 < nodes.size()); }
	public HeapNode<T> getRightChild(HeapNode<T> u) { 
		if (this.hasRightChild(u)) {
			return nodes.get(this.getIndex(u)*2+2);
		}
		return null;
	}
	public boolean hasParent(HeapNode<T> u) { return ((this.getIndex(u)-1)/2 >= 0); }
	public HeapNode<T> getParent(HeapNode<T> u) {
		if (this.hasParent(u)) {
			return nodes.get((this.getIndex(u)-1)/2);
		}
		return null;
	}
	public boolean isLeaf(HeapNode<T> u) { return (!this.hasLeftChild(u) && !this.hasRightChild(u)); }
	public boolean isInternal(HeapNode<T> u) { return !isLeaf(u); }
	public boolean isRoot(HeapNode<T> u) { return !hasParent(u); }
	
	public void add(T key) {
		HeapNode n = new HeapNode(key);
		nodes.add(n);
		upheap(n);
	}
	
	public void upheap(HeapNode<T> u) {
		HeapNode<T> v = u;
		while (!isRoot(v)) {
			if (v.compareTo(this.getParent(v)) < 0) {
				// swap v and parent
				HeapNode<T> temp = this.getParent(v);
				int tempIndex = this.getIndex(v);
				nodes.set(this.getIndex(this.getParent(v)), v);
				nodes.set(tempIndex, temp);
			} else {
				break;
			}
		}
	}
	
	public Iterator iterator() {
		List<T> l = new ArrayList<>();
		for (HeapNode<T> n: nodes) {
			l.add(n.getKey());
		}
		return l.iterator(); 
	}
	
	public T min() {
		return this.nodes.get(0).getKey();
	}
	
	public T removeMin() {
		int firstidx = 0;
		int lastidx = this.nodes.size() - 1;
		
		// swap min and bottom node
		HeapNode<T> min = this.nodes.get(firstidx);
		HeapNode<T> last = this.nodes.get(lastidx);
		nodes.set(firstidx, last);
		nodes.remove(lastidx);
		
		// downheap this new node
		downheap(last);
		
		// return the old key
		return min.getKey();
	}
	
	public void downheap(HeapNode<T> node) {
		// base case: the node is a leaf
		if (this.isLeaf(node)) {
			return;
		}
		
		HeapNode<T> toSwap = null;
		// may only have one child, in which case we must swap
		if (this.hasRightChild(node) && !this.hasLeftChild(node)) {
			if (node.compareTo(this.getRightChild(node)) > 0) {
				toSwap = this.getRightChild(node);
			}
		} else if (this.hasLeftChild(node) && !this.hasRightChild(node)) {
			if (node.compareTo(this.getLeftChild(node)) > 0) {
				toSwap = this.getLeftChild(node);
			}
		} else {
			HeapNode<T> left = this.getLeftChild(node);
			HeapNode<T> right = this.getRightChild(node);
			if (node.compareTo(left) < 0 && node.compareTo(right) < 0) {
				return;
			}
			
			// always swap with left first
			if (node.compareTo(left) > 0 && left.compareTo(right) < 0) {
				toSwap = left;
			} else if (node.compareTo(right) > 0) {
				toSwap = right;
			}	
		}
		
		if (toSwap == null) {
			return;
		}
		
		// swap the nodes
		int nodeIndex = this.getIndex(node);
		int toSwapIndex = this.getIndex(toSwap);
		nodes.set(nodeIndex, toSwap);
		nodes.set(toSwapIndex, node);
		
		// recursively downheap
		downheap(node);
	}
	
	public String toString() {
		String out = "";
		for (T i : this) {
			out += i + " ";
		}
		return out;
	}
}

class HeapImplementation {
	public static void main(String[] args) {
		Heap<Integer> h = new Heap<Integer>();
		h.add(5);
		h.add(3);
		h.add(1);
		h.add(2);
		h.add(6);
		System.out.println(h);
		System.out.println(h.removeMin());
		System.out.println(h);
	}
}