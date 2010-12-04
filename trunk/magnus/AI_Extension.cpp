
/************************************/
/*Magnus Andersson, 2010			*/
/*Chalmers University of Technology	*/
/*Simulaton Engines					*/
/*									*/
/*Path-Planning / Fleeing / AI		*/
/*OGRE 3D Extension					*/
/************************************/

// INCLUDE
#include "stdafx.h"
#include "AI_Extension.h"
// NAMESPACE
using namespace std;
using namespace micropather;


// CONSTANTS
// Critical radius within which a certain fleeing algorithm is called
int SLOWFLEERADIUS = 10;				// Flee slow radius
int FASTFLEERADIUS = 3;					// Flee fast radius
int DONTFLEERADIUS = 20;				// Dont flee radius
// How far away the goal node is set
int SLOWFLEEDIST = 10;					// Flee 10 steps away from current position
int FASTFLEEDIST = 1;					// Flee 1 step away from current position
bool updateGoalManual = true;			// A flag that lets you set the goal manually

// INSTANCES
Pather *mPather;
microTalker *mTalker;

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

// microTalker Communication Functions
void Pather::getPath()
{
	path = mTalker->returnPath();
	unsigned sizeB = path.size();
	for(int f=0; f<sizeB; ++f ) 
	{
//		printf("Path Index:%d %d %d ",(int)path[f], (int)path[f]/20, (int)path[f]%20);
//		printf("\n");
	}
	
}
void Pather::currentPos()
{
	// Set the current position
	char buf[ 256 ];
	printf("\nSet starting Node:# #\n");
	std::cin.getline( buf, 256 );
	int x, y;
	sscanf( buf, "%d %d", &x, &y );
	// Sets the current position for in miroTalker
	mTalker->setCurrentPos(x, y);
}

// Pather Functions
void Pather::getMap(std::vector<std::vector<int>> tempMap)
{
	for (int i = 0; i<map.size.x; i++)
	{
		for (int j = 0; j<map.size.y; j++)
		{
			if (tempMap[i][j] == 0)
				iMap[i][j] = 0;
		}
	}

	printMap();
	mTalker = new microTalker(iMap,map.size.x, map.size.y);
}
int Pather::updatePlayerPos()
{
	//player.currentPos.x = Current Robot X Position
	//player.currentPos.y = Current Robot Y Position
	return 0;
}
int Pather::updateRobotPos()
{
	//robot.currentPos.x = Current Robot X Position
	//robot.currentPos.y = Current Robot Y Position
	return 0;
}
int Pather::pathStatus()
{
	// x,y of last element in path
	int pathEndX, pathEndY;
	mTalker->NodeToXY(path.back(),&pathEndX, &pathEndY);
	// Returns 1 if current robot position is equal to the last element of the path vector.
	if (pathEndX == robot.currentPos.x && pathEndY == robot.currentPos.y)
		return 1;
	else
		return 0;
}
int Pather::updateGoalPos()
{
	int res;
	//int robX,robY;
	if (updateGoalManual == false)
	{
		// Set position of enemy later
		int enemyX, enemyY;
		// Sets the enemy position manual
		char buf[ 256 ];
		printf("\nSet enemy position# #\n");
		std::cin.getline( buf, 256 );
		sscanf( buf, "%d %d", &enemyX, &enemyY );

		// Sets the goal position automatically (avoidance)
		int result = flee(enemyX,enemyY);
	}
	else
	{
		// Sets the goal position manual
		char buf[ 256 ];
		printf("\nSet goal Node# #\n");
		std::cin.getline( buf, 256 );
		sscanf( buf, "%d %d", &robot.nextPos.x, &robot.nextPos.y );
	}
	//Call mTalker
	res = mTalker->SetPos(robot.nextPos.x, robot.nextPos.y );
	return res;
}

