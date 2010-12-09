
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
int SLOWFLEERADIUS = 40;				// Flee slow radius
int FASTFLEERADIUS = 20;					// Flee fast radius
int DONTFLEERADIUS = 80;				// Dont flee radius
// How far away the goal node is set
int SLOWFLEEDIST = 10;					// Flee 10 steps away from current position
int FASTFLEEDIST = 1;					// Flee 1 step away from current position
bool dontFleeMode = true;

// INSTANCES
Pather *mPather;
microTalker *mTalker;
Animation *mOgreDemo = new Animation();

/*
1. if(mapSet == 0)
      getMap()
2. updatePlayerPos()
3. updateRobotPos()
4. if(pathStatus())
5.    updateGoalPos()
6. return INFO
*/

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
void Pather::pathPlanning()
{
	int res;
	res = mTalker->setGoalNode(robot.currentPos.x, robot.currentPos.y, goalNode.x, goalNode.y);
}

// Map and Path Stack
void Pather::setMap(std::vector<std::vector<int>> tempMap,int MAPXin, int MAPYin)
{

	map.size.x = MAPXin;
	map.size.y = MAPYin;

	// Initialize map
	//std::vector<int> rows (map.size.x,1);
	//iMap = vector<vector<int>> (map.size.y,rows);

	for (int i = 0; i<map.size.x; i++)
	{
		for (int j = 0; j<map.size.y; j++)
		{
			if (tempMap[i][j] == 0)
				iMap[i][j] = 0;
		}
	}

	//printMap();

	// Create new instance of microTalker
	mTalker = new microTalker(iMap,map.size.x, map.size.y);
}
void Pather::fillPathDeck()
{

	int x,y;
	pathDeck.clear();
	//pathVector.clear();
	pathVector = mTalker->returnPath();
	mTalker->NodeToXY( pathDeck.front(), &x, &y );
	for (int i = 1; i < pathVector.size(); i++)
	{
		pathDeck.push_back(pathVector[i]);
	}
}


// Fleeing Functions
void Pather::flee()
{
	/*
	// Check if inside the critical radius, if inside flee fast else flee slow
	if (sqrt((float)((player.currentPos.x-robot.currentPos.x)*(player.currentPos.x-robot.currentPos.x) + (player.currentPos.y-robot.currentPos.y)*(player.currentPos.y-robot.currentPos.y))) < FASTFLEERADIUS)
	{
		// Flee fast
		dontFleeMode = false;
		fleeFast();
		
	}
	else if (sqrt((float)((player.currentPos.x-robot.currentPos.x)*(player.currentPos.x-robot.currentPos.x) + (player.currentPos.y-robot.currentPos.y)*(player.currentPos.y-robot.currentPos.y))) < SLOWFLEERADIUS)
	{
		// Flee slow
		dontFleeMode = false;
		fleeSlow();
	}
	else
	{
		*/
		// Dont flee
		dontFleeMode = true;
		dontFlee();
	//}
	pathPlanning();
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
		if(mTalker->Passable(goalNode.x,goalNode.y))
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
	 // Rand between 0-100.
	//srand(time(NULL));
	// int randFive = rand()%10 + 5;	// random [5,15]
	// cout << "Rand" << rand()%5 << "\n";
		switch (i) 
		{
		// Flee Fast Map
		// -1, 1   0, 1  1, 1
		// -1, 0   0, 0  1, 0
		// -1,-1   0,-1  1,-1

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
		if(mTalker->Passable(goalNode.x,goalNode.y))
		{
			break;
		}
	}
	// If the flee algorithm didnt find any good node return 0
	//return 0;
}
void Pather::dontFlee()
{
	 bool pass = true;
	 while(pass)
	 {
		// Rand
		srand(time(NULL));
		goalNode.x = (int)(rand()%map.size.x);
		goalNode.y = (int)(rand()%map.size.y);
		if (mTalker->Passable(goalNode.x,goalNode.y))
		{
			// If a goal node is found and passable return 1
			pass = false;
		}
	 }
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
	std::vector<int> rows (MAPXin,1);
	iMap = vector<vector<int>> (MAPYin,rows);

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

	int x,y;
	// If path deck is empty calculate new path
	if (pathDeck.empty())	
	{
		flee();
		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		if(!pathDeck.empty())
			pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));
	}
/*
	// If player is within SLOWFLEERADIUS while the robot is in dontFlee mode
	else if (dontFleeMode && sqrt((float)((player.currentPos.x-robot.currentPos.x)*(player.currentPos.x-robot.currentPos.x) + (player.currentPos.y-robot.currentPos.y)*(player.currentPos.y-robot.currentPos.y))) < SLOWFLEERADIUS)
	{
		flee();
		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));
	}
	// If player is within FASTFLEERADIUS update calculate a new path
	else if (sqrt((float)((player.currentPos.x-robot.currentPos.x)*(player.currentPos.x-robot.currentPos.x) + (player.currentPos.y-robot.currentPos.y)*(player.currentPos.y-robot.currentPos.y))) < FASTFLEERADIUS)
	{
		flee();
		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));
	}
	*/
	// Else take the next position in path
	else
	{

		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		if (!pathDeck.empty())
			pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));

	}



}