
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


// Structs
Pather::vec3_ vec3;
Pather::currentNode_ currentNode;
Pather::goalNode_ goalNode;
Pather::nextNode_ nextNode;

// Global Variables
//int gMap[MAPX][MAPY];
Pather *mPather = new Pather();


//-------------------------------------------------------------------------------------
Pather::Pather(void)
{
}

Pather::~Pather(void)
{
}

void Pather::getMap(int map[MAPX][MAPY])
{

	printMap(map);
}

void Pather::getCurrentNode()
{ 
}

void Pather::getGoalNode()
{
}

void setVel()
{
	//ogre->setSpeed(200);
}

void Pather::distToGoalNode(void)
{
}

void Pather::readPos()
{
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

void AImain()
{
	mPather->readPos();

	// Create Map
	int gMap[MAPY][MAPX] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
							{1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
							{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	mPather->getMap(gMap);
	microTalker *mTalker = new microTalker(gMap);
	// PathP
	bool done = false;
	char buf[ 256 ];
	int res = 0;
	while (!done)
	{
		printf("\n# # to move\n");
		std::cin.getline( buf, 256 );
		int x, y;
		sscanf( buf, "%d %d", &x, &y );	// sleazy, I know
		res = mTalker->SetPos(x, y );
		printf( "SetPos Returned %d\n", res);

	}

	// Quit
	string waitstr;
	cout << "Press any key to quit";
	getline (cin,waitstr);
}

//-------------------------------------------------------------------------------------




int _tmain(int argc, _TCHAR* argv[])
{
	
	AImain();
	
	return 0;
	
}






