//Clean Code
/***********************TO DO**************************
-MultiPath Finder
-Best Path Chooser
-Path Write
-CLEAN AND ANNOTATE CODE
***********Map Character Definition is 37x11***********
****************************************************/

// File:          z5109981_MTRN4110_PhaseB.cpp
// Date:
// Description:
// Author:
// Modifications:

#include <webots/Robot.hpp>
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <fstream>
#include <cstring>
#include <stdlib.h> 
#include <string>

#define TIME_STEP 64
//#define MAP "C:\\Users\\JUni\\iCloudDrive\\University\\4.2\\MTRN4110\\Assignment\\Part B\\z1234567_MTRN4110_PhaseB\\Map.txt"
#define MAP "C:\\Users\\banzi\\iCloudDrive\\University\\4.2\\MTRN4110\\Assignment\\Part B\\z1234567_MTRN4110_PhaseB\\Map.txt"
using namespace webots;
using namespace std;

int main(int argc, char **argv) 
{
	
	Robot *robot = new Robot();

	//Map Reading Variables
	ifstream MapFile;
	MapFile.open(MAP);
	string M;

	//Array Map Initialisation
	bool HorWall[6][9]; //Horizontal Wall Definition is 6x9
	bool VertWall[5][10];//Vertical Wall Definition is 5x10
	int Cells[5][9]; //Cells Definition is 5x9
  	bool WC=0; //Line Switcher (0 for HorWalls, 1 for VertWalls)
  	int Hcount=0;
	int Vcount = 0;

	//Flood Fill Variables
	int CStep = 0; //Current Explored Value
	bool ChangeMade = 1;
	
	//PathFinder Variables
	char Heading;
	int Start[2];
	int CP[2];
	string P;
	int Path[5][9];
	
	 while (robot->step(TIME_STEP) != -1) 
     {
		 //Cell Array Definition
		 for (int i = 0; i < 5; i++)
		 {
			 for (int j = 0; j < 9; j++) 
			 {
				 Cells[i][j] = 50;
				 Path[i][j] = 50;
			 }
		 }

		 Cells[2][4] = 0; //Centre Cell
		 Path[2][4] = 0;
		 //Map Writing and Wall Arrays Definition
		 while (std::getline(MapFile, M))
		 {
			 cout << M << endl;

			 if (!WC) //Horizontal Walls (---)
			 {
				 for (int i = 0; i < 9; i++)
				 {
					 if (M[4*i+1]=='-')
					 {
						 HorWall[Hcount][i] = 1;
					 }

					 else
					 {
						 HorWall[Hcount][i] = 0;
					 }
				 }

				 Hcount++;
			 }

			 else //Vertical Walls (|)
			 {
				 for (int i = 0; i < 10; i++)
				 {
					 if (M[4 * i] == '|')
					 {
						 VertWall[Vcount][i] = 1;
					 }

					 else
					 {
						 VertWall[Vcount][i] = 0;
					 }

					 //Find Location and Direction of <^v> and save as Start
					 if (i!=9)
					 {
						 switch (M[4 * i + 2])
						 {
						 case '^':
							 Heading = 'N';
							 Start[0] = Vcount;
							 Start[1] = i;
							 break;

						 case 'v':
							 Heading = 'S';
							 Start[0] = Vcount;
							 Start[1] = i;
							 break;

						 case '<':
							 Heading = 'W';
							 Start[0] = Vcount;
							 Start[1] = i;
							 break;

						 case '>':
							 Heading = 'E';
							 Start[0] = Vcount;
							 Start[1] = i;
							 break;

							 default:
								 break;
						 }

					 }
				 }

				 Vcount++;
			 }
                                   WC = !WC;
		 }

		 //Printing Functions
		 /*for (int i = 0; i < 6; i++)
		 {
			 for (int j = 0; j < 9; j++)
			 {
				 printf("%d",HorWall[i][j]);
			 }
			 cout << endl;
		 }
		 
		 printf("--------------------\n");
		 
		 for (int i = 0; i < 5; i++)
		 {
			 for (int j = 0; j < 10; j++)
			 {
				 printf("%d",VertWall[i][j]);
			 }
			 cout << endl;
		 }

		 printf("--------------------\n");

		 for (int i = 0; i < 5; i++)
		 {
			 for (int j = 0; j < 9; j++)
			 {
				 printf("%d ", Cells[i][j]);
			 }
			 cout << endl;
		 }*/
		  //Flood Fill Algorithm
		 while (ChangeMade)
		 {
			 ChangeMade = 0;
			 for (int i = 0; i < 5; i++)
			 {
				 for (int j = 0; j < 9; j++)
				 {
					 if (Cells[i][j] == CStep)
					 {
						 if (!HorWall[i][j]&&Cells[i-1][j]==50)
						 {
							 Cells[i - 1][j] = CStep + 1;
							 ChangeMade = 1;
						 }

						 if (!HorWall[i+1][j] && Cells[i + 1][j] == 50)
						 {
							 Cells[i + 1][j] = CStep + 1;
							 ChangeMade = 1;
						 }

						 if (!VertWall[i][j] && Cells[i][j - 1] == 50)
						 {
							 Cells[i][j -1 ] = CStep + 1;
							 ChangeMade = 1;
						 }

						 if (!VertWall[i][j + 1] && Cells[i][j + 1] == 50)
						 {
							 Cells[i][j + 1] = CStep + 1;
							 ChangeMade = 1;
						 }
					 }
				 }
			 }
			 CStep++;
		 }
		 
		 /*for (int i = 0; i < 5; i++)
		 {
			 for (int j = 0; j < 9; j++)
			 {
				 printf("%.2d ", Cells[i][j]);
			 }
			 cout << endl;
		 }*/

		 
                        cout<<"xxxx"<<endl;



		 //*****************PathFinder
		 CStep = Cells[Start[0]][Start[1]];
		 CP[0] = Start[0];
		 CP[1] = Start[1];
		 Path[CP[0]][CP[1]]=CStep;
		 while (CStep!=0)
		 {
		 CStep--;
			 if (!HorWall[CP[0]][CP[1]]&&Cells[CP[0]-1][CP[1]]==CStep)
			 {
				 Path[CP[0] - 1][CP[1]] = CStep;
				 CP[0]--;
			 }
			 else if (!VertWall[CP[0]][CP[1]+1] && Cells[CP[0]][CP[1] + 1] == CStep)
			 {
				 Path[CP[0]][CP[1] + 1] = CStep;
				 CP[1]++;
			 }
			 else if (!HorWall[CP[0]+1][CP[1]] && Cells[CP[0] + 1][CP[1]] == CStep)
			 {
				 Path[CP[0] + 1][CP[1]] = CStep;
				 CP[0]++;
			 }
			 else if (!VertWall[CP[0]][CP[1]]&&Cells[CP[0]][CP[1] - 1] == CStep)
			 {
				 Path[CP[0]][CP[1] - 1] = CStep;
				 CP[1]--;
			 }
			 
			 
		 }
                        //Path Printer
		 WC = 0;
		 Vcount = 0;

		 MapFile.clear();
		 MapFile.seekg(0);


		 while (std::getline(MapFile, P))
		 {
			 if (WC)
			 {

				 for (int j = 0; j < 9; j++)
				 {
					 if (Vcount == Start[0] && j == Start[1]) {}
					 else if(Path[Vcount][j]!=50)
					 {
						 int k = (Cells[Vcount][j] % 10);
						 P[4 * j + 2] = '0' + k;
						 if (Cells[Vcount][j] > 9)
						 {
							 int k = (Cells[Vcount][j] / 10 % 10);
							 P[4 * j + 1] = '0' + k;
						 }
					 }
				 }

				 Vcount++;
			 }
			 cout << P << endl;
			 WC = !WC;
		 }

		 /*for (int i = 0; i < 5; i++)
		 {
			for (int j = 0; j < 9; j++)
			{
				printf("%.2d ", Path[i][j]);
			}
			cout << endl;
		 }*/
		 
		 break;
     }

	delete robot;
	return 0;
}

