import java.io.PrintWriter;
import java.util.Scanner;

public class Scheduling {		
	
		private int numNodes; //Total number of nodes in the graph
		private int totalJobTimes;
		private int[][] adjacencyMatrix;
		private int[][] scheduleTable;
		//All arrays will be a length of numNodes + 1 (since address 0 will not be used)
		private int [] jobTimeArray;
		private int [] parentCount;
		private int [] jobDone;
		private int [] jobMarked;
		private Node open;
		
		Boolean processJobIsEmpty(int procGiven)
		{
			for(int i =1; i<=procGiven; i++)
			{
				if(processJob[i] > 0)
				{
					return false;
				}
			}
			return true;
		}
		

	public int [] processJob;
	public int [] processTime;

		Scheduling()
		{
			numNodes = 0;
			totalJobTimes = 0;
			adjacencyMatrix = null;
			scheduleTable = null;
			jobTimeArray = null;
			processJob = null;
			processTime = null;
			parentCount = null;
			jobDone = null;
			jobMarked = null;
			open = null;
		}
		Boolean jobDoneHasItems()
		{
			for(int i = 1; i <= numNodes; i++)
			{
				if(jobDone[i] == 0)
				{
					return true; //There are still items in the graph.
				}
			}
			return false; //There are no items in the graph.
		}
		void setNumNodes(int nodes)
		{
			numNodes = nodes;
		}

		void loadMatrix(Scanner inFile1)
		{
			adjacencyMatrix = new int[numNodes + 1][numNodes+1];
			int node1 = 0;
			int node2 = 0;

			while (inFile1.hasNext())
			{
				node1 = inFile1.nextInt();
				node2 = inFile1.nextInt();
				adjacencyMatrix[node1][node2]++; //Increase the value to indicate a dependency.
			}

		}
		
		int computeTotalJobTimes(Scanner inFile2)
		{
			int totalNodes=0;
			totalNodes = inFile2.nextInt();
			if(totalNodes != numNodes)
			{
				throw new RuntimeException("The number of nodes provided in the the input files do not match. Please try again!");
				
			}
			else
			{
				jobTimeArray = new int[numNodes+1];
				
				int nodeNum=0;
				int jobTime=0;
				
				for(int i = 0; i < numNodes; i++)
				{
					nodeNum = inFile2.nextInt();
					jobTime = inFile2.nextInt();
					jobTimeArray[nodeNum] = jobTime;
					totalJobTimes+=jobTime;
				}
			}
			
			return totalJobTimes;
		}
		
		void allocateArrays(int numProc)
		{
			/*This function allocates scheduleTable, processJob, processTime,
			parentCount, jobDone and jobMarked */
			
			//Allocate the scheduleTable 2d array.
			scheduleTable = new int[numProc+1][totalJobTimes+1];
			processJob = new int[numProc+1]; //Use numProc+1 so we can add from Processor 1 onwards.
			processTime = new int [numProc+1]; //Use numProc+1 so we can add from Processor 1 onwards.
			
			//Initialize the vales in the parentCount array by traversing the column of the adjacency matrix.
			parentCount = new int[numNodes +1]; 
			for(int i =1; i <=numNodes; i++)
			{
				for(int j = 1; j <=numNodes; j++)
				{
					if(adjacencyMatrix[i][j] > 0)
					{
						parentCount[j]++;
					}
					
				}
			}
			
			jobDone = new int[numNodes+1];
			jobMarked = new int[numNodes+1];	
		}
		
		int getUnmarkedOrhpan()
		{
			for(int i = 1; i <=numNodes; i++)
			{
				if(jobMarked[i] == 0 && parentCount[i]==0)
				{
					return i;
				}
			}
			return -1;
		}
		
		void markJob(int jobNum)
		{
			jobMarked[jobNum] = 1;
		}
		
