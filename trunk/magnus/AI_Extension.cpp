
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
int map[MAPX][MAPY];

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

void Pather::printMap(int map[MAPX][MAPY])
{
	for (int row=0; row<MAPX; row++)
	{
		for (int col=0; col<MAPY; col++) 
		{
			cout << map[row][col] << " ";
		}
		cout << "\n";
	}	
}

//-------------------------------------------------------------------------------------


Pather *mPather = new Pather();
microTalker *mTalker = new microTalker();

int _tmain(int argc, _TCHAR* argv[])
{
	
	mPather->readPos();

	// Create Map
	int map[MAPX][MAPY] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
						   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	mPather->getMap(map);
	
	bool done = false;
	char buf[ 256 ];
	
	while (!done)
	{
		mTalker->Print();
		printf("\n# # to move, q to quit, r to redraw, d to toggle doors, t for touched\n");
	
		std::cin.getline( buf, 256 );
		if ( *buf )
		{
			if ( buf[0] == 'q' ) {
				done = true;
			}
			else if ( buf[0] == 'd' ) {
				mTalker->ToggleDoor();
				mTalker->ClearPath();
			}
			else if ( buf[0] == 't' ) {
				mTalker->ToggleTouched();   
			}    
			else if ( buf[0] == 'r' ) {
				mTalker->ClearPath();
			}
			else if ( isdigit( buf[0] ) ) {
				int x, y;
				sscanf( buf, "%d %d", &x, &y );	// sleazy, I know
				mTalker->SetPos( x, y );
			} 
		}
		else
		{			
			mTalker->ClearPath();
		}

	}

	// Quit
	string waitstr;
	cout << "Press any key to quit";
	getline (cin,waitstr);
	
	return 0;
	
}