/*void PathFinder3(bool HW[6][9], bool VW[5][10], int Cells[5][9], int Path[5][9], int Start[2], char Heading, vector<string> Directions)
{
	int CStep = Cells[Start[0]][Start[1]];//Start from the Robot Positions Step
	int CP[2];//Current Postion in Matrix to be analysed (Start at Robot Position)
	CP[0] = Start[0];
	CP[1] = Start[1];
	Path[CP[0]][CP[1]] = CStep;//Equate 1st postiion of Path Matrix

	string X = '0' + Start[0];
	X += '0' + Start[1];
	X += Heading;
}*/

//Print Functions

	//Print all developed Path Matrix
	//for (int k = 0; k < Paths.size(); k++)
	//{
	//	for (int i = 0; i < Paths[k].size(); i++)
	//	{
	//		for (int j = 0; j < Paths[k][i].size(); j++)
	//		{
	//			std::cout << Paths[k][i][j] << " ";
	//		}
	//		std::cout << endl;
	//	}
	//	std::cout << "-------------------" << endl;
	//}

		//std::cout << "**************" << endl;
		//std::cout <<"Current Position:"<< CSPos[0] <<","<< CSPos[1] <<" Current Value:"<< CSVal <<endl;
		//std::cout << "**************" << endl;

//std::cout << "Run:" << Counter << endl;
//std::cout << " Step:" << Loop << endl;
//std::cout << "Up Detected" << endl;
//std::cout << "Left Detected" << endl;
//std::cout << "Down Detected" << endl;
//std::cout << "Right Detected" << endl;
//std::cout << "New Path Detected" << endl;

