#include <iostream>
#include <fstream>
#include <stdexcept>

using namespace std;

class Node
	{
	public:	
		int jobId;
		int jobTime;
		Node *next; 
	
		Node() //Default Class Constructor
		{
			jobId = -1; //Use -1 as default to avod duplicates in the event that a job has an id of zero.
			jobTime = 0;
			next = NULL;
		}
		
		Node(int id, int time) //Parameterized constructor to create a node given the id and time.
		{
			jobId = id;
			jobTime = time;
			next = NULL;
		}
		
		const Node& operator= (const Node& otherNode)
	{
		if (this != &otherNode) //Checks for self-assignment.
		{
			//Make sure the object being assigned has all NULL pointers.
			if (next != NULL)
			{
				delete next;
			}
		//Once we know the object being copied to is null, we can simply call the copy constructor.
			jobId = otherNode.jobId;
			jobTime = otherNode.jobTime;
		}
		else
		{
			cout << "Assignment not performed as this would result in self-assignment!" << endl;
		}

		return *this;
	}

	};//End inner class node

class Scheduling{
	
private:
	int numNodes; //Total number of nodes in the graph
	int totalJobTimes;
	int **adjacencyMatrix;
	int **scheduleTable;
	//All arrays will be a length of numNodes + 1 (since address 0 will not be used)
	int *jobTimeArray;
	int *parentCount;
	int *jobDone;
	int *jobMarked;
	Node *open;
	bool processJobIsEmpty(int procGiven)
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
	


public:

	int *processJob;
	int *processTime;

	Scheduling()
	{
		numNodes = 0;
		totalJobTimes = 0;
		adjacencyMatrix = NULL;
		scheduleTable = NULL;
		jobTimeArray = NULL;
		processJob = NULL;
		processTime = NULL;
		parentCount = NULL;
		jobDone = NULL;
		jobMarked = NULL;
		open = NULL;
	}
	bool jobDoneHasItems()
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

	void loadMatrix(ifstream& inFile1)
	{
		adjacencyMatrix = new int*[numNodes + 1]();
		for (int i = 0; i < numNodes + 1; i++)
		{
			adjacencyMatrix[i] = new int[numNodes + 1]();
		}

		int node1;
		int node2;

		while (inFile1 >> node1)
		{
			inFile1 >> node2;
			adjacencyMatrix[node1][node2]++; //Increase the value to indicate a dependency.
		}

	}
	