		void insertOpen(int orphanNode)
		{
			Node newNode = new Node(orphanNode, jobTimeArray[orphanNode]);
			if(open == null) //Linked list is empty.
			{
				open = newNode;
			}
			else if(open.next==null) //There is only one node in the linked list.
			{
				if(newNode.jobTime < open.jobTime || newNode.jobTime==open.jobTime)
				{
					newNode.next = open;
					open = newNode;
				}
				else
				{
					open.next = newNode;
				}
			}
			else
			{
				
				if(open.jobTime >= newNode.jobTime)
				{
					newNode.next = open;
					open=newNode;
				}
				else
				{
				
					Node current = open;
					while(current.next != null && newNode.jobTime > current.next.jobTime)
					{
						current = current.next;
						
					}
						newNode.next = current.next;
						current.next = newNode; 
				}
			}
		}
		
		Node removeFront()
		{
			if (!openIsEmpty())
			{
				Node temp = open;
				if (open.next == null) //You are removing the last value from the stack.
				{
					open=null;
					return temp;
				}
				else
				{
					open = open.next;
					temp.next = null; //Make the next pointer null to avoid returning all linked nodes.
					return temp;
				}
			}
			else
			{
				System.out.println("Unable to remove a node from open since it is empty. Please add items and try again!");
				return null;
			}
		}
		
		Boolean openIsEmpty()
		{
			if(open==null)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		void printList()
		{
			System.out.print("Head -> ");

			Node temp = open;

			while (temp != null)
			{
				System.out.print("(" + temp.jobId + ", " + temp.jobTime + ", ");
				if (temp.next == null)
				{
					System.out.print("null");
				}
				else
				{
					System.out.print(temp.next.jobId);
				}
				System.out.print(") . ");
				
				temp = temp.next;
			}
			System.out.println("null");
		}
		
		int findProcessor(int numProc)
		{
			for(int i =1; i <=numProc+1; i++)
			{
				if(processTime[i] <= 0)
				{
					return i;
				}
			}
			return -1;
		}
		
		void updateTable(int availProcessor, Node newJob, int currentTime)
		{
			for(int i = currentTime; i <= currentTime+newJob.jobTime-1; i++)
			{
				scheduleTable[availProcessor][i] = newJob.jobId;
			}
		}
		
		
		Boolean checkCycle(int procGiven)
		{
			if(openIsEmpty() && processJobIsEmpty(procGiven) && jobDoneHasItems())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		
		int findDoneJob(int procGiven)
		{
			for(int i = 1; i <=procGiven; i++)
			{
				if(processTime[i]==0 && processJob[i] != 0)
				{
					int jobId = processJob[i];
					processJob[i] = 0;
					return jobId;
				}	
			}
			return -1; //No jobs were completed.
		}
		
		void deleteNode(int job)
		{
			jobDone[job] = 1;
		}
		
		void deleteEdge(int job)
		{
			for(int i =1; i <=numNodes; i++)
			{
				if(adjacencyMatrix[job][i] > 0)
				{
					parentCount[i]--;
				}
			}
		}
		
		void printTable(PrintWriter outFile, int procGiven, int currentTime)
		{
			outFile.print("--");
			for(int i =0; i < totalJobTimes; i++)
			{
				outFile.print("---" + i);
			}
			outFile.println();
			for(int i = 1; i <= procGiven; i++)
			{
				outFile.print("P("+i+")| ");
				for(int j = 0; j <=currentTime; j++)
				{
					if(scheduleTable[i][j] > 0)
					{
						outFile.print(scheduleTable[i][j] + "  |  ");
					}
					else
					{
						outFile.print("-  |  ");
					}
				}
				
				outFile.println();
			}
			outFile.println();
		}
		
		void printDebug(int procGiven)
		{
			System.out.print("Job Marked Array: ");
			for(int i = 1; i <= numNodes; i++)
			{
				System.out.print(jobMarked[i] + " ");
			}
			System.out.println();
			System.out.print("Process Time Array: ");
			for(int i = 1; i <= procGiven; i++)
			{
				System.out.print(processTime[i] + " ");
			}
			System.out.println();
			System.out.print("Process Job Array: ");
			for(int i = 1; i <= procGiven; i++)
			{
				System.out.print(processJob[i] + " ");
			}
			System.out.println();
			System.out.print("Job Done Array: ");
			for(int i = 1; i <= numNodes; i++)
			{
				System.out.print(jobDone[i] + " ");
			}
			System.out.println();
			System.out.println("--------------------------");
		}
			
}

	