//std::cout << "-------------------" << endl;
//std::cout << "PRecords" << endl;
//
//for (int k = 0; k < PRecord.size(); k++)
//{
//	for (int i = 0; i < PRecord[k].size(); i++)
//	{
//		for (int j = 0; j < PRecord[k][i].size(); j++)
//		{
//			std::cout << PRecord[k][i][j] << " ";
//		}
//		std::cout << endl;
//	}
//	std::cout << "-------------------" << endl;

PathFinder1
PathFinder(HorWall, VertWall, Cells, Path, Start, X, Heading);
void PathFinder(bool HW[6][9], bool VW[5][10], int Cells[5][9], int Path[5][9], int Start[2], string X, char Heading)
{
	int CStep = Cells[Start[0]][Start[1]];//Start from the Robot Positions Step
	int CP[2];//Current Postion in Matrix to be analysed (Start at Robot Position)
	CP[0] = Start[0];
	CP[1] = Start[1];
	Path[CP[0]][CP[1]] = CStep;//Equate 1st postiion of Path Matrix

	X = '0' + Start[0];
	X += '0' + Start[1];
	X += Heading;
	//cout << X << endl;

	while (CStep != 0)
	{
		CStep--;//Looking for value under Current Positions Step Value in adjacent cells
		//Change corresponding value in Path Matrix and change to new Current Postion

		if (!HW[CP[0]][CP[1]] && Cells[CP[0] - 1][CP[1]] == CStep)//Up
		{
			Path[CP[0] - 1][CP[1]] = CStep;
			CP[0]--;
			switch (Heading)
			{
			case 'N':
				X += 'F';
				break;

			case 'E':
				X += "LF";
				Heading = 'N';
				break;

			case 'S':
				X += "LLF";
				Heading = 'N';
				break;

			case 'W':
				X += "RF";
				Heading = 'N';
				break;

			default:
				break;
			}
		}

		else if (!VW[CP[0]][CP[1] + 1] && Cells[CP[0]][CP[1] + 1] == CStep)//Left
		{
			Path[CP[0]][CP[1] + 1] = CStep;
			CP[1]++;

			switch (Heading)
			{
			case 'N':
				X += "RF";
				Heading = 'E';
				break;

			case 'E':
				X += 'F';
				break;

			case 'S':
				X += "LF";
				Heading = 'E';
				break;

			case 'W':
				X += "LLF";
				Heading = 'E';
				break;

			default:
				break;
			}
		}

		else if (!HW[CP[0] + 1][CP[1]] && Cells[CP[0] + 1][CP[1]] == CStep)//Down
		{
			Path[CP[0] + 1][CP[1]] = CStep;
			CP[0]++;

			switch (Heading)
			{
			case 'N':
				X += "LLF";
				Heading = 'S';
				break;

			case 'E':
				X += "RF";
				Heading = 'S';
				break;

			case 'S':
				X += 'F';
				break;

			case 'W':
				X += "LF";
				Heading = 'S';
				break;

			default:
				break;
			}
		}

		else if (!VW[CP[0]][CP[1]] && Cells[CP[0]][CP[1] - 1] == CStep)//Right
		{
			Path[CP[0]][CP[1] - 1] = CStep;
			CP[1]--;

			switch (Heading)
			{
			case 'N':
				X += "LF";
				Heading = 'W';
				break;

			case 'E':
				X += "LLF";
				Heading = 'W';
				break;

			case 'S':
				X += "RF";
				Heading = 'W';
				break;

			case 'W':
				X += 'F';
				break;

			default:
				break;
			}
		}
	}
	cout << X << endl;
	cout << X.length() << endl;
}

