#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

struct xyCoord
{
	int xCoord;
	int yCoord;
};

//Start Point class definition
class Point
{
	//Private class variables
	int xCoord;
	int yCoord;
	int label;
	//---------------------
	//Constructors	

public:
	Point()
	{
		xCoord = 0;
		yCoord = 0;
		label = 1;
	}

	Point(int x, int y) //This contstructor will be used to instantiate objects in the ptList array prior to the labels being assigned
	{
		xCoord = x;
		yCoord = y;
		label = 1;
	}

	Point(int x, int y, int l)
	{
		xCoord = x;
		yCoord = y;
		label = l;
	}
	//----------------------------------
	//Setters and Getters	

	void setXCoord(int x)
	{
		xCoord = x;
	}
	void setYCoord(int y)
	{
		yCoord = y;
	}
	void setLabel(int l)
	{
		label = l;
	}

	int getXCoord()
	{
		return xCoord;
	}
	int getYCoord()
	{
		return yCoord;
	}
	int getLabel()
	{
		return label;
	}

};
//End Point class definition


//Start Kmean class definition
class Kmean
{
	friend class Image; //Make Image a friend of this class so that it is has access to private/protected variables.

	//Private member variables
	int kCluster;
	int numPts;
	Point* ptList; //To hold an array of 2d points the size of numPts
	int changeFlag;
	xyCoord* kCentroids; //To hold an array of 2d points the size of kCluster + 1

	/*Note that both ptList and kCentroids are pointers that serve as class member variables
	According to the rule of three, this means that this class must contain a custom destructor,
	a copy constructor and an overloaded assignment operator in order to properly manage dynamic memory and
	create deep copies of the array.*/
	//-------------------------------------------------------------------------------

	//Constructors

public:
	Kmean()
	{
		kCluster = 2; //If no value is given, assume there are two clusters.
		numPts = 100; //If no value is given, allocate 100 points to attempt to accomodate any listed points.
		ptList = new Point[numPts];
		changeFlag = 1; //Set changeFlag to one in order to access loop in main function.
		kCentroids = new xyCoord[kCluster + 1]();

	}
	Kmean(int cluster, int pts)
	{
		kCluster = cluster;
		numPts = pts;
		ptList = new Point[numPts];
		changeFlag = 1; //Set changeFlag to one in order to access loop in main function.
		kCentroids = new xyCoord[kCluster + 1]();
	}

	//------------------------------------------------
	//Public functions

	//Setters and Getters
	void setkCluster(int cluster)
	{
		kCluster = cluster;
	}
	void setNumPts(int pts)
	{
		numPts = pts;
	}
	void setChangeFlag(int flag)
	{
		changeFlag = flag;
	}
	int getKCluster()
	{
		return kCluster;
	}
	int getNumPts()
	{
		return numPts;
	}
	int getChangeFlag()
	{
		return changeFlag;
	}

	//Class Member Functions
	/*The following function will read the input file and concatenate the pixel
	row and column values into a Point object and place them in the ptList array*/
	void loadPtList(ifstream &inFile)
	{
		int x = 0;
		int y = 0;
		
		for (int i = 0; i < numPts; i++)
		{
			inFile >> x;
			inFile >> y;
			ptList[i] = Point(x, y);
		}
	}

	/*The following function will sequentially assign a label ranging from
	1 to kCluster to each point in the ptList array.*/
	void assignLabel()
	{
		int currentLabel = 1;

		for (int i = 0; i < numPts; i++)
		{
			if (currentLabel > kCluster){ //Check if the label value is greater than the value for kCluster
				currentLabel = 1; //If the value has exceeded kCluster, make it one again.
			}
			ptList[i].setLabel(currentLabel); //Assign the label value to the point.
			currentLabel++;
		}
	}

