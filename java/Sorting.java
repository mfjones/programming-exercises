import java.util.ArrayList;
import java.util.List;
import java.util.PriorityQueue;

class Sorting {
	public static List<Integer> quicksort(List<Integer> array) {
		if (array.size() < 2) {
			return array;
		}
		
		List<Integer> left = new ArrayList<Integer>();
		List<Integer> right = new ArrayList<Integer>();
		List<Integer> equal = new ArrayList<Integer>();
		int pivot = array.get(array.size()/2);
		
		for (int i = 0; i < array.size(); i++) {
			if (array.get(i) > pivot) {
				right.add(array.get(i));
			} else if (array.get(i) < pivot) {
				left.add(array.get(i));
			} else {
				equal.add(array.get(i));
			}
		}
		
		left = quicksort(left);
		right = quicksort(right);
		
		List<Integer> sorted = new ArrayList<Integer>();
		sorted.addAll(left);
		sorted.addAll(equal);
		sorted.addAll(right);
		
		return sorted;
	}
	
	public static int[] selectionSort(int[] array) {
		for (int position = 0; position < array.length; position++) {
			int min = position;
			for (int i = position; i < array.length; i++) {
				if (array[i] < array[min]) {
					min = i;
				}
			}			
			
			int temp = array[position];
			array[position] = array[min];
			array[min] = temp;
		}
		return array;
	}
	
	public static int[] insertionSort(int[] array) {
		for (int i = 1; i < array.length; i++) {
			int value = array[i];
			int position = i;
			
			while (position > 0 && array[position-1] > value) {
				array[position] = array[position-1];
				position--;
			}
			array[position] = value;
		}
		return array;
	}
	
	public static List<Integer> mergeSort(List<Integer> array) {
		if (array.size() < 2) {
			return array;
		}
		
		int mid = array.size()/2;
		
		List<Integer> left = array.subList(0, mid);
		List<Integer> right = array.subList(mid, array.size());
		
		return merge(mergeSort(left), mergeSort(right));
	}
	
	public static List<Integer> merge(List<Integer> arr1, List<Integer> arr2) {
		List<Integer> finalArr = new ArrayList<Integer>();
		
		int j = 0, k = 0;
		while (j < arr1.size() && k < arr2.size()) {
			if (arr1.get(j) < arr2.get(k)) {
				finalArr.add(arr1.get(j));
				j++;
			} else {
				finalArr.add(arr2.get(k));
				k++;
			}
		}
		
		// add left over items
		for (; j < arr1.size(); j++) {
			finalArr.add(arr1.get(j));
		}
		for (; k < arr2.size(); k++) {
			finalArr.add(arr2.get(k));
		}
		
		return finalArr;
	}
	
	public static List<Integer> heapSort(List<Integer> array) {
		PriorityQueue<Integer> heap = new PriorityQueue<Integer>();
		
		for (int i = 0; i < array.size(); i++) {
			heap.offer(array.get(i));
		}
		
		int heapSize = heap.size();
		List<Integer> finalArr = new ArrayList<Integer>();
		for (int i = 0; i < heapSize; i++) {
			finalArr.add(heap.poll());
		}
		
		return finalArr;
	}
	
	public static int binarySearch(int[] array, int key, int start, int end) {
		if (end - start < 1) {
			return -1;
		}
		
		int mid = (end - start)/2 + start;
		if (key > array[mid]) {
			return binarySearch(array, key, mid+1, end);
		} else if (key < array[mid]) {
			return binarySearch(array, key, start, mid);
		} else {
			return mid;
		}
	}
	
	public static void printArray(int[] array, String msg) {
		System.out.print(msg + ":\n[");
		for (int i = 0; i < array.length; i++) {
			if (i == array.length - 1) {
				System.out.print(array[i] + "]");
			} else {
				System.out.print(array[i] + ", ");
			}

		}
		System.out.println();
	}
	
	public static void main(String[] args) {
		List<Integer> arr = new ArrayList<Integer>();
		arr.add(5); arr.add(1); arr.add(8); arr.add(2);
		arr.add(10); arr.add(1);arr.add(12);
		
		int[] arr2 = {5,1,8,2,10,1,12};
		int[] arr3 = {5,1,8,2,10,12};
		
		// testing quicksort
		System.out.println("Quick sort:");
		System.out.println(quicksort(arr));
		
		// testing insertion/selection sort
		printArray(insertionSort(arr2), "Insertion sort");
		// printArray(selectionSort(arr2), "Selection sort");
		
		// testing merge sort
		System.out.println("Merge sort:");
		System.out.println(mergeSort(arr));
		
		// testing heap sort
		System.out.println("Heap sort:");
		System.out.println(heapSort(arr));
		
		// testing binary search
		System.out.println("Binary search:");
		System.out.println("Position of 10: " + binarySearch(arr3, 10, 0, arr3.length));
	}
}