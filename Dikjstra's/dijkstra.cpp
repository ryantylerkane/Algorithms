#include <iostream>
#include <fstream>

using namespace std;

class DijkstraSSS
{
	int numNodes;
	int sourceNode;
	int minNode;
	int currentNode;
	int newCost;


public:

	int **costMatrix; //2-d array to represent the cost matrix.
	int *fatherAry;
	int *markedAry;
	int *bestCostAry;

	DijkstraSSS(int nodes)
	{
		numNodes = nodes;
		sourceNode = 1;
		minNode = 0;
		currentNode = -1; //Set this to -1 to allow loop on Step 4 to work correctly.
		newCost = 0;

		//Initialize 2-d and 1-d arrays
		costMatrix = new int*[numNodes];
		for (int i = 0; i < numNodes; i++)
		{
			costMatrix[i] = new int[numNodes];
			for (int j = 0; j < numNodes; j++)
			{
				costMatrix[i][j] = 99999;
				if (i == j)
				{
					costMatrix[i][j] = 0;
				}
			}

		}

		fatherAry = new int[numNodes];
		for (int i = 0; i < numNodes; i++)
		{
			fatherAry[i] = i;
		}

		markedAry = new int[numNodes]();

		bestCostAry = new int[numNodes]();
	}

	void setCurrentNode(int current)
	{
		currentNode = current;
	}

	void setSource(int source)
	{
		sourceNode = source;
	}
	int getCurrentNode()
	{
		return currentNode;
	}

	int getSource()
	{
		return sourceNode;
	}

	void setMinNode(int min)
	{
		minNode = min;
	}

	void loadCostMatrix(ifstream& fileIn)
	{
		int node1;
		int node2;
		int cost;

		while (fileIn >> node1)
		{
			fileIn >> node2;
			fileIn >> cost;
			costMatrix[node1 - 1][node2 - 1] = cost;
		}
	}

	void setBestCostAry()
	{
		for (int i = 0; i < numNodes; i++)
		{
			//Since the array is of size numNode x numNode, the node number is located at index of sourceNode-1.
			bestCostAry[i] = costMatrix[sourceNode - 1][i];
		}

	}

	void setFatherAry()
	{
		for (int i = 0; i < numNodes; i++)
		{
			fatherAry[i] = sourceNode-1;
		}
	}


	void setMarkedAry()
	{
		for(int i =0; i < numNodes; i++)
		{
			markedAry[i] = 0;
		}
		markedAry[sourceNode - 1] = 1; //Mark the source node.
	}

	void markMinNode(int node)
	{
		markedAry[node] = 1;
	}

	void debugPrint(ostream& outFile2)
	{
		outFile2 << "The source node is: " << sourceNode << endl;
		outFile2 << "FatherAry: ";
		for (int i = 0; i < numNodes; i++)
		{
			outFile2 << fatherAry[i]+1 << " ";
		}
		outFile2 << endl;

		outFile2 << "bestCostAry: ";
		for (int i = 0; i < numNodes; i++)
		{
			outFile2 << bestCostAry[i] << " ";
		}
		outFile2 << endl;

		outFile2 << "markedAry: ";
		for (int i = 0; i < numNodes; i++)
		{
			outFile2 << markedAry[i] << " ";
		}
		outFile2 << endl << endl;

	}

	int computeCost()
	{
		int temp = bestCostAry[minNode] + costMatrix[minNode][currentNode];
		return temp;
	}

	void changeFather()
	{
		fatherAry[currentNode] = minNode;
	}

	void changeCost()
	{
		bestCostAry[currentNode] = computeCost();
	}

	bool checkForUnmarked()
	{
		for (int i = 0; i < numNodes; i++)
		{
			if(markedAry[i] == 0)
			{ 
				return true;
			}
		}
		return false;
	}