// Fleeing Functions
int Pather::flee(int enemyX, int enemyY)
{
	// Get the robots position
	int robX = mTalker->X();
	int robY = mTalker->Y();
	
	// Check if inside the critical radius, if inside flee fast else flee slow
	if (sqrt((float)((enemyX-robX)*(enemyX-robX) + (enemyY-robY)*(enemyY-robY))) < FASTFLEERADIUS)
	{
		// Temp set the robots position as goal
		goalNode.x = robX;
		goalNode.y = robY;
		// Flee fast
		return(fleeFast(enemyX, enemyY));
		
	}
	else if (sqrt((float)((enemyX-robX)*(enemyX-robX) + (enemyY-robY)*(enemyY-robY))) < SLOWFLEERADIUS)
	{
		// Temp set the robots position as goal
		goalNode.x = robX;
		goalNode.y = robY;
		// Flee slow
		return(fleeSlow(enemyX, enemyY));
	}
	else
	{
		// Dont flee
		return(dontFlee());
	}
}
int Pather::checkQuadrant(int robX, int robY, int enemyX, int enemyY)
{
	//  Quadrant
	//  4  3
	//  1  2

	if (enemyX < robX && enemyY < robY) // LeftButtom
		return 1;
	else if (enemyX > robX && enemyY < robY) // RightButtom
		return 2;
	else if (enemyX > robX && enemyY > robY) // RightTop 
		return 3;
	else if (enemyX < robX && enemyY > robY) // LeftTop
		return 4;
}
int Pather::fleeFast(int enemyX, int enemyY)
{
	// Flee fast
	for (int i; i<8;i++) // Loop through the surrounding neighbours
	{
		switch (i) 
		{
		// Flee Fast Map
		// -1, 1   0, 1  1, 1
		// -1, 0   0, 0  1, 0
		// -1,-1   0,-1  1,-1

			case 1 :	// -1,-1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
				}
				break;
			case 2 :	// -1,0
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
				}
				break;
			case 3 :	// 0,-1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
				}
				break;
			case 4 :	// -1,1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
				}
				break;
			case 5 :	// 1,-1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
				}
				break;
			case 6 :	// 0,1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
				}
				break;
			case 7 :	// 1,0
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
				}
				break;
			case 8 :	// 1,1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y - FASTFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x + FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x - FASTFLEEDIST;
						goalNode.y = goalNode.y + FASTFLEEDIST;
						break;
				}
				break;
		}
		if(mTalker->Passable(goalNode.x,goalNode.x))
		{
			// If new goalNode is updated and passable return 1
			return 1;
		}
	}
	// If the flee algorithm didnt find any good node return 0
	return 0;
}
int Pather::fleeSlow(int enemyX,int enemyY)
{
	// Flee slow
	for (int i; i<8;i++) // Loop through the surrounding neighbours
	{
	 // Rand between 0-100.
	 srand(time(NULL));
	 int randFive = rand()%10 + 5;	// random [5,15]
	 cout << "Rand" << rand()%5 << "\n";
		switch (i) 
		{
		// Flee Fast Map
		// -1, 1   0, 1  1, 1
		// -1, 0   0, 0  1, 0
		// -1,-1   0,-1  1,-1

			case 1 :	// -1,-1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
				}
				break;
			case 2 :	// -1,0
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
				}
				break;
			case 3 :	// 0,-1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
				}
				break;
			case 4 :	// -1,1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
				}
				break;
			case 5 :	// 1,-1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
				}
				break;
			case 6 :	// 0,1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
				}
				break;
			case 7 :	// 1,0
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
				}
				break;
			case 8 :	// 1,1
				switch(checkQuadrant(goalNode.x,goalNode.y,enemyX,enemyY))
				{
					// Flee Towards RightTop
					case 1 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftTop
					case 2 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y - SLOWFLEEDIST;
						break;
					// Flee Towards LeftBottom
					case 3 :
						goalNode.x = goalNode.x + SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
					// Flee Towards RightBottom
					case 4 :
						goalNode.x = goalNode.x - SLOWFLEEDIST;
						goalNode.y = goalNode.y + SLOWFLEEDIST;
						break;
				}
				break;
		}
		if(mTalker->Passable(goalNode.x,goalNode.x))
		{
			// If new goalNode is updated and passable return 1
			return 1;
		}
	}
	// If the flee algorithm didnt find any good node return 0
	return 0;
}
int Pather::dontFlee()
{
	 // Rand
	 srand(time(NULL));

	 for (int iter = 0; iter < 100; iter++)
	 {
		goalNode.x = (int)(map.size.x*(rand()%1));
		goalNode.y = (int)(map.size.y*(rand()%1));
		if (mTalker->Passable(goalNode.x,goalNode.y))
			// If a goal node is found and passable return 1
			return 1;
	 }
	 // Else return 0
	 return 0;
}

