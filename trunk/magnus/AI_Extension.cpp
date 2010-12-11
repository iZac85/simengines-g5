
/************************************/
/*Magnus Andersson, 2010			*/
/*Chalmers University of Technology	*/
/*Simulaton Engines					*/
/*									*/
/*Path-Planning / Fleeing / AI		*/
/*OGRE 3D Extension					*/
/************************************/

// INCLUDE
#include "AI_Extension.h"
// NAMESPACE
using namespace std;
using namespace micropather;

// CONSTANTS
// Critical radius within which a certain fleeing algorithm is called
int SLOWFLEERADIUS = 80;				// Flee slow radius
int FASTFLEERADIUS = 20;					// Flee fast radius
// How far away the goal node is set
int SLOWFLEEDIST = 10;					// Flee 10 steps away from current position
int FASTFLEEDIST = 1;					// Flee 1 step away from current position
int DONTFLEEDIST = 100;
bool DONTFLEEACTIVE = true;

// INSTANCES
Pather *mPather;
microTalker *mTalker;
Animation *mOgreDemo = new Animation();

// Constructor/Deconstructor Functions
Pather::Pather()
{
}
Pather::Pather(int mapSizeX, int mapSizeY)
{

	map.size.x = mapSizeX;
	map.size.y = mapSizeY;

	// Initialize map
	std::vector<int> rows (map.size.x,5);
	iMap = vector<vector<int>> (map.size.y,rows);
}
Pather::~Pather(void)
{
}

// Path
int Pather::pathPlanning()
{

	return(mTalker->setGoalNode(robot.currentPos.x, robot.currentPos.y, goalNode.x, goalNode.y));
}

// Map and Path Stack
void Pather::setMap(std::vector<std::vector<int>> tempMap,int MAPXin, int MAPYin)
{

	map.size.x = MAPXin;
	map.size.y = MAPYin;

	for (int i = 0; i<map.size.x; i++)
	{
		for (int j = 0; j<map.size.y; j++)
		{
			if (tempMap[i][j] == 0)
				iMap[i][j] = 0;
		}
	}
	// Create new instance of microTalker
	mTalker = new microTalker(iMap,map.size.x, map.size.y);
}
void Pather::fillPathDeck()
{

	pathDeck.clear();
	pathVector = mTalker->returnPath();
	for (unsigned int i = 1; i < pathVector.size(); i++)
	{
		pathDeck.push_back(pathVector[i]);
	}
}

