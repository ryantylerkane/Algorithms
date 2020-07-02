#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

using namespace std;

class listBinTreeNode
{
	//Private Class Member Variables
	string chStr;
	int prob;
	string code;
	listBinTreeNode* next;
	listBinTreeNode* left;
	listBinTreeNode* right;

	/*Since there class member variables that are pointers,
	we must abide by the rule of three (copy constructor,
	overloaded assignment operator and custom destructor).*/

public:

	//CLASS CONSTRUCTORS	
	listBinTreeNode()
	{
		chStr = "";
		prob = 0;
		code = "";
		next = NULL;
		left = NULL;
		right = NULL;
	}


	//Parameterized constructor with chStr and prob (to be used when adding nodes to the linked list)

	listBinTreeNode(string ch, int probability)
	{
		chStr = ch;
		prob = probability;
		code = "";
		next = NULL;
		left = NULL;
		right = NULL;
	}

	//END CLASS CONSTRUCTORS

	//SETTERS AND GETTERS
	void setChStr(string ch)
	{
		chStr = ch;
	}

	void setProb(int probability)
	{
		prob = probability;
	}
	void setCode(string codeStr)
	{
		code = codeStr;
	}
	void setNext(listBinTreeNode* nxt)
	{
		next = nxt;
	}
	void setLeft(listBinTreeNode* l)
	{
		left = l;
	}
	void setRight(listBinTreeNode* r)
	{
		right = r;
	}
	string getChStr()
	{
		return chStr;
	}
	int getProb()
	{
		return prob;
	}
	string getCode()
	{
		return code;
	}
	listBinTreeNode* getNext()
	{
		return next;
	}
	listBinTreeNode* getLeft()
	{
		return left;
	}
	listBinTreeNode* getRight()
	{
		return right;
	}
	//END SETTERS AND GETTERS


	void printNode(ofstream& fileOut)
	{
		fileOut << "(";
		fileOut << getChStr() << ", ";
		fileOut << getProb() << ", ";
		if(getCode() == "")
		{
			fileOut << "\"\", ";
		}
		else
		{
			fileOut << getCode() << ", ";
		}
		if (getNext() == NULL)
		{
			fileOut << "NULL" << ", ";
		}
		else
		{
			fileOut << getNext()->getChStr() << ", ";
		}
		if (getLeft() == NULL)
		{
			fileOut << "NULL" << ", ";
		}
		else
		{
			fileOut << getLeft()->getChStr() << ", ";
		}
		if (getRight() == NULL)
		{
			fileOut << "NULL";
		}
		else
		{
			fileOut << getRight()->getChStr();
		}
		fileOut << ")" <<endl;
	}
	//----------------------------------------------------------------	
	//Rule of three (copy constructor, overloaded assignment operator, and custom destructor).	

	//Copy Constructor
	listBinTreeNode(const listBinTreeNode& otherNode)
	{
		chStr = otherNode.chStr;
		prob = otherNode.prob;
		code = otherNode.code;

		//Check if each of the otherNode pointers are null, if they are, set the corresponding pointers to null.
		//If they aren't, set the data of otherNode pointer equal to the data of the corresponding pointer.
		if (otherNode.left == NULL)
		{
			left = NULL;
		}
		else
		{
			left = new listBinTreeNode;
			*left = *otherNode.left;

		}
		if (otherNode.right == NULL)
		{
			right = NULL;
		}
		else
		{
			right = new listBinTreeNode;
			*right = *otherNode.right;
		}
		if (otherNode.next == NULL)
		{
			next = NULL;
		}
		else
		{
			next = new listBinTreeNode;
			*next = *otherNode.next;
		}
	}
	//Overloaded Assignment Operator
	const listBinTreeNode& operator= (const listBinTreeNode& otherNode)
	{
		if (this != &otherNode) //Checks for self-assignment.
		{
			//Make sure the object being assigned has all NULL pointers.
			if (next != NULL)
			{
				delete next;
			}

			if (left != NULL)
			{
				delete left;
			}

			if (right != NULL)
			{
				delete right;
			}

		//Once we know the object being copied to is null, we can simply call the copy constructor.
			listBinTreeNode(otherNode);

		}
		else
		{
			cout << "Assignment not performed as this would result in self-assignment!" << endl;
		}

		return *this;
	}

	//Custom Destructor
	~listBinTreeNode()
	{
		//This will be invoked when HuffmanLLTree is deallocated at program termiantion. Since the list linked
		//list is deallocated in the HuffmanLLTree destructor, we cannot get rid of the next pointer without
		//destroying the integrity of the tree/linked list.
		delete left;
		delete right;
	}

}; //END listBinTreeNode

class HuffmanLListTree
{
	listBinTreeNode* listHead;
	listBinTreeNode* root;

public:

	//CONSTRUCTOR
	HuffmanLListTree()
	{
		listHead = new listBinTreeNode("dummy", 0); //Assign a dummy node to the front of the linked list.
		root = NULL; //The root of the binary tree will remain null until the list is constructed.
	}

