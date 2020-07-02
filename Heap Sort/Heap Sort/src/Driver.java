import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

public class Driver {

	public static void main(String args[])
	{
		Scanner inFile; 
		PrintWriter fileOut1;
		PrintWriter fileOut2;
		
		try
		{
			//Open the input file and both output files.
			inFile = new Scanner(new FileReader(args[0]));
			fileOut1= new PrintWriter( new FileOutputStream(args[1]));
			fileOut2= new PrintWriter( new FileOutputStream(args[2]));
			
			//Create a new heap.
			HeapSort heap = new HeapSort();
			heap.setNumItems(heap.countData(inFile));
			heap.setHeapAry(); //Allocate the heapAry to numItems+1
			inFile.close();
			
			//Populate the heap
			heap.buildHeap(inFile, fileOut1, args[0]);
			
			//Delete the heap and close the input/output files.
			heap.deleteHeap(fileOut1, fileOut2);
			fileOut1.close();
			fileOut2.close();
			inFile.close();
		}
		catch(FileNotFoundException e) //Output error message if input or output files cannot be found.
		{
			e.printStackTrace();
		}
		catch(IOException i)
		{
			i.printStackTrace();
		}
	}
}
