#ifndef __microTalker_h_
#define __microTalker_h_

#include "AI_Extension.h"
#include "micropather.h"
using namespace micropather;

class microTalker : Graph
{
  private:
	// Path and Map Vectors
	std::vector<void*> path;
	std::vector<std::vector<int>> gMap;

	// Structs
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

  public:
	microTalker(void);
	microTalker(std::vector<std::vector<int>>,int,int);
	virtual ~microTalker(void);

	// Functions
	int setGoalNode(int, int, int, int);
	int Passable(int nx, int ny);	
	std::vector<void*> returnPath(void);
	void ClearPath(void);			
	void NodeToXY(void* node, int* x, int* y);								
	void* XYToNode(int x, int y);
	virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd);	
	virtual void AdjacentCost(void* node, std::vector< StateCost > *neighbors); 
	virtual void PrintStateInfo( void* node );
	

};

#endif