// Fleeing Functions
void Pather::flee(int euclDist)
{
	if (euclDist < FASTFLEERADIUS)
	{
		fleeFast();
	}
	else if (euclDist < SLOWFLEERADIUS)
	{
		fleeSlow();
	}
	else
	{
		if (DONTFLEEACTIVE)
			dontFlee();
		else
			fleeSlow();
	}
	fillPathDeck();
}
int Pather::checkQuadrant()
{
	//  Quadrant
	//  4  3
	//  1  2

	if (player.currentPos.x < robot.currentPos.x && player.currentPos.y < robot.currentPos.y) // LeftButtom
		return 1;
	else if (player.currentPos.x >= robot.currentPos.x && player.currentPos.y < robot.currentPos.y) // RightButtom
		return 2;
	else if (player.currentPos.x >= robot.currentPos.x && player.currentPos.y >= robot.currentPos.y) // RightTop 
		return 3;
	else // (player.currentPos.x < robot.currentPos.x && player.currentPos.y >= robot.currentPos.y) // LeftTop
		return 4;
}
void Pather::fleeFast()
{
	// Flee fast
	for (int i = 1; i<=8;i++) // Loop through the surrounding neighbours
	{
		switch (i) 
		{
		// Flee Fast Map
		// -1, 1   0, 1  1, 1
		// -1, 0   0, 0  1, 0
		// -1,-1   0,-1  1,-1

			case 1 :	// -1,-1w
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
				}
				break;
			case 2 :	// -1,0
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
				}
				break;
			case 3 :	// 0,-1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
				}
				break;
			case 4 :	// -1,1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
				}
				break;
			case 5 :	// 1,-1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
				}
				break;
			case 6 :	// 0,1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
				}
				break;
			case 7 :	// 1,0
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
				}
				break;
			case 8 :	// 1,1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x + FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x - FASTFLEEDIST;
						goalNode.y = robot.currentPos.y + FASTFLEEDIST;
						break;
				}
				break;
		}
		if(mTalker->Passable(goalNode.x,goalNode.y) == 1  && pathPlanning() == 0)
		{
			break;
		}
	}
}
void Pather::fleeSlow()
{
	// Flee slow
	for (int i = 1; i<=8;i++) // Loop through the surrounding neighbours
	{
		switch (i) 
		{
			case 1 :	// -1,-1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
				}
				break;
			case 2 :	// -1,0
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
				}
				break;
			case 3 :	// 0,-1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
				}
				break;
			case 4 :	// -1,1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
				}
				break;
			case 5 :	// 1,-1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
				}
				break;
			case 6 :	// 0,1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
				}
				break;
			case 7 :	// 1,0
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
				}
				break;
			case 8 :	// 1,1
				switch(checkQuadrant())
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = robot.currentPos.x + SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = robot.currentPos.x - SLOWFLEEDIST;
						goalNode.y = robot.currentPos.y + SLOWFLEEDIST;
						break;
				}
				break;
		}
		if(mTalker->Passable(goalNode.x,goalNode.y) == 1 && pathPlanning() == 0)
		{
			break;
		}
	}
}
void Pather::dontFlee()
{

	//fleeSlow();
	
	 bool pass = true;

	 int maxX = min(robot.currentPos.x + DONTFLEEDIST,map.size.x);
	 int minX = max(robot.currentPos.x - DONTFLEEDIST,0);
	 int maxY = min(robot.currentPos.y + DONTFLEEDIST,map.size.y);
	 int minY = max(robot.currentPos.y - DONTFLEEDIST,0);

	 while(pass)
	 {
		// Rand
		srand(time(NULL));
		goalNode.x = rand()%(maxX-minX+1) + minX;
		goalNode.y = rand()%(maxY-minY+1) + minY;	
		if (mTalker->Passable(goalNode.x,goalNode.y) == 1 && pathPlanning() == 0)
		{
			pass = false;
		}
	}
	

}
bool Pather::lineOfSight()
{
	return true;
}

// Other Functions
int Pather::sign(int v)
{
	// Return sign of int
	return v > 0 ? 1 : (v < 0 ? -1 : 0);
}
int Pather::odd(int v)
{
	// If even return 0
	if (v%2 == 0)
		return 0;
	// If odd return 1
	else
		return 1;
}

// AI Initialization
void Pather::AIinit(std::vector<std::vector<int>> tempMapVector)
{

	MAPXin = tempMapVector.size();
	MAPYin = tempMapVector[0].size();

	// Initialize map
	std::vector<int> rows (MAPYin,1);
	iMap = vector<vector<int>> (MAPXin,rows);

	setMap(tempMapVector,MAPXin,MAPYin);


}
// AI per Frame
Ogre::Vector3 Pather::AIframe(int robPosX, int robPosY, int playerPosX, int playerPosY)
{

	// Update Robot position
	robot.currentPos.x = robPosX;
	robot.currentPos.y = robPosY;
	// Update Player position
	player.currentPos.x = playerPosX; 
	player.currentPos.y = playerPosY;

	int euclDist = sqrt((float)((player.currentPos.x-robot.currentPos.x)*(player.currentPos.x-robot.currentPos.x) + (player.currentPos.y-robot.currentPos.y)*(player.currentPos.y-robot.currentPos.y)));

	int x,y;
	// If path deck is empty calculate new path
	if (pathDeck.empty() || euclDist < SLOWFLEERADIUS)	
	{
		flee(euclDist);
		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));
	}
	else
	{
		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));
	}
}