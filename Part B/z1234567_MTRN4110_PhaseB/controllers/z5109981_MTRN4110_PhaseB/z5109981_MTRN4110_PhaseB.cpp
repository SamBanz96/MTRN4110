
// File:          z5109981_MTRN4110_PhaseB.cpp
// Date:          11.07.2020
// Description:   Assignment Part 2
// Author:        Samson Banzi
// Modifications:

#include <webots/Robot.hpp>
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <fstream>
#include <cstring>
#include <stdlib.h> 
#include <string>
#include <vector>

#define TIME_STEP 64

//Original Paths
#define MAP "C:\\Users\\JUni\\iCloudDrive\\University\\4.2\\MTRN4110\\Assignment\\Part B\\z1234567_MTRN4110_PhaseB\\Map.txt" 
#define MAPOUT "C:\\Users\\JUni\\iCloudDrive\\University\\4.2\\MTRN4110\\Assignment\\Part B\\z1234567_MTRN4110_PhaseB\\PathPlanFound.txt"

//Asked Paths
//#define MAP “../../Map.txt”
//#define MAPOUT ../../PathPlanFound.txt”

using namespace webots;
using namespace std;

//SubFunctions
//Map Writing and Wall Arrays Definition
char WallArrays(bool HW[6][9], bool VW[5][10], int Start[2])
{
	string M;			//String used to print out Map
	ifstream MapFile;	//Map file
	MapFile.open(MAP);
	bool WC = 0;		//0:Horizontal Wall Row,1:Vertical Wall/Cell Row
	int Hcount = 0;		//Horizontal Matrix Row Increment
	int Vcount = 0;		//Vertical Matrix Row Increment
	char H;				//Heading


	while (std::getline(MapFile, M)) //Until end of Text
	{
		cout << M << endl; //Print Each Row before Matrix application

		if (!WC) //Horizontal Walls (---)
		{
			for (int i = 0; i < 9; i++)//Horizontal Wall Coulumns
			{
				if (M[4 * i + 1] == '-') //Check for wall in each cell
				{
					HW[Hcount][i] = 1;//Wall
				}

				else
				{
					HW[Hcount][i] = 0;//No Wall
				}
			}

			Hcount++;
		}

		else //Vertical Walls (|)
		{
			for (int i = 0; i < 10; i++)//Vertical Wall Coulumns
			{
				if (M[4 * i] == '|')//Check for wall in each cell
				{
					VW[Vcount][i] = 1;//Wall
				}

				else
				{
					VW[Vcount][i] = 0;//No Wall
				}

				//Find Location and Direction of <^v> and save as Start
				if (i != 9)//Cells have 9 Columns, V Walls have 10 (discredit)
				{
					switch (M[4 * i + 2])
					{
					case '^':
						H = 'N';
						Start[0] = Vcount;
						Start[1] = i;
						break;

					case 'v':
						H = 'S';
						Start[0] = Vcount;
						Start[1] = i;
						break;

					case '<':
						H = 'W';
						Start[0] = Vcount;
						Start[1] = i;
						break;

					case '>':
						H = 'E';
						Start[0] = Vcount;
						Start[1] = i;
						break;

					default://if no symbol is found
						break;
					}

				}

			}

			Vcount++;
		}
		WC = !WC;//Switch Wall row type per row
	}

	return H;
}

//Flood Fill Algorithm
void FloodFill(bool HW[6][9], bool VW[5][10], int Cells[5][9])
{
	bool ChangeMade = 1;//Check if cell change is made
	int CStep = 0;//Starting Point
	while (ChangeMade)
	{
		ChangeMade = 0;//Start as 0 and only change if a cell change is made;
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 9; j++)//Cell Matrix
			{
				if (Cells[i][j] == CStep)//If Current Step Number is found
				{
					//Check if adjacent wall is not defined and if there is no wall blocking cells to change
					if (!HW[i][j] && Cells[i - 1][j] == 50)
					{
						Cells[i - 1][j] = CStep + 1;
						ChangeMade = 1;
					}

					if (!HW[i + 1][j] && Cells[i + 1][j] == 50)
					{
						Cells[i + 1][j] = CStep + 1;
						ChangeMade = 1;
					}

					if (!VW[i][j] && Cells[i][j - 1] == 50)
					{
						Cells[i][j - 1] = CStep + 1;
						ChangeMade = 1;
					}

					if (!VW[i][j + 1] && Cells[i][j + 1] == 50)
					{
						Cells[i][j + 1] = CStep + 1;
						ChangeMade = 1;
					}
				}
			}
		}
		CStep++;//Next Step to check
	}


}

