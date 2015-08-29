//Brittany Harrington
//Fundamentals of Computing II
//March 5, 2015
//Lab 6-- Sodoku Solver
//This is the .h file for the Puzzle class. This class is utilized in the solver.cpp program to solve a Sudoku puzzle. Contains functions that update a 3D possibilities vector, a 2D board vector, and utilize the singleton algorithm.

#ifndef PUZZLE_H
#define PUZZLE_H
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

//Puzzle class definition
class Puzzle	{
	public:
		Puzzle(int puzzleSize=9);	//default constructor
		int getSize();	
		int isEmpty(int i, int j);
		void printPuzzle();
		void checkColumn(int col);
		void checkRow(int row);
		void checkSquare(int row, int col);
		int boardFull();
		void play();
		void singletonRow(int row);
		void singletonCol(int col);
	private:
		int size;
		vector<vector <int> > myPuzzle;	//pointer to first element of pointer based array
		vector<vector<vector <int> > > myPossibilities;	//3D vector that stores possible values at each spot
};	//end class Puzzle

Puzzle:: Puzzle(int puzzleSize)	{
	size= (puzzleSize>0 ? puzzleSize : 9);	//validate puzzleSize

	string fileName;
	cout << "Please enter a file name: ";
	cin >> fileName;

	ifstream inFile;
	inFile.open(fileName.c_str());

	vector<int> tempVec;
	int tempVar;	//not sure about this line??

	while(!inFile.eof()){ //While the file is not at the end  
        	for(int i = 0 ; i < size ; i++){    //Read in a line into a temporary vector
                	  inFile >> tempVar; //Put just read value into temporary vector
                  	tempVec.push_back(tempVar); 
          	}
       	   	myPuzzle.push_back(tempVec);   //Push back the line just read)
          	tempVec.clear(); //Clear/recycle temporary vector
	}

	//intialize all possibilities to 1 for empty board spaces
	int value=1;
	for (int i=0; i<size; i++)	{
		vector < vector < int > > temp1;
		myPossibilities.push_back(temp1);
		for (int j=0; j<size; j++)	{
			vector < int > temp2;
			myPossibilities[i].push_back(temp2);
			for (int k=0; k<size; k++)	{
				if (isEmpty(i,j))	{
					myPossibilities[i][j].push_back(value);
				}
				else	{
					myPossibilities[i][j].push_back(0);
				}
			}
		}
	}
}	//end Puzzle default constructor

int Puzzle::getSize()	{
	return (size);	//number of elements in array
}	//end function getSize

void Puzzle::printPuzzle()	{

//	cout << "printing game " << endl<< endl;
	for (int i=0; i<size; i++)	{
		for (int j=0; j<size; j++)	{
			cout << myPuzzle[i][j]<< " ";
		}
		cout << endl;
	}

}

int Puzzle:: isEmpty(int i, int j)	{
	if (myPuzzle[i][j]==0)
		return 1;
	else
		return 0;
}

void Puzzle::checkRow(int row)	{
	//step through each column in the row
	for (int i=0; i<size; i++)	{
		//if this spot is filled...
		if (!isEmpty(row, i))	{
			//adjust this row in the possibilities vector-- all columns in this row should be 0 for this number
			for (int col=0; col<size; col++)	{
				myPossibilities[row][col][myPuzzle[row][i]-1]=0;	
				//use myPuzzle[row][i]-1 to adjust from 1-9 to 0-8
			}
		}
	}
}

void Puzzle::checkColumn(int col)	{
	//same logic as checkRow--except looking down each column instead of each row
	for (int i=0; i<size; i++)	{
		if (!isEmpty(i, col))	{
			for (int row=0; row<size; row++)	{
				myPossibilities[row][col][myPuzzle[i][col]-1]=0;
			}
		}
	}
}

