import java.util.*;

class MapNode<K,V> {
	private K key;
	private V value;

	public MapNode(K key, V value) {
		this.key = key;
		this.value = value;
	}

	public void setKey(K key) { this.key = key; }
	public void setValue(V value) { this.value = value; }
	public K getKey() { return this.key; }
	public V getValue() { return this.value; }
}

class CMap<K extends Comparable<K>,V> implements Iterable<V> {
	protected int cap;
	protected List<MapNode<K,V>> items;

	public CMap() {
		cap = 10;
		items = new ArrayList<MapNode<K,V>>(cap);
	}

	public boolean keyExists(K key) {
		for (MapNode<K,V> m: items) {
			if (m.getKey().compareTo(key) == 0) {
				return true;
			}
		}
		return false;
	}

	public V put(K key, V value) {
		// see if we can find key
		if (this.keyExists(key)) {
			V old_val;
			for (MapNode<K,V> m: items) {
				if (m.getKey().compareTo(key) == 0) {
					old_val = m.getValue();
					m.setValue(value);
					return old_val;
				}
			}
		}

		// key doesn't exist, find next position
		MapNode<K,V> new_node = new MapNode<K,V>(key, value);
		items.add(new_node);
		return value;
	}

	public V get(K key) {
		// if exists, loop through map
		if (this.keyExists(key)) {
			for (MapNode<K,V> m: items) {
				if (m.getKey().compareTo(key) == 0) {
					return m.getValue();
				}
			}
		}
		return null;
	}

	public Iterator iterator() {
		List<V> l = new ArrayList<V>();
		for (MapNode<K,V> m: items) {
			l.add(m.getValue());
		}

		return l.iterator();
	}

	public List<MapNode<K,V>> items() {
		return this.items;
	}
}

class HashCMap<K extends Comparable<K>,V> extends CMap<K,V> {
	protected int cap;
	protected List<CMap<K,V>> buckets;
	private int shift, scale, prime, size;
	private Random r;

	public HashCMap() {
		r = new Random();
		cap = 10;
		prime = 11;
		shift = r.nextInt(prime+1)-1;
		scale = r.nextInt(prime);
		buckets = new ArrayList<>(cap);
		for (int i = 0; i < cap; i++) {
			buckets.add(new CMap<K,V>());
		}
	}

	private int hashCode(K key) {
		if (key instanceof Integer) {
			return (((scale*(Integer)key + shift) % prime) % cap);
		} else {
			return key.hashCode();
		}
	}

	@Override
	public V put(K key, V value) {
		CMap<K,V> bucket = buckets.get(this.hashCode(key));
		for (MapNode<K,V> m: bucket.items()) {
			if (m.getKey().compareTo(key) == 0) {
				V old_val = m.getValue();
				m.setValue(value);
				return old_val;
			}
		}

		bucket.put(key,value);
		return value;
	}

	@Override
	public V get(K key) {
		CMap<K,V> bucket = buckets.get(this.hashCode(key));

		if (bucket != null) {
			for (MapNode<K,V> m: bucket.items()) {
				if (m.getKey().compareTo(key) == 0) {
					return m.getValue();
				}
			}
		}

		return null;
	}


}

class MapImplementation {
	public static void main(String[] args) {
		HashCMap<Integer,String> map = new HashCMap<Integer,String>();
		map.put(1,"hi");
		map.put(2,"hello");
		System.out.println(map.get(1));
		System.out.println(map.get(2));
		System.out.println("----");
		map.put(1,"blah");
		System.out.println(map.get(1));
		System.out.println(map.get(2));
	}
}
