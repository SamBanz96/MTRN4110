#include <iostream>
#include <Windows.h>
#include <math.h>
#include <fstream>
#include <cstring>
#include <stdlib.h> 
#include <string>
#include <vector>

#define MAP "C:\\Users\\JUni\\iCloudDrive\\University\\4.2\\MTRN4110\\Assignment\\Part B\\z1234567_MTRN4110_PhaseB\\Map.txt"

using namespace std;

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
					//Single Digit Steps
					int k = (Cells[Vcount][j] % 10);
					P[4 * j + 2] = '0' + k;

					//Double Digit Steps
					if (Cells[Vcount][j] > 9)
					{
						int k = (Cells[Vcount][j] / 10 % 10);
						P[4 * j + 1] = '0' + k;
					}
				}
			}

			Vcount++;
		}
		cout << P << endl; //Print all rows (including edited cell rows)
		WC = !WC;
	}
}

void PathFinder2(bool HW[6][9], bool VW[5][10], int Cells[5][9], int Start[2], char Header)
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

	char HCurrent=Header;
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
	for (int k = 0; k < Paths.size(); k++)
	{
		int v = Directions[k].length();
		if (v<=SP)
		{
			a = k;
			SP = v;
		}

		cout << "Path:" << k + 1 << endl;
		cout << "Direction: " << Directions[k]  <<" Length:"<< Directions[k].length()<< endl;
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

	cout << "Best Path:" << a + 1 << endl;
	cout << "Direction: " << Directions[a] << endl;
	PathPrinter(Cells, PTemp, Start);

}



int main() 
{
	bool HW[6][9] = { {1,1,1,1,1,1,1,1,1}, {0,1,0,0,0,0,1,0,0} ,{1,0,0,0,1,1,0,1,0}, {0,0,1,0,1,1,1,0,0}, {0,0,1,0,0,1,0,1,0}, {1,1,1,1,1,1,1,1,1} };
	bool VW[5][10] = { {1,0,0,0,1,0,0,0,0,1},{1,0,0,1,0,0,1,1,0,1},{1,0,1,1,1,0,0,0,0,1},{1,1,0,0,0,0,1,0,0,1},{1,1,0,0,1,0,0,1,0,1} };
	int C[5][9] = { {16,15,14,13,10,9,8,7,6},{15,14,15,12,11,10,3,6,5},{14,13,16,11,0,1,2,3,4},{15,12,11,10,9,10,5,4,5},{16,13,12,11,8,7,6,7,6} };
	int S[2] = { 0,0 };
	char H = 'S';
	vector<vector<vector<int>>> Paths;

	PathFinder2(HW, VW, C, S, H);
	return 0;
}




