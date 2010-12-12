
/************************************/
/*Magnus Andersson, 2010			*/
/*Chalmers University of Technology	*/
/*Simulaton Engines					*/
/*									*/
/*Path-Planning / Avoidance / AI	*/
/*OGRE 3D Extension					*/
/************************************/

// INCLUDE
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
	std::vector<int> rows_ (mapG.size.y,1);
	gMap = vector<std::vector<int>> (mapG.size.x,rows_);

	// Copy Map
	gMap = map;

	// New instance of micropather (A* Path-Planning) and the Pather class
	micro = new MicroPather(this, 10);
	myPather = new Pather();


}
microTalker::~microTalker()
{
	delete micro;
}

// Path Computation
int microTalker::setGoalNode(int x, int y, int nx, int ny)	// Set new goal node
{
	rob.pos.x = x;
	rob.pos.y = y;

	ClearPath();

	int result = 1;
	if ( Passable( nx, ny ) == 1 )
	{
		float totalCost;
		result = micro->Solve( XYToNode( rob.pos.x, rob.pos.y ), XYToNode( nx, ny ), &path, &totalCost );
	}
	return result;
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

// Path Functions
std::vector<void*> microTalker::returnPath()
{
	return path;
}
void microTalker::ClearPath()	// Clear the computed path
{
	path.resize(0);
}
void microTalker::NodeToXY(void* node, int* x, int* y) // Goal node to Pos
{
	int index = (int)node;
	*y = index/mapG.size.x;
	*x = index - *y*mapG.size.x;
}
void* microTalker::XYToNode(int x, int y)
{
	return (void*) (y*mapG.size.x + x);
}

// Cost Functions
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