	/*The following function will calculate the centroid for each labeled
	group of points*/
	void computeCentroid()
	{
		//Declare three dynamic arrays and initialize their values to zero
		//For the sum of the x coordinates, y coordinates, and the number of points per label.
		int* sumCnt = new int[kCluster + 1]();
		int* sumX = new int[kCluster + 1]();
		int* sumY = new int[kCluster + 1]();
		int ptIndex = 0;

		/*Use a while loop to find the sum of the x and y coordinates for all
		of the points in each label group.*/
		while (ptIndex < numPts)
		{
			int label = ptList[ptIndex].getLabel();
			sumX[label] += ptList[ptIndex].getXCoord();
			sumY[label] += ptList[ptIndex].getYCoord();
			sumCnt[label]++;
			ptIndex++;
		}
		
		int clusterIndex = 1;
		
		//Use a while loop to calculate the x and y indices of each centroid.
		while (clusterIndex <= kCluster)
		{
			kCentroids[clusterIndex].xCoord = sumX[clusterIndex] / sumCnt[clusterIndex];
			kCentroids[clusterIndex].yCoord = sumY[clusterIndex] / sumCnt[clusterIndex];
			clusterIndex++;
		}
		
	/*Since we are using integers to compute the centroid value instead of doubles, it is entirely possible to have two (or more) label groups that are at the exact same coordinates.
	This is the case with labels 3 and 4 in the test file. This will lead to one of the groups becoming entirely empty due to how the labels are changed in the clusterCheck
	alorithm. As a consequence, a division by zero error will be thrown on the following iteration of computeCentroid. To avoid this issue, we must check if any of the 
	computed centroids have the same coordinates and then make an adjustment to one of those coordiantes to assure the algorithm will keep running. The degree in which we
	increase and decrease the x and y axises will indeed affect the efficiency of the algorithm, but will not prevent it from obtaining the correct answer given the change in minimal.*/
	
	checkCentroids();
		
			
		//Delete the dynamic arrays to avoid memory leaks.
		delete[] sumCnt;
		delete[] sumX;
		delete[] sumY;
	}

	//This function will check all of the centroids to assure that no duplicate points are used. An explanation as to why this is being used can
	//be seen in the comment block found in the computeCentroid() function above.
	void checkCentroids()
	{
		bool changes = false;
		
		//Use a nested for loop to compare each of the centroid points
		for (int i = 1; i <= kCluster; i++)
		{
			for(int j = i+1; j <= kCluster; j++)
			{
				if(kCentroids[i].xCoord == kCentroids[j].xCoord && kCentroids[i].yCoord == kCentroids[j].yCoord)
				{
					//If two points are the same, adjust one of their x coordinates.
					kCentroids[j].xCoord = kCentroids[j].xCoord+1;	
					changes = true;
				}	
			}	
		}
		
		if(changes==true)
		{
			checkCentroids(); //Check to make sure that the changes did not create a new set of duplicates (or make more changes if there were >2 duplicates)
		}
	}
	/*This function will compare the distance between each point and all calculated
	centroids. If the centroid that is closest to a given point is not the centroid of
	its label group, then the points label is switched to the group whose centroid is closest.*/
	void clusterCheck()
	{
		double* dist = new double[kCluster + 1]();
		int ptIndex = 0;
		
		while (ptIndex < numPts)
		{
			int label = ptList[ptIndex].getLabel();
			double minDist = 99999.0;
			int clusterIndex = 1;
			int minLabel = label;
			
			//Use a while loop to check a single point's distance from each centroid.
			while (clusterIndex <= kCluster)
			{
				//Compute the distance between the point and each centroid.
				dist[clusterIndex] = computeDistance(ptList[ptIndex], Point(kCentroids[clusterIndex].xCoord, kCentroids[clusterIndex].yCoord, clusterIndex));
				if (dist[clusterIndex] < minDist)
				{
					//If a newly-calcualted distance is closer than the current minDistance,
					//update the variable.
					minDist = dist[clusterIndex];
					minLabel = clusterIndex;
				}
				clusterIndex++;
			}
			
			//Check if the label of the point matches the label of the closest centroid.
			if (ptList[ptIndex].getLabel() != minLabel)
			{
				ptList[ptIndex].setLabel(minLabel); //Set the point's label to the label of the nearest centroid.
				changeFlag++; //Increase changeFlag to record the change. 
			}

			ptIndex++;
		}
		
		delete [] dist; //Delete the distance array in order to free up the allocated dynamic memory.
	}

	//This function will calculate and return the distance between two points.
	double computeDistance(Point p1, Point p2)
	{
		return sqrt(pow((p1.getXCoord() - p2.getXCoord()), 2.0) + pow((p1.getYCoord() - p2.getYCoord()), 2.0));
	}

	//This function will print each point (and their label) in the ptList array.
	void printPtList(ofstream& fileOut)
	{
		fileOut << left << setw(15) << "Point";
		fileOut << fixed << setw(5) << "Label" << endl;
		 
		for (int i = 0; i <numPts; i++)
		{
			fileOut << "(";
			fileOut <<left << setw(2) << ptList[i].getXCoord();
			fileOut << ",";
			fileOut << left<< setw(2) << ptList[i].getYCoord(); 
			fileOut << ")";
			fileOut <<right << setw(13) << ptList[i].getLabel() << endl;
		}
		
		fileOut << "--------------------------------------" << endl;
	}
	//End class member functions
	//-----------------------------
	//Rule of three (copy constructor, overloaded assignment operator, and custom destructor).	

