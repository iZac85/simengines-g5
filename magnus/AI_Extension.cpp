
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
using namespace std;
using namespace micropather;

// CONSTANTS
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

// Path Planning
int Pather::pathPlanning()
{

	return(mTalker->setGoalNode(robot.currentPos.x, robot.currentPos.y, goalNode.x, goalNode.y));
}

// Map and Path Stack
void Pather::setMap(std::vector<std::vector<int>> tempMap)
{
	// Map Size
	map.size.x = tempMap.size();
	map.size.y = tempMap[0].size();

	// Initialize map
	std::vector<int> rows (map.size.y,1);
	iMap = vector<vector<int>> (map.size.x,rows);

	// Copy Map
	iMap = tempMap;

	const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	for (int i = 1; i<map.size.x-1; i++)
	{
		for(int j = 1; j<map.size.y-1; j++)
		{
			for( int k=0; k<8; ++k ) 
			{
				int nx = i + dx[k];
				int ny = j + dy[k];

				if(tempMap[nx][ny] == 1)
				{
					iMap[i][j] = 1;
					break;
				}
			}
		}
	}

	tempMap = iMap;

	for (int i = 1; i<map.size.x-1; i++)
	{
		for(int j = 1; j<map.size.y-1; j++)
		{
			for( int k=0; k<8; ++k ) 
			{
				int nx = i + dx[k];
				int ny = j + dy[k];

				if(tempMap[nx][ny] == 1)
				{
					iMap[i][j] = 1;
					break;
				}
			}
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

// Starting Positions
Ogre::Vector3 Pather::randPlayerPos()
{
	bool pass = true;
	while(pass)
	{
		startPosX = rand()%(map.size.x+1);
		startPosY = rand()%(map.size.y+1);
		if (mTalker->Passable(startPosX,startPosY) == 1)
		{
			pass = false;
		}
	}

	return(Ogre::Vector3(startPosX,10,startPosY));
}
Ogre::Vector3 Pather::randNinjaPos()
{
	bool pass = true;
	while(pass)
	{
		startPosX = rand()%(map.size.x+1);
		startPosY = rand()%(map.size.y+1);
		if (mTalker->Passable(startPosX,startPosY) == 1)
		{
			pass = false;
		}
	}
	return(Ogre::Vector3(startPosX,0,startPosY));
}
Ogre::Vector3 Pather::centerOfMap()
{
	return(Ogre::Vector3(map.size.x/2,2,map.size.y/2));
}
bool Pather::inMap(int x, int y)
{
	if(x > -60 && y > -60 && x < map.size.x+60 && y < map.size.y+60)
		return true;
	else 
		return false;
}

// Fleeing Functions
void Pather::flee(int euclDist)
{
	if (euclDist < FASTFLEERADIUS && stuckFlag == 0)
	{
		fleeFast();
		ninjaSpeed = 30*AISPEED;
	}
	else if (euclDist < SLOWFLEERADIUS && stuckFlag == 0)
	{
		fleeSlow();
		ninjaSpeed = 20*AISPEED;
	}
	else if (stuckFlag == 0)
	{
		if (DONTFLEEACTIVE)
		{
			dontFlee();
			ninjaSpeed = 5*AISPEED;
		}
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

	 bool pass = true;

	 int maxX = min(robot.currentPos.x + DONTFLEEDIST,map.size.x);
	 int minX = max(robot.currentPos.x - DONTFLEEDIST,0);
	 int maxY = min(robot.currentPos.y + DONTFLEEDIST,map.size.y);
	 int minY = max(robot.currentPos.y - DONTFLEEDIST,0);

	 while(pass)
	 {
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

bool Pather::isStuck()
{

	// Keep the deck to size 10 elements
	if(stuckDeck.size() > 10)
		stuckDeck.pop_front();

	if (stuckFlag == 0)
	{
		int iter = 0;
		bool stuck = false;
		int i,j;
		for (i = 0; i<(int)stuckDeck.size(); i++)
		{
			iter = 0;
			for (j = 0; j<(int)stuckDeck.size(); j++)
			{
				int temp1 = (int)stuckDeck[i]/(int)stuckDeck[j];
				int temp2 = (int)stuckDeck[i];
				if((int)stuckDeck[i]/(int)stuckDeck[j] == 0)
					iter = iter+1;
				if(iter > 4)
				{
					stuck = true;
					break;
				}
			}	
			if(j<(int)stuckDeck.size())
				break;
		}
		if(stuck)
		{
			stuckFlag = SLOWFLEEDIST-2;
			fleeSlow();
			fillPathDeck();
			return true;
		}
	}

	return false;
}

// AI Initialization
void Pather::AIinit(std::vector<std::vector<int>> tempMapVector, int SFR, int FFR, int SFD, int FFD, int DFD, int AIS)
{

	SLOWFLEERADIUS = SFR;
	FASTFLEERADIUS = FFR;
	SLOWFLEEDIST = SFD;
	FASTFLEEDIST = FFD;
	DONTFLEEDIST = DFD;
	AISPEED = AIS;

	stuckFlag = 0;

	setMap(tempMapVector);

	// Rand
	srand(time(NULL));

	ninjaSpeed = 5;
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
	if ((pathDeck.empty() || euclDist < SLOWFLEERADIUS ) && stuckFlag == 0)	
	{
		
		flee(euclDist);
		isStuck();
		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		stuckDeck.push_back(pathDeck.front());
		pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));
	}
	else
	{
		if(stuckFlag>0)
			stuckFlag--;

		mTalker->NodeToXY( pathDeck.front(), &x, &y );
		stuckDeck.push_back(pathDeck.front());
		pathDeck.pop_front();
		return(Ogre::Vector3((float)x,0.0f,(float)y));
	}
}