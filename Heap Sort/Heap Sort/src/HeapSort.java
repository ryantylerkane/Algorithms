import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.Scanner;

public class HeapSort {
	
	private int rootIndex;
	private int fatherIndex;
	private int leftKidIndex;
	private int rightKidIndex;
	private int minKidIndex;
	private int numItems;
	private int [] heapAry;
	
	public HeapSort() //Default constructor
	{
		rootIndex = 1;
		fatherIndex = 0;
		leftKidIndex = 0;
		rightKidIndex = 0;
		minKidIndex = 0;
		numItems = 0;
		//Do not allocate the heapAry until you get a count of the number of items.
	}
	
	public void setNumItems(int items)
	{
		numItems = items;
		
	}
	public void setHeapAry()
	{
		heapAry = new int [numItems +1];
		heapAry[0] = 0;
	}
	
	public int getRoot() //Returns the data at the root of the heap.
	{
		return heapAry[rootIndex];
	}
	
	public Boolean isRoot(int index) //Function returns true if the provided index is the root of the heap
	{
		if(index==1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public Boolean isLeaf(int index)
	{
		leftKidIndex = index*2;
		rightKidIndex = index*2+1;
		if(leftKidIndex > heapAry[0] && rightKidIndex > heapAry[0])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public int countData(Scanner inFile) //Returns the number of elements in the heap.
	{
		int totalCount = 0;
		
			while(inFile.hasNextInt())
			{
				totalCount++;
				inFile.nextInt();
			}
			return totalCount;
		}
	
	public Boolean isHeapEmpty()
	{
		if(heapAry[0]==0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public Boolean isHeapFull()
	{
		if(heapAry[0] == numItems+1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public void insertOneDataItem(int data)
	{
		heapAry[0]++; //Increase the count of the number of items in the heap.
		if(!isHeapFull()) //Check to make sure you are not adding an element that would be out of bounds.
		{
			heapAry[heapAry[0]]= data; //Add the new item to the end of the heap.
		}
		else
		{
			heapAry[0]--; //Since no element is going to be added, subtract one from the count.
			System.err.println("You are attempting to add to a full heap! Please remove an item and try again!");
		}
	}
	
	public void buildHeap(Scanner inFile, PrintWriter fileOut1, String inputArg)
	{
		//RootIndex set = 1 in the constructor
		int kidIndex;
		try {
			inFile = new Scanner(new FileReader(inputArg));
			while(inFile.hasNextInt())
			{
				insertOneDataItem(inFile.nextInt());
				kidIndex = heapAry[0];
				bubbleUp(kidIndex);
				printHeap(fileOut1);//To output file 1.
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
	
	private void bubbleUp(int kidIndex)
	{
		if(isRoot(kidIndex))
		{
			//If the kidIndex is the root, then there is no data to swap.
		}
		else //The recently added data is not located at the root.
		{
			fatherIndex = kidIndex/2;
			
			if(heapAry[kidIndex] >= heapAry[fatherIndex])
			{
				//There is no more data to swap.
			}
			else
			{
				//Swap the father and the kid index.
				int temp = heapAry[kidIndex];
				heapAry[kidIndex] = heapAry[fatherIndex];
				heapAry[fatherIndex] = temp;
				bubbleUp(fatherIndex); //Recursively call bubble up on the father index.
			}
		}
	}
	
	public void deleteHeap(PrintWriter outFile1, PrintWriter outFile2)
	{
		int data;
		while(!isHeapEmpty())
		{
			data = getRoot();
			outFile2.println(data);
			replaceRoot();
			fatherIndex = rootIndex;
			bubbleDown(fatherIndex);
			printHeap(outFile1);
		}
	}
	
	private void bubbleDown(int fatherIndex)
	{
		if(isLeaf(fatherIndex))
		{
			//If you reach the leaf, there is no need to make any more recursive calls.
		}
		else //We are at an internal node or the root
		{
			leftKidIndex = fatherIndex * 2;
			rightKidIndex = fatherIndex * 2 +1;
			if(rightKidIndex > heapAry[0]) //If the rightKidIndex is out of bounds, there is no reason to check it.
			{
				minKidIndex = leftKidIndex; //We know that the left is the only child since we already checked if the father is a leaf.
			}
			else //Neither the left nor the right child are out of bounds.
			{
				minKidIndex = findMinKidIndex(leftKidIndex, rightKidIndex);
			}
			if(heapAry[minKidIndex] >= heapAry[fatherIndex])
			{
				//If the kid is greater than the parent, then we have found the right location for the node.
			}
			else
			{
				//Swap the values held by minKidIndex and fatherIndex
				int temp = heapAry[fatherIndex];
				heapAry[fatherIndex] = heapAry[minKidIndex];
				heapAry[minKidIndex] = temp;
				bubbleDown(minKidIndex);
			}
		}
	}
	
	private int findMinKidIndex(int left, int right)
	{
		if(heapAry[right] <= heapAry[left])
		{
			return right;
		}
		else
		{
			return left;
		}
	}
	
	private void replaceRoot()
	{
		if(!isHeapEmpty())
		{
			//Set the root equal to the last element in the array.
			heapAry[rootIndex] = heapAry[heapAry[0]];
			heapAry[0]--; //Decrease the size of the heap.
		}
		else
		{
			System.err.println("You are attempting to remove from an empty"
					+ "heap. Please check the number of elements and try again!");
		}
	}
	
	public void printHeap(PrintWriter outFile)
	{
		outFile.println("Number of elements in the heap: " + heapAry[0]);
		for(int i = 1; i <=heapAry[0]; i++)
		{
			outFile.print(heapAry[i] + " ");
		}
		
		outFile.println();
		outFile.println();
	}

}