	listBinTreeNode* getRoot()
	{
		return root;
	}

	bool isEmpty()
	{
		//The list will be considered empty if there are no other nodes except for the dummy node in the list.
		//The best way to do this is to check listHead, since the root of the tree holding any value is dependent
		//On a linked list being formed.
		if (listHead->getNext() == NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//Rule of three (copy constructor, overloaded assignment operator, and custom destructor

	//Copy constructor
	HuffmanLListTree(const HuffmanLListTree& otherTree)
	{
		if (!isEmpty()) //Assure that the list is empty and initialized.
		{
			root = NULL; //Since root is a node in the linked list, we can make it NULL without worrying about losing any data that is connected via pointers.
			
		listBinTreeNode* temp;
			
			while (listHead->getNext() != NULL)
			{	
    			temp = listHead->getNext();
    			delete listHead;
    			listHead = temp;
			}
			//ListHead and dummy node are the only thing that remains.
		}

		if (otherTree.root==NULL) //If the list to be copied is empty, simply initialize the other list.
		{
			listHead = NULL;
			root = NULL;
		}

		else //The list to be copied has at least one node.
		{
			listBinTreeNode *current = listHead;
			listBinTreeNode *otherCurrent = otherTree.listHead;

			while (otherCurrent->getNext() != NULL) //Traverse until you reach the last node.
			{
				listBinTreeNode *newNode = new listBinTreeNode;
				*newNode = *otherCurrent->getNext();
				current->setNext(newNode);
				current = current->getNext();
				otherCurrent = otherCurrent->getNext();
			}
			//Add the last node to the new linked list.
			listBinTreeNode *newNode = new listBinTreeNode;
			*newNode = *otherCurrent->getNext();
			current->setNext(newNode);
		}
	}

	//Overloaded Assignment Operator
	const HuffmanLListTree& operator= (const HuffmanLListTree& otherTree)
	{
		if (this != &otherTree) //Checks for self-assignment.
		{
			//Call the copy constructor. Note that the constructor will check if the left operand is set to null before
			//Adding a new information.
			HuffmanLListTree(otherTree);

		}
		else
		{
			 cout << "Assignment not performed as this would result in self-assignment!" << endl;
		}

		return *this;
	}

	//Custom Destructor
	~HuffmanLListTree() //Delete every node in the linked list including the dummy node.
	{
		if (!isEmpty())
		{
			listBinTreeNode* temp;
			
			while (listHead != NULL)
			{			
    			temp = listHead->getNext();
    			delete listHead;
    			listHead = temp;
			}
		}
	}

	void constructLinkedList(ifstream& fileIn, ofstream& fileOut1)
	{
		//listHead is already pointing to a dummy node after constructor being executed.
		string chStr;
		int prob;
		listBinTreeNode* spot;
		listBinTreeNode* newNode = NULL;

		while (fileIn >> chStr)
		{
			fileIn >> prob;
			spot = findSpot(prob); //Find the location in which the new node will be entered.
			newNode = new listBinTreeNode(chStr, prob); //Make a new node with the chStr and Prob values extracted from the input file.
			insertNewNode(spot, newNode);
			printList(fileOut1);
		}
		fileIn.close();

	}

	//This function finds the location in which a node with the provided probability is expected to be entered and returns 
	//the position as a pointer variable.
	listBinTreeNode* findSpot(int probability)
	{
		listBinTreeNode* spot = listHead;

		while (spot->getNext() != NULL && spot->getNext()->getProb() < probability)
		{
			spot = spot->getNext();
		}


		return spot;
	}

	//Inserts a new node at the location indicated by the spot pointer.
	void insertNewNode(listBinTreeNode* spot, listBinTreeNode* newNode)
	{
		if (spot != NULL) //Check if there is a valid spot to add the newNode to the list.
		{
			newNode->setNext(spot->getNext()); //Set the next of spot to be the next of the new node,
			spot->setNext(newNode); //Insert the newNode in the list by having spot's next pointer point to it.
		}
		else //The spot returned by the findSpot function was invalid.
		{
			throw invalid_argument("The value that is expected to be added to the list is invalid!");
		}
	}

	//Function will build a Huffman Binary Tree from an ordered linked list organized in probability ascending order.

	void constructBinTree(ofstream& fileOut)
	{
		listBinTreeNode *newNode;
		listBinTreeNode *spot;

		//Continue to iterate until the dummy node and one other node remains.

		while (listHead->getNext()->getNext() != NULL){
			newNode = new listBinTreeNode();
			//Take the sum of the probabilities of the first and second nodes and load the value into the new node.
			newNode->setProb(listHead->getNext()->getProb() + listHead->getNext()->getNext()->getProb());
			//Concatenate the code string from the first and second nodes and load it into the new node.
			newNode->setChStr(listHead->getNext()->getChStr() + listHead->getNext()->getNext()->getChStr());
			//Set the left pointer of the newNode to the first node in the linked list and the right pointer to the second node in the list.
			newNode->setLeft(listHead->getNext());
			newNode->setRight(listHead->getNext()->getNext());
			newNode->printNode(fileOut); //Print the tree node that was recently constructed.
			spot = findSpot(newNode->getProb()); //Find the location in which to enter the new binTreeNode.
			insertNewNode(spot, newNode); //Insert the newly-created node.
			listHead->setNext(listHead->getNext()->getNext()->getNext()); //Move the listHead two elements ahead.
			printList(fileOut);
		}

		root = listHead->getNext(); //Only one node and the dummy node remain.

	}

	void constructCode(listBinTreeNode *root, string code, ofstream& fileOut)
	{
		if (root == NULL) //The tree is empty.
		{
			throw runtime_error("This is an empty tree. Terminating program!");
		}
		else if (isLeaf(root)) //The node is a leaf.
		{
			root->setCode(code);
			fileOut << "(" << root->getChStr() << ", " << root->getCode() <<")" << endl;
		}
		else //The node is an internal node.
		{
			//Make a recursive call to constructCode to both the right and left child.
			constructCode(root->getLeft(), code + "0", fileOut);
			constructCode(root->getRight(), code + "1", fileOut);
		}
	}

	void preOrder(listBinTreeNode* root, ofstream& fileOut)
	{
		if (root == NULL)
		{
			//We have reached the bottom of the tree.
		}
		else
		{
			root->printNode(fileOut); // output to outFile4 in the format given
			preOrder(root->getLeft(), fileOut);
			preOrder(root->getRight(), fileOut);
		}
	}

	void inOrder(listBinTreeNode* root, ofstream& fileOut)
	{
		if (root == NULL)
		{
			//We have reached the bottom of the tree.
		}
		else
		{
			inOrder(root->getLeft(), fileOut);
			root->printNode(fileOut); // output to outFile4 in the format given
			inOrder(root->getRight(), fileOut);
		}
	}

	void postOrder(listBinTreeNode* root, ofstream& fileOut)
	{
		if (root == NULL)
		{
			//We have reached the bottom of the tree.
		}
		else
		{
			postOrder(root->getLeft(), fileOut);
			postOrder(root->getRight(), fileOut);
			root->printNode(fileOut); // output to outFile4 in the format given
		}
	}


	//The following function will determine if a node in a Hoffman Binary tree is leaf node or not.
	bool isLeaf(listBinTreeNode* node)
	{
		if (node->getLeft() == NULL && node->getRight() == NULL) //If the node has no children, then it is a leaf.
		{
			return true;
		}
		else
		{
			return false;
		}
	}



	//Prints the symbol, probability, and next symbol for each node in the linked list.
	void printList(ofstream &fileOut)
	{
		listBinTreeNode* temp = listHead;

		fileOut << "listHead -->";
		while (temp->getNext() != NULL) //Traverse the list until you reach the last node.
		{
			fileOut << "(" << temp->getChStr() << ", " << temp->getProb() << ", ";
			if (temp->getNext() == NULL)
			{
				fileOut << "NULL)";
			}
			else
			{
				fileOut << temp->getNext()->getChStr();
				fileOut << ")";
			}
			  fileOut << "-->";
			temp = temp->getNext();
		}

		//Output the last node and end the line.
		fileOut << "(" << temp->getChStr() << ", " << temp->getProb() << ", ";
		if (temp->getNext() == NULL)
		{
			fileOut << "NULL";
		}
		else
		{
			fileOut << temp->getNext()->getChStr();
		} 
		fileOut << ")-->NULL" << endl;
	}

};

int main(int argc, char** argv) {

	ifstream inFile;
	ofstream fileOut;

	HuffmanLListTree hList;

	if (argc > 1) //Check if a file was loaded into argv[1]
	{
		inFile.open(argv[1]);
			if (argc > 5)
			{
				fileOut.open(argv[2]);
				hList.constructLinkedList(inFile, fileOut);
				fileOut.close();
				fileOut.open(argv[3]);
				hList.constructBinTree(fileOut);
				fileOut.close();
				fileOut.open(argv[4]);
				hList.constructCode(hList.getRoot(), "", fileOut);
				fileOut.close();
				fileOut.open(argv[5]);
				fileOut << "Pre-order: " << endl;
				hList.preOrder(hList.getRoot(), fileOut);
				fileOut << "----------------------------" << endl;
				fileOut << "In-order: " << endl;
				hList.inOrder(hList.getRoot(), fileOut);
				fileOut << "-----------------------------" << endl;
				fileOut << "Post-order: " << endl;
				hList.postOrder(hList.getRoot(), fileOut);
				fileOut.close();
			}
			else
			{
				cout << "There is an insufficient number of files given as parameters!" << endl;
				return 1;
			}
		}
	else //File was not loaded into argv[1]
	{
		cout << "There was no input file path loaded into argv[1]. Please provide a valid path and try again!" << endl;
		return 1;
	}
		return 0;
}
