
/************************************/
/*Magnus Andersson, 2010			*/
/*Chalmers University of Technology	*/
/*Simulaton Engines					*/
/*									*/
/*Path-Planning / Avoidance / AI	*/
/*OGRE 3D Extension					*/
/************************************/

// Include Precompiled Header
#include "stdafx.h"
// Include AI Extension Header
#include "AI_Extension.h"

#include "microTalker.h"

#define USE_MICROPATHER

// Include Various
#include <ctype.h>
#include <stdio.h>
#include <memory.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Include MicroPather Extension
#include "micropather.h"
using namespace micropather;

// Map size
const int MAPX = 20;
const int MAPY = 20;

// Path Vector
std::vector<void*> path;
// Structs
Pather::vec3_ vec3;
Pather::currentNode_ currentNode;
Pather::goalNode_ goalNode;
Pather::nextNode_ nextNode;

// Global Variables
//int gMap[MAPX][MAPY];
Pather *mPather = new Pather();
microTalker *mTalker;

int intMap[MAPY][MAPX] =   {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
							{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};


//-------------------------------------------------------------------------------------
Pather::Pather(void)
{
}

Pather::~Pather(void)
{
}

void Pather::getMap()
{
	//Should have the map as the input.
	int gMap[MAPY][MAPX] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	memcpy(intMap, gMap, sizeof(gMap)); 



	printMap(gMap);
	mTalker = new microTalker(gMap);
}

void Pather::getCurrentNode()
{ 
}

void Pather::getGoalNode()
{
}

void Pather::setVel()
{
}

void Pather::distToGoalNode(void)
{
}

void Pather::currentPos()
{
	// Set the current position
	char buf[ 256 ];
	printf("\nSet starting Node:# #\n");
	std::cin.getline( buf, 256 );
	int x, y;
	sscanf( buf, "%d %d", &x, &y );
	// Sets the current position for in miroTalker
	mTalker->setCurrentPos(x, y);
}
int Pather::goalPos()
{
	// Sets the goal position
	char buf[ 256 ];
	int res = 0;
	printf("\nSet goal Node# #\n");
	std::cin.getline( buf, 256 );
	int x, y;
	sscanf( buf, "%d %d", &x, &y );
	//Call mTalker
	res = mTalker->SetPos(x, y );
	return res;
}

void Pather::getPath()
{
	path = mTalker->returnPath();
	unsigned sizeB = path.size();
	for(int f=0; f<sizeB; ++f ) 
	{
//		printf("Path Index:%d %d %d ",(int)path[f], (int)path[f]/20, (int)path[f]%20);
//		printf("\n");
	}
	
}

void Pather::printPos(currentNode_ currentNode)
{
}

void Pather::printMap(int map[MAPY][MAPX])
{
	for (int row=0; row<MAPY; row++)
	{
		for (int col=0; col<MAPX; col++)
		{
			cout << map[row][col] << " ";
		}
		cout << "\n";
	}	
}

void Pather::printPath()
{

	int addNr;
	for (int row=0; row<MAPY; row++)
	{
		for (int col=0; col<MAPX; col++)
		{
			
			addNr = 0;
			for (int f = 0; f<path.size();f++)
			{
				if (row == (int)path[f]%20 && col == (int)path[f]/20 && addNr == 0)
				{
					cout << "." << " ";
					addNr = 1;
				}
			}
			if (addNr == 0)
				cout << intMap[row][col] << " ";
		}
		cout << "\n";
	}	
}
void Pather::AImain()
{
	// Get the map
	mPather->getMap();
	
	//While
	bool done = false;
	int res = 0;
	while (!done)
	{
		currentPos();
		res = mPather->goalPos();
		printf( "SetPos Returned %d\n", res);
		getPath();
		printPath();
	}
}

//-------------------------------------------------------------------------------------




int _tmain(int argc, _TCHAR* argv[])
{
	
	mPather->AImain();
	
	return 0;
	
}






