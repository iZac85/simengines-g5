
/************************************/
/*Magnus Andersson, 2010			*/
/*Chalmers University of Technology	*/
/*Simulaton Engines					*/
/*									*/
/*Path-Planning / Avoidance / AI	*/
/*OGRE 3D Extension					*/
/************************************/

// INCLUDE
#include "stdafx.h"
#include "microTalker.h"
using namespace std;

MicroPather *micro;
Pather *myPather;

microTalker::microTalker()
{
}
microTalker::microTalker(std::vector<std::vector<int>> map, int MAPXin,int MAPYin)
{
	// Set the map size in a global struct
	mapG.size.x = MAPXin;
	mapG.size.y = MAPYin;

	

	// Initialize 2dVector
	std::vector<int> rows_ (mapG.size.x,5);
	gMap = vector<std::vector<int>> (mapG.size.y,rows_);

	// Copy the map to the class global map 2d vector
	for (int i=0; i<mapG.size.y; i++)
	{
		for (int j=0; j<mapG.size.x; j++)
		{
			gMap[i][j] = map[i][j];

		}
	}    
	rob.pos.x = 0;
	rob.pos.y = 0;
	micro = new MicroPather(this, 20);
	myPather = new Pather();


}
microTalker::~microTalker()
{
	delete micro;
}

void microTalker::setCurrentPos(int x, int y)
{
	rob.pos.x = x;
	rob.pos.y = y;
}
int microTalker::X() // Returns robotposX
{
	return rob.pos.x;
}	
int microTalker::Y()	// Returns robotposY
{
	return rob.pos.y;
}
std::vector<void*> microTalker::returnPath()
{
	return path;
}

unsigned microTalker::Checksum()
{
	return micro->Checksum();
}
void microTalker::ClearPath()	// Clear the computed path
{
	path.resize(0);
}
int microTalker::Passable(int nx, int ny)	// Is the node passable/free
{
	if (nx >= 0 && nx < mapG.size.x && ny >= 0 && ny < mapG.size.y)
	{
		if (gMap[nx][ny] == 0)
			return 1;
	}		
	return 0;
}

int microTalker::SetPos(int nx, int ny)	// Set new goal node
{

	int result = 0;
	if ( Passable( nx, ny ) == 1 )
	{
		// Print Current Pos
		printf("XYToNode(posX,posY) Index:%d \n",(int)XYToNode(rob.pos.x,rob.pos.y));
		printf("XYToNode(posX,posY) X:%d Y:%d \n",(int)XYToNode(rob.pos.x,rob.pos.y)/10, (int)XYToNode(rob.pos.x,rob.pos.y)%10);
		// Print Goal Pos
		printf("XTToNode(nx,ny) Index:%d \n",(int)XYToNode(nx,ny));
		printf("XTToNode(nx,ny) X:%d Y:%d \n",(int)XYToNode(nx,ny)/10, (int)XYToNode(nx,ny)%10);
		float totalCost;
		result = micro->Solve( XYToNode( rob.pos.x, rob.pos.y ), XYToNode( nx, ny ), &path, &totalCost );

		if ( result == MicroPather::SOLVED ) 
		{
			rob.pos.x = nx;
			rob.pos.y = ny;
		}
		printf( "Pather returned %d\n", result );
	}
	else
	{
		printf( "Not Passable!\n");
	}
	return result;
}
void microTalker::Print() 
{
	// Dont print
}
void microTalker::NodeToXY(void* node, int* x, int* y) // Goal node to Pos
{
	int index = (int)node;
	*y = index/mapG.size.x;
	*x = index - *y*mapG.size.x;
}
void* microTalker::XYToNode(int x, int y) // Pos to Goal node
{
	return (void*) (y*mapG.size.x + x);
}
float microTalker::LeastCostEstimate(void* nodeStart, void* nodeEnd) // Least Cost
{
	int xStart, yStart, xEnd, yEnd;
	NodeToXY( nodeStart, &xStart, &yStart );
	NodeToXY( nodeEnd, &xEnd, &yEnd );

	 // Compute the minimum path cost using distance measurement. It is possible
	 //	to compute the exact minimum path using the fact that you can move only 
	 //	on a straight line or on a diagonal, and this will yield a better result.
	
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
			if ( pass == 1) 
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