	int computeTotalJobTimes(ifstream& inFile2)
	{
		int totalNodes;
		inFile2 >> totalNodes;
		if(totalNodes != numNodes)
		{
			throw runtime_error("The number of nodes provided in the the input files do not match. Please try again!");
			
		}
		else
		{
			jobTimeArray = new int[numNodes+1]();
			
			int nodeNum;
			int jobTime;
			
			for(int i = 0; i < numNodes; i++)
			{
				inFile2 >> nodeNum;
				inFile2 >> jobTime;
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
		scheduleTable = new int*[numProc+1]();
		for(int i = 0; i < numNodes+1; i++)
		{
			scheduleTable[i] = new int[totalJobTimes +1]();
		}
		
		processJob = new int[numProc+1](); //Use numProc+1 so we can add from Processor 1 onwards.
		processTime = new int [numProc+1](); //Use numProc+1 so we can add from Processor 1 onwards.
		
		//Intitialize the vales in the parentCount array by traversing the column of the adjacency matrix.
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
		
		jobDone = new int[numNodes+1]();
		jobMarked = new int[numNodes+1]();	
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
		Node *newNode = new Node(orphanNode, jobTimeArray[orphanNode]);
		if(open == NULL) //Linked list is empty.
		{
			open = newNode;
		}
		else if(open->next==NULL) //There is only one node in the linked list.
		{
			if(newNode->jobTime < open->jobTime || newNode->jobTime==open->jobTime)
			{
				newNode->next = open;
				open = newNode;
			}
			else
			{
				open->next = newNode;
			}
		}
		else
		{
			
			if(open->jobTime >= newNode->jobTime)
			{
				newNode->next = open;
				open=newNode;
			}
			else
			{
			
				Node *current = open;
				while(current->next != NULL && newNode->jobTime > current->next->jobTime)
				{
					current = current->next;
					
				}
					newNode->next = current->next;
					current->next = newNode; 
			}
		}
	}
	
	Node* removeFront()
	{
		if (!openIsEmpty())
		{
			Node *temp = open;
			if (open->next == NULL) //You are removing the last value from the stack.
			{
				open=NULL;
				return temp;
			}
			else
			{
				open = open->next;
				temp->next = NULL; //Make the next pointer NULL to avoid returning all linked nodes.
				return temp;
			}
		}
		else
		{
			cout << "Unable to remove a node from open since it is empty. Please add items and try again!" << endl;
			return NULL;
		}
	}
	
	bool openIsEmpty()
	{
		if(open==NULL)
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
		cout << "Head -> ";

		Node* temp = open;

		while (temp != NULL)
		{
			cout << "(" << temp->jobId << ", " << temp->jobTime << ", ";
			if (temp->next == NULL)
			{
				cout << "NULL";
			}
			else
			{
				cout << temp->next->jobId;
			}
			cout << ") -> ";
			
			temp = temp->next;
		}
		cout << "NULL" << endl;
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
	
	void updateTable(int availProcessor, Node* newJob, int currentTime)
	{
		for(int i = currentTime; i <= currentTime+newJob->jobTime-1; i++)
		{
			scheduleTable[availProcessor][i] = newJob->jobId;
		}
	}
	
	
	bool checkCycle(int procGiven)
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
	
	void printTable(ofstream& outFile, int procGiven, int currentTime)
	{
		outFile << "--";
		for(int i =0; i < totalJobTimes; i++)
		{
			outFile << "---" << i;
		}
		outFile << endl;
		for(int i = 1; i <= procGiven; i++)
		{
			outFile << "P("<<i<<")| ";
			for(int j = 0; j <=currentTime; j++)
			{
				if(scheduleTable[i][j] > 0)
				{
					outFile << scheduleTable[i][j] << "  |  ";
				}
				else
				{
					outFile << "-  |  ";
				}
			}
			
			outFile << endl;
		}
		outFile << endl;
	}
	
	void printDebug(int procGiven)
	{
		cout << "Job Marked Array: ";
		for(int i = 1; i <= numNodes; i++)
		{
			cout << jobMarked[i] << " ";
		}
		cout << endl;
		cout << "Process Time Array: ";
		for(int i = 1; i <= procGiven; i++)
		{
			cout << processTime[i] << " ";
		}
		cout << endl;
		cout << "Process Job Array: ";
		for(int i = 1; i <= procGiven; i++)
		{
			cout << processJob[i] << " ";
		}
		cout << endl;
		cout << "Job Done Array: ";
		for(int i = 1; i <= numNodes; i++)
		{
			cout << jobDone[i] << " ";
		}
		cout << endl;
		cout << "--------------------------" << endl;
	}
		
};

int main(int argc, char** argv)
{
	if (argc > 3)
	{

		ifstream inFile1;
		ifstream inFile2;
		ofstream outFile;
		inFile1.open(argv[1]);
		inFile2.open(argv[2]);
		outFile.open(argv[3]);
		
		//Get the number of nodes from inputFile1 containing the graph.
		int numNodes;
		inFile1 >> numNodes;
		Scheduling schedule; //Create a new class object and invoke the default constructor.
		schedule.setNumNodes(numNodes);
		schedule.loadMatrix(inFile1); //Load the graph from input file 1 into the 2d array
		
		int totalJobTime = schedule.computeTotalJobTimes(inFile2); //Calculates the total job time required.
		
		int procGiven;
		cout << "How many processors are needed?" << endl;
		cin >> procGiven;
		
		if(procGiven <= 0)
		{
			cout << "The number of processors provided is invalid. Please try again with a correct value!" << endl;
			return 1;
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
					
					Node* newJob = schedule.removeFront(); //Use overloaded assignment operator to copy the node from the front of the open linked list.
					schedule.processJob[availProcessor] = newJob->jobId;
					schedule.processTime[availProcessor] = newJob->jobTime;
					schedule.updateTable(availProcessor, newJob, currentTime);
				}
				
				if(schedule.checkCycle(procGiven)>= 1)
				{
					outFile << "The graph provided in the input file contains cycles. Exiting program!" << endl;
					inFile1.close();
					inFile2.close();
					outFile.close();
					return 1;
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
				
				cout << "Current Time: " << currentTime << endl;
				schedule.printDebug(procGiven);
				}
				//schedule.printTable(outFile, procGiven, currentTime);
				inFile1.close();
				inFile2.close();
				outFile.close();
	}
	else
	{
		cout << "Insufficient number of input/output files loaded. Please try again!" << endl;
	}

	return 0;
}

