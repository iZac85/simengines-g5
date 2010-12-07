#ifndef __microTalker_h_
#define __microTalker_h_

#include "AI_Extension.h"
#include "micropather.h"
using namespace micropather;

class microTalker : Graph
{
  private:
	// Path Vector
	std::vector<void*> path;
	struct vec2_
        {
            int x;
            int y;
        };
	struct rob_
		{
			vec2_ pos;
		};
	struct mapG_
		{
			vec2_ size;
		};

	rob_ rob;
	mapG_ mapG;

	std::vector<std::vector<int>> gMap;

  public:
	microTalker(void);
	microTalker(std::vector<std::vector<int>>,int,int);
	virtual ~microTalker(void);

	// Functions
	int X(void);
	int Y(void);
	unsigned Checksum(void);
	void ClearPath(void);			// Clear the computed path
	void ToggleTouched(void);		// Toggle show considered on/off
	void ToggleDoor(void);			// Open/Close Doors
	int Passable(int nx, int ny);	// Is the node passable/free
	int setGoalNode(int, int, int, int);		// Set new goal node
	void NodeToXY(void* node, int* x, int* y);									// Goal node to Pos
	void* XYToNode(int x, int y);	// Pos to Goal node
	virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd);			// Least Cost
	virtual void AdjacentCost(void* node, std::vector< StateCost > *neighbors); // Adjecent Cost
	virtual void PrintStateInfo( void* node );
	std::vector<void*> returnPath(void);
	

};

#endif