void PathFinder2(bool HW[6][9], bool VW[5][10], int Cells[5][9], int Start[2], vector<vector<vector<int>>> Paths)
{
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

	int PB;										//Path Branches (at current step)
	int NP = 1;									//Number of Paths
	int Counter = 0;
	int Loop = 0;
	vector<vector<vector<int>>>PRecord;			//Temporary Paths Vector 5x9
	vector<vector<int>>Pos;						//Temporary Current Position Vector 2 
	vector<int>Step;							//Temporary Step Value Vector
	vector<int> t;								//Seperate Temp Vector
	vector<vector<int>>t2;

	while (NP != 0)																		//Stop when all Paths are done (NP Will decrement to 0)
	{
		NP--;
		Counter++;
		TempPos[0] = CSPos[0];
		TempPos[1] = CSPos[1];
		//std::cout << endl;
		//std::cout << "Run:" << Counter << endl;
		//std::cout << "Start Position:" << CSPos[0] << "," << CSPos[1] << " Start Value:" << CSVal << endl;

		while (CSVal != 0)																//Stop current Path definition when Goal is met (CSVal=0)
		{
			PB = 0;																		//Start with no Path branches at the start of each step
			CSVal--;
			Loop++;
			//std::cout << endl;
			//std::cout << " Step:" << Loop << endl;
			//std::cout << "Current Position:" << CSPos[0] << "," << CSPos[1] << " Current Value:" << CSVal << endl;
			//std::cout << endl;
			//cout << "Current Matrix" << endl;
			//for (int i = 0; i < 5; i++)
			//{
			//	for (int j = 0; j < 9; j++)
			//	{
			//		cout << PCurrent[i][j] << " ";
			//	}
			//	cout << endl;
			//}
			//cout << endl;
			//cout << "Temporary Matrix" << endl;
			//for (int i = 0; i < 5; i++)
			//{
			//	for (int j = 0; j < 9; j++)
			//	{
			//		cout << PTemp[i][j] << " ";
			//	}
			//	cout << endl;
			//}
			//cout << endl;
			//Check Up
			if (!HW[CSPos[0]][CSPos[1]] && Cells[CSPos[0] - 1][CSPos[1]] == CSVal)
			{
				//std::cout << "Up Detected" << endl;
				PB++;																	//Path Branch found
				PTemp[CSPos[0] - 1][CSPos[1]] = CSVal;									//Update Temporary Matrix
				TempPos[0]--;															//Update Temporary Current Position
			}

			//Left
			if (!VW[CSPos[0]][CSPos[1] + 1] && Cells[CSPos[0]][CSPos[1] + 1] == CSVal)
			{
				//std::cout << "Left Detected" << endl;
				PB++;
				if (PB > 1)																//Multiple Paths found
				{
					//std::cout << "New Path Detected" << endl;
					NP++;																//Number of Paths increase
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
				}

				//Update Matrix for new Path 
				PTemp[CSPos[0]][CSPos[1] + 1] = CSVal;
				//Update Postion for New Path
				TempPos[1]++;
			}

			//Check Down
			if (!HW[CSPos[0] + 1][CSPos[1]] && Cells[CSPos[0] + 1][CSPos[1]] == CSVal)
			{
				//std::cout << "Down Detected" << endl;
				PB++;
				if (PB > 1)
				{
					//std::cout << "New Path Detected" << endl;
					NP++;
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

					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 9; j++)
						{
							PTemp[i][j] = PCurrent[i][j];
						}
					}
					TempPos[0] = CSPos[0];
					TempPos[1] = CSPos[1];
				}
				PTemp[CSPos[0] + 1][CSPos[1]] = CSVal;
				TempPos[0]++;
			}

			//Right
			if (!VW[CSPos[0]][CSPos[1]] && Cells[CSPos[0]][CSPos[1] - 1] == CSVal)
			{
				//std::cout << "Right Detected" << endl;
				PB++;
				if (PB > 1)
				{
					//std::cout << "New Path Detected" << endl;
					NP++;
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

					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < 9; j++)
						{
							PTemp[i][j] = PCurrent[i][j];
						}
					}
					TempPos[0] = CSPos[0];
					TempPos[1] = CSPos[1];
				}
				PTemp[CSPos[0]][CSPos[1] - 1] = CSVal;
				TempPos[1]--;
			}

			//Update Current Matrix to current Path Matrix
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					PCurrent[i][j] = PTemp[i][j];
				}
			}
			//Update Current Position to current Postion
			CSPos[0] = TempPos[0];
			CSPos[1] = TempPos[1];
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

		//Check Current Paths needed to be defined



		//Place last saved Paths furthest value as the Current Value
		CSVal = Step.back();

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




	}
	//Print all developed Path Matrix
	for (int k = 0; k < Paths.size(); k++)
	{
		cout << "Path:" << k + 1 << endl;
		for (int i = 0; i < Paths[k].size(); i++)
		{

			for (int j = 0; j < Paths[k][i].size(); j++)
			{
				PCurrent[i][j] = Paths[k][i][j];
			}
		}
		PathPrinter(Cells, PCurrent, Start);
	}
}