// Print Functions
void Pather::printRobotPos()
{
	cout << "X,Y:" << robot.currentPos.x << robot.currentPos.y << "\n";
}
void Pather::printPlayerPos()
{
	cout << "X,Y:" << player.currentPos.x << player.currentPos.y << "\n";
}
void Pather::printMap()
{
	for (int i=0; i<map.size.x; i++)
	{
		for (int j=0; j<map.size.y; j++)
		{
			cout << iMap[i][j] << " ";
		}
		cout << "\n";
	}	
}
void Pather::printPath()
{

	int addNr;
	for (int row=0; row<map.size.y; row++)
	{
		for (int col=0; col<map.size.x; col++)
		{
			
			addNr = 0;
			for (int f = 0; f<path.size();f++)
			{
				if (row == (int)path[f]%20 && col == (int)path[f]/20 && addNr == 0)
				{
					cout << " " << " ";
					addNr = 1;
				}
			}
			if (addNr == 0)
				cout << iMap[row][col] << " ";
		}
		cout << "\n";
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

// AI main Function
void Pather::AImain(std::vector<std::vector<int>> tempMap)
{
	/*
	 mapSet = 0;
	 
	 if (mapSet == 0)
		getMap();
	 updatePlayerPos();
	 updateRobotPos();
	 if(pathStatus())
		updateGoalPos();
	 return INFO (Struct with robotPos, velocity, angle, angularVelocity)
	 */
	
	// Get the map
	getMap(tempMap);
	
	//While
	bool done = false;
	int res = 0;
	while (!done)
	{
		currentPos();
		res = mPather->updateGoalPos();
		printf( "SetPos Returned %d\n", res);
		getPath();
		printPath();
	}
}

// AI test main function
void Pather::AIgo()
{
	const int MAPXin = 20;
	const int MAPYin = 20;

	int tempMapArray[MAPXin][MAPYin] =  {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
										{1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1},
										{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};


	std::vector<std::vector<int>> tempMapVector;
	std::vector<int> rows (MAPXin,5);
	tempMapVector = vector<vector<int>> (MAPYin,rows);

	for (int i = 0; i<MAPXin; i++)
	{
		for (int j = 0; j<MAPYin; j++)
		{
			if (tempMapArray[i][j] == 0)
				tempMapVector[i][j] = 0;
		}
	}


	mPather = new Pather(MAPXin,MAPYin);
	mPather->AImain(tempMapVector);
}

// Main main
int _tmain(int argc, _TCHAR* argv[])
{

	const int MAPXin = 20;
	const int MAPYin = 20;

	int tempMapArray[MAPXin][MAPYin] =  {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
										{1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1},
										{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
										{1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										{1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
										{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};


	std::vector<std::vector<int>> tempMapVector;
	std::vector<int> rows (MAPXin,5);
	tempMapVector = vector<vector<int>> (MAPYin,rows);

	for (int i = 0; i<MAPXin; i++)
	{
		for (int j = 0; j<MAPYin; j++)
		{
			if (tempMapArray[i][j] == 0)
				tempMapVector[i][j] = 0;
		}
	}


	mPather = new Pather(MAPXin,MAPYin);
	mPather->AImain(tempMapVector);
	
	return 0;
	
}