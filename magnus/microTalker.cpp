
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
// Include microTalker Header
#include "microTalker.h"
// Include MicroPather Header
#include "micropather.h"
using namespace micropather;

// Include Various
#include <ctype.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <vector>
#include <iostream>


const int MAPX = 50;
const int MAPY = 14;
const char gMap[MAPX*MAPY+1] =
  //"01234567890123456789012345678901234567890123456789"
	"|------------------------------------------------|"
	"|                |               |       D       |"
	"|       |        |               |       |       |"
	"|       |                  ------|       |       |"
	"|---DD------------               |       |       |"
	"|           |    |               D               |"
	"|           --DD-|------  -----  ----------DD----|"
	"|                |        |         |            |"
	"|                |                  |            |"
	"|                |        |         |            |"
	"|                D        |         D            |"
	"|                |        |         |            |"
    "|                |        |         |            |"
	"|------------------------------------------------|";

MicroPather* micro;
microTalker::microTalker()
{
	posX = 0;
	posY = 0;
	doorsOpen = false;
	showConsidered = false;
	micro = new MicroPather(this, 20);
}
microTalker::~microTalker()
{
	delete micro;
}
int microTalker::X() // Returns posX
{
	return posX;
}	
int microTalker::Y()	// Returns posY
{
	return posY;
}
unsigned microTalker::Checksum()
{
	return micro->Checksum();
}
void microTalker::ClearPath()	// Clear the computed path
{
	path.resize(0);
}
void microTalker::ToggleTouched()
{
	showConsidered = !showConsidered;
	micro->Reset();
}
void microTalker::ToggleDoor()
{
	doorsOpen = !doorsOpen;
	micro->Reset();
}
int microTalker::Passable(int nx, int ny)	// Is the node passable/free
{
	if (    nx >= 0 && nx < MAPX 
			&& ny >= 0 && ny < MAPY )
	{
		int index = ny*MAPX+nx;
		char c = gMap[ index ];
		if ( c == ' ' )
			return 1;
		else if ( c == 'D' )
			return 2;
	}		
	return 0;
}
int microTalker::SetPos(int nx, int ny)	// Set new goal node
{
	int result = 0;
	if ( Passable( nx, ny ) == 1 )
	{
		float totalCost;
		if ( showConsidered )
			micro->Reset();
					
		result = micro->Solve( XYToNode( posX, posY ), XYToNode( nx, ny ), &path, &totalCost );

		if ( result == MicroPather::SOLVED ) {
			posX = nx;
			posY = ny;
		}
		printf( "Pather returned %d\n", result );
	}
	return result;
}
void microTalker::Print() 
{
	char buf[ MAPX+1 ];

	std::vector< void* > stateVec;
		
	if ( showConsidered )
		micro->StatesInPool( &stateVec );
	printf( " doors %s\n", doorsOpen ? "open" : "closed" );
	printf( " 0         10        20        30        40\n" );
	printf( " 01234567890123456789012345678901234567890123456789\n" );
	for( int j=0; j<MAPY; ++j ) {
		// Copy in the line.
		memcpy( buf, &gMap[MAPX*j], MAPX+1 );
		buf[MAPX]=0;

		unsigned k;
		// Wildly inefficient demo code.
		unsigned size = path.size();
		for( k=0; k<size; ++k ) {
			int x, y;
			NodeToXY( path[k], &x, &y );
			if ( y == j )
				buf[x] = '0' + k%10;
		}
		if ( showConsidered )
		{
    		for( k=0; k<stateVec.size(); ++k ) {
           		int x, y;
    			NodeToXY( stateVec[k], &x, &y );
    			if ( y == j )
    				buf[x] = 'x';
        	}     
      	}  		
		// Insert the player
		if ( j==posY )
			buf[posX] = 'i';

		printf( "%d%s\n", j%10, buf );
	}
}
void microTalker::NodeToXY(void* node, int* x, int* y) // Goal node to Pos
{
	int index = (int)node;
	*y = index/MAPX;
	*x = index - *y*MAPX;
}
void* microTalker::XYToNode(int x, int y) // Pos to Goal node
{
	return (void*) (y*MAPX + x);
}
float microTalker::LeastCostEstimate(void* nodeStart, void* nodeEnd) // Least Cost
{
	int xStart, yStart, xEnd, yEnd;
	NodeToXY( nodeStart, &xStart, &yStart );
	NodeToXY( nodeEnd, &xEnd, &yEnd );

	/* Compute the minimum path cost using distance measurement. It is possible
		to compute the exact minimum path using the fact that you can move only 
		on a straight line or on a diagonal, and this will yield a better result.
	*/
	int dx = xStart - xEnd;
	int dy = yStart - yEnd;
	return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
}
void microTalker::AdjacentCost(void* node, std::vector< StateCost > *neighbors) // Adjecent Cost
{
	int x, y;
	const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

	NodeToXY( node, &x, &y );

	for( int i=0; i<8; ++i ) 
	{
		int nx = x + dx[i];
		int ny = y + dy[i];

		int pass = Passable( nx, ny );
		if ( pass > 0 ) 
		{
			if ( pass == 1 || doorsOpen ) 
			{
				// Normal floor
				StateCost nodeCost = { XYToNode( nx, ny ), cost[i] };
				neighbors->push_back( nodeCost );
			}
			else 
			{
				// Normal floor
				StateCost nodeCost = { XYToNode( nx, ny ), FLT_MAX };
				neighbors->push_back( nodeCost );
			}
		}
	}
}
void microTalker::PrintStateInfo(void* node) 
{
	int x, y;
	NodeToXY( node, &x, &y );
	printf( "(%d,%d)", x, y );
}
