#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class listNode
{
	friend class linkedListStack;
	friend class linkedListQueue;
	friend class RadixSort;

	int data;
	listNode *next; //Pointer as a class member variable, must abide by the rule of three.

	listNode()
	{
		data = 0;
		next = NULL;
	}

	//Parameterized constructor that will allow us to instantiate the object with the given data.
	listNode(int d)
	{
		data = d;
		next = NULL;
	}
};

class linkedListStack
{
	friend class RadixSort;

	listNode *top; //Pointer as a class member variable, must abide by the rule of three.

	linkedListStack()
	{
		top = NULL; //Set the top of the stack to NULL in order to indicate that a new (empty) stack has been created.
	}
	
	void push(listNode *newNode) //Adds a node to the top of the stack.
	{
		newNode->next = top;
		top = newNode;
	}

	listNode* pop() //Removes the node located at the top of the stack.
	{
		if (!isEmpty())
		{
			listNode *temp = top;
			if (top->next == NULL) //You are removing the last value from the stack.
			{
				top=NULL;
				return temp;
			}
			else
			{
				top = top->next;
				temp->next = NULL; //Make the next pointer NULL to avoid returning all linked nodes.
				return temp;
			}
		}
		else
		{
			cout << "Unable to remove an item from the stack since it's empty. Please add items and try again!" << endl;
		}
	}