	//Copy Constructor
	Kmean(const Kmean& otherKmean)
	{
		changeFlag = otherKmean.changeFlag;
		kCluster = otherKmean.kCluster;
		numPts = otherKmean.numPts;
		
		ptList = new Point[numPts];
		for(int i =0; i < numPts; i++)
		{
			ptList[i] = otherKmean.ptList[i];
		}
		
		kCentroids= new xyCoord[kCluster+1];
		
		for(int i=1; i<=kCluster; i++)
		{
			kCentroids[i] = otherKmean.kCentroids[i];
		}
	}
	
	//Overloaded Assignment Operator
	const Kmean& operator= (const Kmean& otherKmean)
	{
		if (this != &otherKmean) //Checks for self-assignment.
	{
		
		if (ptList != NULL) //Check if the ptList that is going to be copied to is empty.
		{
			delete[] ptList; //list is now null.
		}
		
		if(kCentroids != NULL)
		{
			delete [] kCentroids;
		}
		
		changeFlag = otherKmean.changeFlag;
		kCluster = otherKmean.kCluster;
		numPts = otherKmean.numPts;
		ptList = new Point[numPts];
		//If the list is empty, the following if statement will not execute.
		
		if (otherKmean.ptList != NULL) //If the other lists have elements, copy them in.
		{
			for(int i =0; i < numPts; i++)
			{
				ptList[i] = otherKmean.ptList[i];
			}
		
		}
		
		kCentroids= new xyCoord[kCluster+1];
		
		if(otherKmean.kCentroids!= NULL)
		{
			for(int i=1; i<=kCluster; i++)
			{
				kCentroids[i] = otherKmean.kCentroids[i];
			}
		}
	}
	else
	{
		cout << "Assignment not performed as this would result in self-assignment!" << endl;
	}

	return *this;
	}
	
	//Custom Destructor
	~Kmean()
	{
		//Deallocate both of the dynamic arrays.
		delete [] ptList;
		delete [] kCentroids;
	}
	
}; //End Kmean class definition

class Image //This is a friend class to Kmeans
{
	int numRows;
	int numCols;
	int** imgAry; //Will be used o hold a dynamically allocated 2d array representing an image.

	/*Note that imgAry is a pointer that serves as class member variable.
	According to the rule of three, this means that this class must contain a custom destructor,
	a copy constructor and an overloaded assignment operator in order to properly manage dynamic memory and
	create deep copies of the array.*/
	//Start public Member Functions
public:
	//Constructors
	Image()
	{
		//If not values are given, allocate a 100 x 100 array by default.
		numRows = 100;
		numCols = 100;
		imgAry = new int*[numRows]();
		for (int i; i < numRows; i++)
		{
			imgAry[i] = new int[numCols];
		}
	}

	Image(int rows, int columns)
	{
		numRows = rows;
		numCols = columns;
		imgAry = new int*[numRows];
		for (int i; i < numRows; i++)
		{
			imgAry[i] = new int[numCols]();
		}
	}
	//------------------------------------------

	//Setters and getters

	int getNumRows()
	{
		return numRows;
	}

	int getNumCols()
	{
		return numCols;
	}

	void setNumRows(int rows)
	{
		numRows = rows;
	}

	void setNumCols(int cols)
	{
		numCols = cols;
	}


	//This function will traverse the ptList array and place the label value of each point 
	//in its corresponding location in the imgAry.
	void mapPointToImage(Kmean*& km)
	{
		for (int i=0; i < km->numPts; i++)
		{
			imgAry[km->ptList[i].getXCoord()][km->ptList[i].getYCoord()] = km->ptList[i].getLabel();
		}
	}

	//This function will traverse the 2d array and check if its label is greater than 0.
	//If the label is greater than 0, then it is printed. If it is less than or equal to
	//zero a blank space is outputted.
	void prettyPrintImgAry(ofstream& fileOut)
	{
		for (int i=0; i < numRows; i++)
		{
			for (int j=0; j< numCols; j++)
			{
				if (imgAry[i][j] > 0)
				{
					fileOut << imgAry[i][j];
				}
				else
				{
					fileOut << " ";
				}
			}

			fileOut << endl; //End the row of output and move to the next line.
		}
		
		fileOut << "----------------------------------------------------------------------" << endl;
	}
	
	//END CLASS MEMBER FUNCTIONS
	//----------------------------------------------
	//Rule of three (copy constructor, overloaded assignment operator, and custom destructor).
	