void Puzzle:: checkSquare(int row, int col)	{ 
	//create variables the mark the top left corner of each mini 3x3 square
	int startCol=0, startRow=0;
	//depending on which square the spot lies, update the startRow and startCol variables accordingly (9 different start locations)
	if ((row<=2) && (col<=2))       {
        	startCol=0;
	        startRow=0;
	}
	else if ((row<=2) && (col<=5))  {
        	startCol=3;
	        startRow=0;
	}
	else if ((row<=2) && (col<=8))  {
        	startCol=6;
	        startRow=0;
	}
	else if ((row<=5) && (col<=2))  {
        	startCol=0;
        	startRow=3;
	}
	else if ((row<=5) && (col<=5))  {
        	startCol=3;
        	startRow=3;
	}
	else if ((row<=5) && (col<=8))  {
        	startCol=6;
        	startRow=3;
	}
	else if ((row<=8) && (col<=2))  {
        	startCol=0;
        	startRow=6;
	}
	else if ((row<=8) && (col<=5))  {
        	startCol=3;
        	startRow=6;
	}
	else if((row<=8) && (col<=8))   {
        	startCol=6;
        	startRow=6;
	}
	
	//from start location and up/over 3, if a spot is filled, update the possibilities vector for all spots in the square accordingly
	//this number is no longer a possibility anywhere in the square
	for (int i=startRow; i<(startRow+3); i++)       {
        	for (int j=startCol; j<(startCol+3); j++)       {
                	if (!isEmpty(i, j))     {
                        	for (int y=startRow; y<(startRow+3); y++)       {
                                	for (int z=startCol; z<(startCol+3); z++)       {
                                        	myPossibilities[y][z][myPuzzle[i][j]-1]=0;
                                	}
                        	}
                	}
        	}
	}	

}	//end checkSquare
int Puzzle::boardFull()	{
	int numEmpty=0;
	for (int i=0; i<size; i++)	{
		for (int j=0; j<size; j++)	{
			if (isEmpty(i,j))	{
				numEmpty++;
			}
		}
	}
	if (numEmpty==0)
		return 1;
}

void Puzzle:: play()	{
	printPuzzle();
	while (!boardFull())	{
		//step through each row and column
		for (int i=0; i<size; i++)	{
			for (int j=0; j<size; j++)	{
				//if a spot is empty...
				if (isEmpty(i,j))	{
					//perform checks to update possibilities vector
					checkRow(i);
					checkColumn(j);
					checkSquare(i,j);
					int sum=0, solution=0;
					//if there is only one possibility at this location, set in on the board
					for (int n=0; n<size; n++)	{
						if (myPossibilities[i][j][n]==1)	{
							solution=n+1;
						}
						sum= sum + myPossibilities[i][j][n];
					}
					if (sum==1)	{
						myPuzzle[i][j]=solution;
					}
					//run singleton algorithms to set numbers on board
					singletonRow(i);
					singletonCol(j);
				}
				else	{
				//if a spot is filled, update the possibilities vector-- no other numbers are a possibility at this spot
					for (int n=0; n<size; n++)	{
						myPossibilities[i][j][n]=0;
					}
				}
			}
		}
		
	}
		cout << "Completed Puzzle" << endl;
		printPuzzle();
}



void Puzzle::singletonRow(int row)	{
	vector <int> position;	//vector that tracks position of possibilities
	vector <int> value;	//vector that tracks possible values
	//looping through each number
	for (int num=0; num<size; num++)	{
			for (int col=0; col<size; col++)	{
				//if the number is a posibility, increment values vector for this number
				if (myPossibilities[row][col][num]==1)	{
					//location where you could potentially place this number
					position.push_back(col);
					//increment number of times this number appears
					value.push_back(num);
				}
			}
		}//end num for loop

	for (int p=0; p<value.size(); p++)	{
	//step through every possible value in the values vector
		int mycount=std::count(value.begin(), value.end(), value[p]);
		//use count algorithm to count how many times a possiblity occurs
//		cout << p << " occurs ", mycount << " times" << endl;
		if (mycount==1)	{
			//if a possibiity only occurs once in row, set the spot where it occurs to its value
			myPuzzle[row][position[p]]=value[p]+1;
		}
	}
}	

void Puzzle::singletonCol(int col)	{
	vector <int> position;
	vector <int> value;
	//looping through each number
	for (int num=0; num<size; num++)	{
			for (int row=0; row<size; row++)	{
				//if the number is a posibility, increment values vector for this number
				if (myPossibilities[row][col][num]==1)	{
					//location where you could potentially place this number
					position.push_back(row);
					//increment number of times this number appears
					value.push_back(num);	//change to vector and push back num
				}
			}
		}	//end num for loop

	for (int p=0; p<value.size(); p++)	{
	//step through every possible value
		int mycount=std::count(value.begin(), value.end(), value[p]);
//		cout << p << " occurs ", mycount << " times" << endl;
		if (mycount==1)	{
			myPuzzle[position[p]][col]=value[p]+1;
		}
	}
}

#endif
