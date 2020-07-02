import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.Scanner;

public class Driver {

	public static void main(String args[])
	{
		try
		{

			Scanner inFile1 = new Scanner(new FileReader(args[0]));
			Scanner inFile2 = new Scanner(new FileReader(args[1]));
			PrintWriter outFile = new PrintWriter( new FileOutputStream(args[2]));
			
			//Get the number of nodes from inputFile1 containing the graph.
			int numNodes;
			numNodes = inFile1.nextInt();
			Scheduling schedule = new Scheduling(); //Create a new class object and invoke the default constructor.
			schedule.setNumNodes(numNodes);
			schedule.loadMatrix(inFile1); //Load the graph from input file 1 into the 2d array
			
			int totalJobTime = schedule.computeTotalJobTimes(inFile2); //Calculates the total job time required.
			
			int procGiven;
			Scanner in = new Scanner(System.in);
			System.out.println("How many processors are needed?");
			procGiven=in.nextInt();
			in.close();
			
			if(procGiven <= 0)
			{
				System.err.println("The number of processors provided is invalid. Please try again with a correct value!");
				System.exit(1);
			}
			else if(procGiven > numNodes)
			{
				//Adjust the number of processors to be equal to numNodes if too many were provided.
				procGiven = numNodes;
			}
			else
			{
				//Number of processors provided is adequate. No changes need to be made.
			}

			schedule.allocateArrays(procGiven);
			int procUsed = 0;
			int currentTime = 0;
			
			while(schedule.jobDoneHasItems()){
					int orphanNode = schedule.getUnmarkedOrhpan();
					while(orphanNode != -1)
					{
						schedule.markJob(orphanNode);
						schedule.insertOpen(orphanNode);
						orphanNode = schedule.getUnmarkedOrhpan();
					}		
					
					schedule.printList();
					
					while(!schedule.openIsEmpty() && procUsed<procGiven)
					{
						int availProcessor = schedule.findProcessor(procGiven);
						if(availProcessor > 0)
						{
							procUsed++;
						}
						
						Node newJob = schedule.removeFront(); //Use overloaded assignment operator to copy the node from the front of the open linked list.
						schedule.processJob[availProcessor] = newJob.jobId;
						schedule.processTime[availProcessor] = newJob.jobTime;
						schedule.updateTable(availProcessor, newJob, currentTime);
					}
					
					if(schedule.checkCycle(procGiven)== true)
					{
						outFile.println("The graph provided in the input file contains cycles. Exiting program!");
						inFile1.close();
						inFile2.close();
						outFile.close();
						System.exit(1);
					}
					
					schedule.printTable(outFile, procGiven, currentTime);
					
					currentTime++;
					
					for(int i = 1; i <=procGiven; i++)
					{
						if(schedule.processTime[i] > 0)
							schedule.processTime[i]--;	
					}
					
					int job = schedule.findDoneJob(procGiven);
					while(job!= -1)
					{
						procUsed--;
						schedule.deleteNode(job);
						schedule.deleteEdge(job);
						job = schedule.findDoneJob(procGiven);
					}
					
					//The following output statements are for debugging purposes.
					
					System.out.println("Current Time: " + currentTime);
					schedule.printDebug(procGiven);
					}
					inFile1.close();
					inFile2.close();
					outFile.close();
		}
		catch(FileNotFoundException e) //Output error message if input or output files cannot be found.
		{
			e.printStackTrace();
		}

	}



}
