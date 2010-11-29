#ifndef __microTalker_h_
#define __microTalker_h_

#include "micropather.h"
using namespace micropather;

class microTalker : Graph
{
  private:
	// Variables
	int posX, posY;
	bool doorsOpen;
	bool showConsidered;
	std::vector<void*> path;


	// Other
	microTalker(const microTalker&);
	void operator=(const microTalker&);


  public:
	microTalker(int intMap[20][20]);
	virtual ~microTalker(void);

	int gMap[20][20];

	// Functions
	int X(void);	// Returns posX
	int Y(void);	// Returns posY
	unsigned Checksum(void);
	void ClearPath(void);	// Clear the computed path
	void ToggleTouched(void); // Toggle show considered on/off
	void ToggleDoor(void); // Open/Close Doors
	int Passable(int nx, int ny);	// Is the node passable/free
	int SetPos(int nx, int ny);	// Set new goal node
	void Print(void); // Print
	void NodeToXY(void* node, int* x, int* y); // Goal node to Pos
	void* XYToNode(int x, int y); // Pos to Goal node
	virtual float LeastCostEstimate(void* nodeStart, void* nodeEnd); // Least Cost
	virtual void AdjacentCost(void* node, std::vector< StateCost > *neighbors); // Adjecent Cost
	virtual void PrintStateInfo( void* node );
	// Variables

	// Other
	

};

#endif