	void printShortestPath(ostream& outFile1)
	{
		int father = fatherAry[currentNode];
		int prevFather;
		int parent = sourceNode-1;
		outFile1 << "Shortest path from " << sourceNode << " to " << currentNode+1 <<":" << endl;
		outFile1 << sourceNode << "-----"; 
		
		if(fatherAry[currentNode] != sourceNode-1) //If the father isn't the source node...
		{
			while(prevFather != currentNode)
			{
				while(father != parent)
				{
					prevFather = father;
					father = fatherAry[father];
				}
				outFile1 << "(" << costMatrix[father][prevFather] << ")" << "----->" << prevFather+1;
				parent = prevFather;
				father = currentNode;
				if(prevFather != currentNode)
				{
					outFile1 << "-----";
				}
			}
			
			outFile1 << endl;
		}
		else //Father is the source node.
		{
			outFile1 << "(" << costMatrix[sourceNode-1][currentNode] << ")" << "----->" << currentNode+1 << endl;
		}
		outFile1 << "Total cost: " << bestCostAry[currentNode] << endl;	
		outFile1 << endl;
	}
	
	~DijkstraSSS() //Custom Destructor
	{
		for(int i = 0; i < numNodes; i++)
		{
			delete costMatrix[i];
		}
		delete [] costMatrix;
		delete [] fatherAry;
		delete [] bestCostAry;
		delete [] markedAry;
	}
};

int main(int argc, char *argv[])
{
	if(argc < 4)
	{
		cout << "Incorrect number of input and output files loaded. Please check the command line arguments and try again!" << endl;
	}
	else
	{
		ifstream fileIn;
		ofstream outFile1;
		ofstream outFile2;
	
		fileIn.open(argv[1]);
		outFile1.open(argv[2]);
		outFile2.open(argv[3]);
	
		int numNodes;
		fileIn >> numNodes;
		DijkstraSSS graph(numNodes);
		outFile1 << "There are " << numNodes << " nodes in the input graph." << endl; 
		outFile1 << endl;
		graph.loadCostMatrix(fileIn);
		//Source node is set = 1 to in the class's parameterized constructor.
	
		while (graph.getSource() <= numNodes)
		{
			graph.setBestCostAry();
			graph.setFatherAry();
			graph.setMarkedAry();
	
			//Start step 3
			int currentMin; //Temporary variable that will be used to find the minimum node.
			int currentCost;
	
			while (graph.checkForUnmarked())
			{
				currentMin = -1;
				currentCost = 999999;
				graph.setCurrentNode(0); //Set the currentNode to start from the beginning of the array.
	
				for (int i = 0; i < numNodes; i++)
				{
					if (graph.markedAry[i] == 0 && graph.bestCostAry[i] <= currentCost) //Node is not marked.
					{
						currentMin = i;
						currentCost = graph.bestCostAry[i];
					}
				}
				graph.markMinNode(currentMin);
				graph.setMinNode(currentMin);
				graph.debugPrint(outFile2);
	
				//End step 3
				
				//Start step 4
				bool nodesUnmarked = true;
	
				while (nodesUnmarked)
				{	
					//Expand minNode
					for (int i = 0; i < numNodes; i++)
					{
						if (graph.markedAry[i] == 0 && i > graph.getCurrentNode()) //Node is not marked.
						{
							graph.setCurrentNode(i);
							break;
						}
					}
					if (graph.computeCost() < graph.bestCostAry[graph.getCurrentNode()])
					{
						graph.changeFather();
						graph.changeCost();
					}
	
					graph.debugPrint(outFile2);
	
					nodesUnmarked = false; //Assume we have reached the end of the array and have therefore checked all of the elements.
					
					for (int i = 0; i < numNodes; i++)	//Check if any unmarked nodes have not been checked.
					{
						if (graph.markedAry[i] == 0 && i > graph.getCurrentNode()) //Node is not marked.
						{
							nodesUnmarked = true; //There is an unmarked node that has yet to be accounted for.
							break;
						}
					} 	
					//End step 4
				}
			}
			graph.setCurrentNode(0); //Set the currentNode to start from the beginning of the array. 
			outFile1 << "Source Node = " << graph.getSource() << endl;
			outFile1<<endl;
			while (graph.getCurrentNode() < numNodes) //Use numNodes
			{
				graph.printShortestPath(outFile1);
				graph.setCurrentNode(graph.getCurrentNode() + 1);
			}
	
			graph.setSource(graph.getSource() + 1);
		}
		fileIn.close();
		outFile1.close();
		outFile2.close();
	}
	return 0;
}