//Main Path Printer
void PathPrinter(int Cells[5][9], int Path[5][9], int Start[2])
{
	bool WC = 0;
	int Vcount = 0;
	string P;
	ifstream MapFile;
	MapFile.open(MAP);
	//MapFile.clear();
	//MapFile.seekg(0);
	while (std::getline(MapFile, P))
	{
		if (WC)//Only Checking Cell(i.e Vertical Wall) rows 
		{

			for (int j = 0; j < 9; j++)
			{
				if (Vcount == Start[0] && j == Start[1]) {}//Do not Change Robot Representation in Print
				else if (Path[Vcount][j] != 50)
				{
					//Change all positions in print with path steps
					int k = (Cells[Vcount][j] % 10);

					if (Cells[Vcount][j] < 10)//Single Digit Steps
					{
						P[4 * j + 2] = '0' + k;
					}

					//Double Digit Steps
					else
					{
						int k2 = (Cells[Vcount][j] / 10 % 10);
						P[4 * j + 2] = '0' + k2;
						P[4 * j + 3] = '0' + k;
					}
				}
			}

			Vcount++;
		}
		cout << P << endl; //Print all rows (including edited cell rows)
		WC = !WC;
	}
}

void PathFinder(bool HW[6][9], bool VW[5][10], int Cells[5][9], int Start[2], char Header)
{
	vector<vector<vector<int>>> Paths;
	vector<string>Directions;
	int CSVal = Cells[Start[0]][Start[1]];		//Current Step Value

	int CSPos[2];								//Current Step Position
	CSPos[0] = Start[0];
	CSPos[1] = Start[1];
	int TempPos[2];								//Temporary Step Position
	TempPos[0] = Start[0];
	TempPos[1] = Start[1];

	int PCurrent[5][9];							//Current Step Position Matrix
	int PTemp[5][9];							//Temporary Matrix
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			PTemp[i][j] = 50;
			PCurrent[i][j] = 50;
		}
	}
	PTemp[Start[0]][Start[1]] = CSVal;
	PCurrent[Start[0]][Start[1]] = CSVal;

	string SCurrent;
	SCurrent += '0' + Start[0];
	SCurrent += '0' + Start[1];
	SCurrent += Header;
	string STemp;

	char HCurrent = Header;
	char HTemp;


	int PB;										//Path Branches (at current step)
	int NP = 1;									//Number of Paths
	int Counter = 0;
	int Loop = 0;
	vector<vector<vector<int>>>PRecord;			//Temporary Paths Vector 5x9
	vector<vector<int>>Pos;						//Temporary Current Position Vector 2 
	vector<int>Step;							//Temporary Step Value Vector
	vector<string>Dir;
	vector<char>Head;
	vector<int> t;								//Seperate Temp Vector
	vector<vector<int>>t2;

	while (NP != 0)																		//Stop when all Paths are done (NP Will decrement to 0)
	{
		//NP--;
		Counter++;
		Loop = 0;
		TempPos[0] = CSPos[0];
		TempPos[1] = CSPos[1];
		STemp = SCurrent;
		HTemp = HCurrent;
		//std::cout << endl;
		//std::cout << "Run:" << Counter << endl;
		//std::cout << "Start Position:" << CSPos[0] << "," << CSPos[1] << " Start Value:" << CSVal << endl;

		while (CSVal != 0)																//Stop current Path definition when Goal is met (CSVal=0)
		{
			PB = 0;																		//Start with no Path branches at the start of each step
			CSVal--;
			Loop++;

			//Check Up
			if (!HW[CSPos[0]][CSPos[1]] && Cells[CSPos[0] - 1][CSPos[1]] == CSVal)
			{
				//std::cout << "Up Detected" << endl;
				PB++;																	//Path Branch found
				PTemp[CSPos[0] - 1][CSPos[1]] = CSVal;									//Update Temporary Matrix
				TempPos[0]--;															//Update Temporary Current Position
				switch (HTemp)
				{
				case 'N':
					STemp += 'F';
					break;

				case 'E':
					STemp += "LF";
					HTemp = 'N';
					break;

				case 'S':
					STemp += "LLF";
					HTemp = 'N';
					break;

				case 'W':
					STemp += "RF";
					HTemp = 'N';
					break;

				default:
					break;
				}
			}

			//Left
			if (!VW[CSPos[0]][CSPos[1] + 1] && Cells[CSPos[0]][CSPos[1] + 1] == CSVal)
			{
				//std::cout << "Left Detected" << endl;
				PB++;
				if (PB > 1)																//Multiple Paths found
				{
					//Push last path matrx into Record Vector
					for (int i = 0; i < 1; i++)
					{
						vector<vector<int>>temp1;
						for (int i = 0; i < 5; i++)
						{
							vector<int>temp2;
							for (int j = 0; j < 9; j++)
							{
								temp2.push_back(PTemp[i][j]);
							}
							temp1.push_back(temp2);
						}
						PRecord.push_back(temp1);
					}

					//Push updated current position for last path matrix into Position Vector 
					for (int i = 0; i < 1; i++)
					{
						vector<int>temp;
						for (int i = 0; i < 2; i++)
						{
							temp.push_back(TempPos[i]);
						}
						Pos.push_back(temp);
					}

					//Push updated current value for last path matrix into Value Vector
					Step.push_back(CSVal);

					//Push updated Direction string for last path into Direction Vector
					Dir.push_back(STemp);
					Head.push_back(HTemp);

					//Reset Temp Matrix to before changes
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 9; j++)
						{
							PTemp[i][j] = PCurrent[i][j];
						}
					}
					//Reset Temp Position to before changes
					TempPos[0] = CSPos[0];
					TempPos[1] = CSPos[1];

					//Reset Temp Direction String to before changes
					STemp = SCurrent;
					HTemp = HCurrent;
				}

				//Update Matrix for new Path 
				PTemp[CSPos[0]][CSPos[1] + 1] = CSVal;
				//Update Postion for New Path
				TempPos[1]++;
				switch (HTemp)
				{
				case 'N':
					STemp += "RF";
					HTemp = 'E';
					break;

				case 'E':
					STemp += 'F';
					break;

				case 'S':
					STemp += "LF";
					HTemp = 'E';
					break;

				case 'W':
					STemp += "LLF";
					HTemp = 'E';
					break;

				default:
					break;
				}
			}

			//Check Down
			if (!HW[CSPos[0] + 1][CSPos[1]] && Cells[CSPos[0] + 1][CSPos[1]] == CSVal)
			{
				//std::cout << "Down Detected" << endl;
				PB++;
				if (PB > 1)
				{
					//std::cout << "New Path Detected" << endl;
					//NP++;
					for (int i = 0; i < 1; i++)
					{
						vector<vector<int>>temp1;
						for (int i = 0; i < 5; i++)
						{
							vector<int>temp2;
							for (int j = 0; j < 9; j++)
							{
								temp2.push_back(PTemp[i][j]);
							}
							temp1.push_back(temp2);
						}
						PRecord.push_back(temp1);
					}


					for (int i = 0; i < 1; i++)
					{
						vector<int>temp;
						for (int i = 0; i < 2; i++)
						{
							temp.push_back(TempPos[i]);
						}
						Pos.push_back(temp);
					}

					Step.push_back(CSVal);

					Dir.push_back(STemp);
					Head.push_back(HTemp);
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 9; j++)
						{
							PTemp[i][j] = PCurrent[i][j];
						}
					}
					TempPos[0] = CSPos[0];
					TempPos[1] = CSPos[1];

					STemp = SCurrent;
					HTemp = HCurrent;
				}
				PTemp[CSPos[0] + 1][CSPos[1]] = CSVal;
				TempPos[0]++;
				switch (HTemp)
				{
				case 'N':
					STemp += "LLF";
					HTemp = 'S';
					break;

				case 'E':
					STemp += "RF";
					HTemp = 'S';
					break;

				case 'S':
					STemp += 'F';
					break;

				case 'W':
					STemp += "LF";
					HTemp = 'S';
					break;

				default:
					break;
				}
			}

			//Right
			if (!VW[CSPos[0]][CSPos[1]] && Cells[CSPos[0]][CSPos[1] - 1] == CSVal)
			{
				//std::cout << "Right Detected" << endl;
				PB++;
				if (PB > 1)
				{
					//std::cout << "New Path Detected" << endl;
					//NP++;
					for (int i = 0; i < 1; i++)
					{
						vector<vector<int>>temp1;
						for (int i = 0; i < 5; i++)
						{
							vector<int>temp2;
							for (int j = 0; j < 9; j++)
							{
								temp2.push_back(PTemp[i][j]);
							}
							temp1.push_back(temp2);
						}
						PRecord.push_back(temp1);
					}


					for (int i = 0; i < 1; i++)
					{
						vector<int>temp;
						for (int i = 0; i < 2; i++)
						{
							temp.push_back(TempPos[i]);
						}
						Pos.push_back(temp);
					}

					Step.push_back(CSVal);

					Dir.push_back(STemp);
					Head.push_back(HTemp);
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 9; j++)
						{
							PTemp[i][j] = PCurrent[i][j];
						}
					}
					TempPos[0] = CSPos[0];
					TempPos[1] = CSPos[1];

					STemp = SCurrent;
					HTemp = HCurrent;
				}
				PTemp[CSPos[0]][CSPos[1] - 1] = CSVal;
				TempPos[1]--;
				switch (HTemp)
				{
				case 'N':
					STemp += "LF";
					HTemp = 'W';
					break;

				case 'E':
					STemp += "LLF";
					HTemp = 'W';
					break;

				case 'S':
					STemp += "RF";
					HTemp = 'W';
					break;

				case 'W':
					STemp += 'F';
					break;

				default:
					break;
				}
			}

			//Update Current Matrix from Temp Path Matrix
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					PCurrent[i][j] = PTemp[i][j];
				}
			}
			//Update Current Position from Temp Postion
			CSPos[0] = TempPos[0];
			CSPos[1] = TempPos[1];

			//Update Current Directions from Temp Directions
			SCurrent = STemp;
			HCurrent = HTemp;
		}

		//Insert Defined Path into Main Path vector
		for (int i = 0; i < 1; i++)
		{
			vector<vector<int>>temp1;
			for (int i = 0; i < 5; i++)
			{
				vector<int>temp2;
				for (int j = 0; j < 9; j++)
				{
					temp2.push_back(PCurrent[i][j]);
				}
				temp1.push_back(temp2);
			}
			Paths.push_back(temp1);
		}

		//Insert Direction String into vector
		Directions.push_back(SCurrent);

		//Check Current Paths needed to be defined
		if (PRecord.empty())
		{
			NP = 0;
		}
		else
		{
			//Place last saved Paths furthest value as the Current Value
			CSVal = Step.back();
			//Step.pop_back()*************************************?

			//Place last saved Position as the Current Position
			t.clear();
			t = Pos.back();
			CSPos[1] = t.back();
			t.pop_back();
			CSPos[0] = t.back();

			//Place last saved Path Matrix as the Current Matrix
			t2.clear();
			t2 = PRecord.back();

			for (int i = 0; i < t2.size(); i++)
			{

				for (int j = 0; j < t2[i].size(); j++)
				{
					PTemp[i][j] = t2[i][j];
					PCurrent[i][j] = t2[i][j];
				}
			}

			SCurrent = Dir.back();
			HCurrent = Head.back();

			PRecord.pop_back();
			Pos.pop_back();
			Step.pop_back();
			Dir.pop_back();
			Head.pop_back();
		}

	}
	//Print all developed Path Matrix and Strings
	int SP = 50;
	int a;
	cout << "--- Task 2 ---"<< endl;
	for (int k = 0; k < Paths.size(); k++)
	{
		int v = Directions[k].length();
		if (v <= SP)
		{
			a = k;
			SP = v;
		}
		cout << "--- Path " << k + 1 << " ---" << endl;
		for (int i = 0; i < Paths[k].size(); i++)
		{
			for (int j = 0; j < Paths[k][i].size(); j++)
			{
				PCurrent[i][j] = Paths[k][i][j];
			}
		}
		PathPrinter(Cells, PCurrent, Start);
	}

	for (int i = 0; i < Paths[a].size(); i++)
	{
		for (int j = 0; j < Paths[a][i].size(); j++)
		{
			PTemp[i][j] = Paths[a][i][j];
		}
	}

	cout << "--- Task 3 ---"<< endl;
	PathPrinter(Cells, PTemp, Start);
	cout << "Steps: " << Directions[a].length()-3 << endl;
	cout << "Path: " << Directions[a] << endl;
           ofstream Out(MAPOUT);
	Out << Directions[a];

	cout << "--- Task 4 ---" << endl;
	cout << "File: ../../PathPlanFound.txt" << endl;
	cout << "Path: " << Directions[a] << endl;
}

int main(int argc, char** argv)
{
	Robot* robot = new Robot();

	//Map Reading Variables

	//Array Map Initialisation
	bool HorWall[6][9]; //Horizontal Wall Definition is 6x9
	bool VertWall[5][10];//Vertical Wall Definition is 5x10
	int Cells[5][9]; //Cells Definition is 5x9
	//Cell Array Definition

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			Cells[i][j] = 50;
		}
	}
	Cells[2][4] = 0; //Centre Cell

	char Heading;
	int Start[2];

	while (robot->step(TIME_STEP) != -1)
	{
      	           cout<< "--- Task 1 ---"<<endl;
		Heading = WallArrays(HorWall, VertWall, Start);
		FloodFill(HorWall, VertWall, Cells);
		PathFinder(HorWall, VertWall, Cells, Start, Heading);
		break;
	}
	delete robot;
	return 0;
}