	bool isEmpty() //Returns true if the stack is empty or false if the stack is non-empty.
	{
		if (top == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
		
	//Custom Destructor
	~linkedListStack() //Delete every node in the stack.
	{
		if (!isEmpty())
		{
			listNode* temp;
			
			while (top != NULL)
			{			
    			temp = top->next;
    			delete top;
    			top = temp;
			}
		}
	}

	void printStack(ofstream& fileOut)
	{
		fileOut << "Top ->";

		listNode* temp = top;

		while (temp != NULL)
		{
			fileOut << "(" << temp->data << ", ";
			if (temp->next == NULL)
			{
				fileOut << "NULL";
			}
			else
			{
				fileOut << temp->next->data;
			}
			fileOut << ") ->";
			
			temp = temp->next;
		}
		fileOut << "NULL" << endl;
	}
};

class linkedListQueue
{
	friend class RadixSort;

	listNode* head; // Pointer as a class member variable, must abide by the rule of three.
	listNode* tail; // Pointer as a class member variable, must abide by the rule of three.

	linkedListQueue()
	{
		//Have the head and tail pointers point to the same dummy node.
		listNode* dummy = new listNode;
		head = dummy;
		tail = dummy;
	}

	void addTail(listNode* newNode)
	{
		tail->next = newNode;
		tail = newNode;
	}

	bool isEmpty()
	{
		if (head == tail)
		{
			//Only the dummy node remains.
			return true;
		}
		else
		{
			return false;
		}
	}

	listNode* deleteFront()
	{
		//The head is always equal to the dummy node. In order to delete the first node
		//representing actual data, we will have to delete head->next.
		if (!isEmpty())
		{
			listNode* temp = head->next;
			
			//First check to make sure there is more than one item in the queue.
			if (head->next == tail)
			{
				head->next = NULL;
				tail = head;
				//Removing the dummy node is handled by the custom destructor below.
			}
			else
			{
				head->next = head->next->next; //Set the next of dummy to point to the second item in the list.
			}
			temp->next = NULL; //Make the next pointer NULL to avoid returning more than one node.
			return temp;
		}

		else
		{
			cout << "Unable to remove from the queue as it is already empty!" << endl;
			return NULL;
		}
	}

	void printQueue(int index, ofstream fileOut)
	{
		fileOut << "Front(" << index << ")" << "->";
		listNode* temp = head->next; //Position temp at the first node containing data.

		while (temp != NULL)
		{
			fileOut << "(" << temp->data << ", ";
			if (temp->next == NULL)
			{
				fileOut << "NULL";
			}
			else
			{
				fileOut << temp->next->data;
			}
			fileOut << ") ->";
		}
		fileOut << "NULL" << endl;

		//Print the tail node.
		fileOut << "Tail(" << index << ")" << "->";
		fileOut << "(" << tail->data;
		if (tail->data == NULL)
		{
			fileOut << "NULL)";
		}
		else //This should never be reached, but will be included for debugging purposes.
		{
			fileOut << tail->next->data;
		}

		fileOut << ") -> NULL";
	}
		
	~linkedListQueue() //Delete every node in the queue.
	{
		if (head!= NULL)
		{
			listNode* temp;
			while (head != NULL)
			{	
    			temp = head->next;
    			delete head;
    			head = temp;
			}
		}
		tail = NULL;
	}
			

};

class RadixSort
{
	int tableSize;
	linkedListQueue** hashTable; //2-d array to be intitialized at runtime.
	linkedListStack* top;
	int data;
	int currentTable;
	int previousTable;
	int maxDigits;
	int offSet;
	int currentDigit;

public: 
	RadixSort()
	{
		tableSize = 10;

		hashTable = new linkedListQueue*[2];
		for (int i = 0; i < 2; i++)
		{
			hashTable[i] = new linkedListQueue[tableSize];
		}
		top = NULL;
		data = 0;
		currentTable = 0;
		previousTable = 0;
		maxDigits = 0;
		offSet = 0;
		currentDigit = 0;
	}

	void firstReading(ifstream& inFile)
	{
		int negativeNum = 0;
		int positiveNum = 0;
		int data;

		while (inFile >> data)
		{
			if (data < negativeNum)
			{
				negativeNum = data;
			}
			if (data > positiveNum)
			{
				positiveNum = data;
			}
		}
		
		offSet = abs(negativeNum);
		positiveNum += offSet;
		maxDigits = getMaxDigits(positiveNum);
	}
	
	void loadStack(ifstream& inFile, ofstream& outFile2)
	{
		top = new linkedListStack;

		while (inFile >> data)
		{
			data += offSet;
			listNode *newNode = new listNode(data);
			top->push(newNode);
		}

		outFile2 << "***Below is the constructed stack***" << endl;
		top->printStack(outFile2);
	}

	void radixSort(ofstream& fileOut2)
	{
		//Hashtable created inside of class constructor.
		currentDigit = 0;
		currentTable = 0;

		dumpStack(fileOut2); //Move the stack into hashTable[0]

		currentDigit++;
		currentTable=1;
		previousTable = 0;
		int currentQueue = 0;
		
		while (currentDigit <= maxDigits)
		{
			while (currentQueue <= tableSize - 1)
			{
				while (!hashTable[previousTable][currentQueue].isEmpty())
				{
					listNode* node = hashTable[previousTable][currentQueue].deleteFront(); //Extract a node from the currentQueue in the previousTable. 
					hashTable[currentTable][hashIndex(getOneDigit(node))].addTail(node); //Place node from previousTable into the current one.
				}
				currentQueue++;
			}
			
			printTable(fileOut2); //Print the current table to show the intermediate steps of the algorithm. 
			int temp = currentTable;
			currentTable = previousTable;
			previousTable = temp;
			currentQueue = 0;
			currentDigit++;
		}
		
	}
	
	void printSortedData(ofstream& fileOut)
	{
		fileOut << "The sorted data is as follows: ";
		listNode* temp = hashTable[previousTable][0].head->next; //Place the pointer at the head of the queue.
				while (temp -> next != NULL)
				{
					fileOut << temp->data-offSet << ", ";
					temp = temp->next;
				}
				fileOut << temp->data-offSet << endl;
	}
	
	//Custom Destructor
	~RadixSort() //Delete every node in each queue of both hash tables.
	{
		if(!top->isEmpty()) //Make sure that the stack is fully empty.
		{
			delete top; //Invoke the stack's custom destructor.
		}
		
		top = NULL;
		
		for(int i =0; i <tableSize; i++) //Delete all queues from the previous table.
		{
				hashTable[previousTable][i].~linkedListQueue();
		}
		
		for(int i =0; i <tableSize; i++) //Delete all queues from the current table.
		{
				hashTable[currentTable][i].~linkedListQueue();
		}
	}
private:

	int getMaxDigits(int num) //Returns the total digits in a given integer value.
	{
		string temp = to_string(num); //Convert the number to a string.
		return temp.length(); //Return the length of the string.
	}

	void dumpStack(ofstream& outFile2)
	{
		while (!top->isEmpty())
		{
			listNode* node = top->pop();
			hashTable[currentTable][hashIndex(getOneDigit(node))].addTail(node);
		}

		printTable(outFile2);
	}

	char getOneDigit(listNode* node)
	{
		string temp = to_string(node->data);
		int length = temp.length();
		
		if(length-currentDigit-1 < 0)
		{
			return '0';
		}
		else
		{
		return temp[temp.length()-currentDigit-1];
		}
	}

	void printTable(ofstream& fileOut) //Prints the current table to the output file.
	{
		fileOut << endl;
		fileOut << "Printing Hash Table #" << currentTable << endl;
		for (int i = 0; i < tableSize; i++)
		{
			if (!hashTable[currentTable][i].isEmpty())
			{
				fileOut << "Table[" << currentTable << "][" << i << "]: ";
				listNode* temp = hashTable[currentTable][i].head->next; //Place the pointer at the head of the queue.
				while (temp -> next != NULL)
				{
					fileOut << temp->data << ", ";
					temp = temp->next;
				}
				fileOut << temp->data << endl;
			}
		}
		
	}

	int hashIndex(char singleDigit) //This function takes a single character and returns it as an integer variable.
	{
		int i = singleDigit-'0';
		return i;
	}
		
};

int main(int argc, char** argv)
{
	if (argc > 3)
	{

		ifstream fileIn;
		fileIn.open(argv[1]);
		ofstream outFile1;
		outFile1.open(argv[2]);
		ofstream outFile2;
		outFile2.open(argv[3]);

		RadixSort sortData;
		sortData.firstReading(fileIn);
		fileIn.close();
		fileIn.open(argv[1]);
		sortData.loadStack(fileIn, outFile2);
		sortData.radixSort(outFile2);
		sortData.printSortedData(outFile1);
		fileIn.close();
		outFile1.close();
		outFile2.close();

	}
	else
	{
		cout << "Insufficient number of input/output files loaded. Please try again!" << endl;
	}
	
	return 0;
}