	//Custom Destructor
	~Image()
	{
		for (int i = 0; i < numRows; i++)
		{
			delete [] imgAry[i];
		}
		
		delete [] imgAry;
	}
	
	//Copy Constructor
	Image(const Image& otherImage)
	{
		numRows = otherImage.numRows;
		numCols = otherImage.numCols;
		
		imgAry = new int*[numRows]();
		for (int i = 0; i < numRows; i++)
		{
			imgAry[i] = new int[numCols]();
		}
		
		for(int i = 0; i <numRows; i++)
		{
			for(int j = 0; j < numCols; j++)
			{
				imgAry[i][j]= otherImage.imgAry[i][j];
			}
		}
	}
		
	//Overloaded Assignment Operator
	const Image& operator= (const Image& otherImage)
	{
		if (this != &otherImage) //Checks for self-assignment.
		{
			if(imgAry != NULL)
			{
				for (int i = 0; i < numRows; i++)
				{
					delete [] imgAry[i];
				}
		
				delete [] imgAry;
			}
			
			numRows = otherImage.numRows;
			numCols = otherImage.numCols;
		
			imgAry = new int*[numRows]();
			for (int i = 0; i < numRows; i++)
			{
				imgAry[i] = new int[numCols]();
			}
			
			for(int i = 0; i <numRows; i++)
			{
				for(int j = 0; j < numCols; j++)
				{
					imgAry[i][j]= otherImage.imgAry[i][j];
				}
			}
		}
		else
		{
			cout << "Assignment not performed as this would result in self-assignment!" << endl;
		}

		return *this;
	}
	
}; //END OF IMAGE CLASS

int main(int argc, char *argv[])
{
	Kmean* points;
	Image* image;
	ifstream inFile;
	ofstream fileOut;
	
	if (argc > 1) //Check if a file was loaded into argv[1]
	{
		inFile.open(argv[1]);
		if (inFile.is_open()) //Check if the file was successfully opened.
		{
			//Read the text file to extract the number of rows, number of columns, and number of points.
			int temp = 0; //Use a temporary value to store the value from the input file before storing them into the constants.
			inFile >> temp;
			const int NUM_ROWS = temp;
			inFile >> temp;
			const int NUM_COLUMNS = temp;
			inFile >> temp;
			const int NUM_POINTS = temp;

			//Ask the user for the number of clusters
			int kCluster;
			cout << "What is the K value? ";
			cin >> kCluster;

			points = new Kmean(kCluster, NUM_POINTS);
			image = new Image(NUM_ROWS, NUM_COLUMNS);

			points -> loadPtList(inFile); //Load the points into the ptList array.
			points->assignLabel(); //Assign labels to each of the points in the ptList array.
		}
			else //Error occured while opening the input file.
		{
			cout << "Error opening the input file: " << argv[1] << "." << endl;
			return 1;
		}

	}
	else //File was not loaded into argv[1]
	{
		cout << "There was no file path loaded into argv[1]. Please provide a valid path and try again!" << endl;
		return 1;
	}
		if(argc > 2) //Check if there is a file in argv[2] that will receive the output.
		{
				fileOut.open(argv[2]);
				
				if(fileOut.is_open()) //Check if the output file was correctly opened.
				{
					points->printPtList(fileOut); //Print the list of points (and their labels) to the output file.
			
					/*With all of the points loaded, we will now repeatedly compute the centroid of each label group and then map the distance between
					each point and the calculated centroids. Each point will then be checked to assure that the centroid for their group is the centroid
					that is the closest in distance. If this is not the case, the point's label will be changed to reflect its nearest centroid. Once
					this process can be completed with no changes having to be made (changeFlag ==0) then the loop will exit.*/
					while (points->getChangeFlag() > 0)
					{
						image->mapPointToImage(points); //Create a 2D array representing the points currently stored in ptList.
						image->prettyPrintImgAry(fileOut); //Print the 2D array representing the image to the output file.
						points->setChangeFlag(0); //Set the changeFlag to zero prior to making any changes to the point labels.
						points->computeCentroid(); //Compute the centroids for the given points.
						points->clusterCheck(); //Check if the points are approrpriately labeled to represent the clusters.
					}
				}
				
				else
				{
					cout << "Error opening the output file: " << argv[2] << "." << endl;
					return 1;
				}
			}
			
			else //File path was not placed in argv[2]
			{
				cout << "There was no file path loaded into argv[2]. Please provide a valid path and try again!" << endl;
				return 1;	
			}
			
	delete points;
	delete image;
	inFile.close();
	fileOut.close();
	return 0;
}
