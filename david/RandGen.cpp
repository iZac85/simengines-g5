
#include "RandGen.h"
#include "time.h"

RandGen::RandGen(void)
{
	//map *arena;
	//arena = new map;//arena = new map;
}
RandGen::~RandGen(void)
{
	//delete map;
}
void RandGen::generateMap(int dim_x, int dim_y, int room_min_x, int room_min_y, int room_max_area, int door_cnt, bool furniture_enable, int desk_cnt, int chair_cnt, int shelf_cnt, int painting_cnt)
{
    srand ( (unsigned int) time(NULL) ); //Get seed for randomizer
	//Fill structures
	
	//Dimensions
	room tmp;
	arena.wall_cnt=0;
	arena.dim.x = dim_x;
	arena.dim.y = dim_y;
	tmp.dim.x  = dim_x;
	tmp.dim.y  = dim_y;
	tmp.orig.x = 0;
	tmp.orig.y = 0;
	arena.room.push_back(tmp);
	arena.room_cnt         = 1;
	arena.room_min_size.x  = room_min_x;
	arena.room_min_size.y  = room_min_y;
	arena.room_max_area	= room_max_area;
	
	//Black tiles
	std::vector<int> rows (dim_y,0);
	arena.blackTile = std::vector<std::vector<int>> (dim_x,rows);
	//Generate Walls
	split();
	//doors and Furniture 
	for (int i = 0; i < arena.room_cnt; i++)
	{
		arena.room[i].door.cnt = door_cnt;
		if (furniture_enable)
		{
				
			arena.room[i].desk.cnt = desk_cnt;
			arena.room[i].chair.cnt = chair_cnt;
			arena.room[i].shelf.cnt = shelf_cnt;
			arena.room[i].painting.cnt = painting_cnt;
		}
		else
		{
			arena.room[i].desk.cnt = 0;
			arena.room[i].chair.cnt = 0;
			arena.room[i].shelf.cnt = 0;
			arena.room[i].painting.cnt = 0;
		}
		doors(i);
		//compute3DWallPos();
	}
	//doors(0);
	compute3DWallPos();
}
void RandGen::split()
{ 
	int k;	//index to active room
	int min,max;
	int wallPos=0;
	tile wall;
	tile pos;
	
	//Generate exterior walls
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y;
	wall.x=arena.room[0].dim.x;
	wall.y=1;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y + arena.room[0].dim.y-1;
	wall.x=arena.room[0].dim.x;
	wall.y=1;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y;
	wall.x=1;
	wall.y=arena.room[0].dim.y;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x + arena.room[0].dim.x-1;
	pos.y=arena.room[0].orig.y;
	wall.x=1;
	wall.y=arena.room[0].dim.y;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	//Generate interior walls
	while(sizeOkForSplitting(getBiggestRoom()))//Is the biggest room too big? room with longest wall? //Wrong????
	{
		k = getBiggestRoom();
		wallPos = 0;
		//is x the longest wall or random selected?
		if(arena.room[k].dim.x > arena.room[k].dim.y || (arena.room[k].dim.x == arena.room[k].dim.y) && randInt(0,1) == 1)
		{
			min = (int)(0 + arena.room_min_size.x -1) ;
			max = (int)(arena.room[k].dim.x - arena.room_min_size.x);
			if(min>max)
			{
				int b=0;
			}
			wallPos = randInt(min,max);
			wall.x = 1;
			wall.y = arena.room[k].dim.y;
			if(wall.y<14)
			{
				int bu =9;
			}
			pos.x = arena.room[k].orig.x + wallPos;
			pos.y = arena.room[k].orig.y;
		}
		//is y the longest wall or random selected?
		else
		{
			min = (int)(0 + arena.room_min_size.y - 1);
			max = (int)(arena.room[k].dim.y - arena.room_min_size.y);
			if(min>max)
			{
				int y = 0;
			}
			wallPos = randInt(min,max);
			wall.x = arena.room[k].dim.x;
			wall.y = 1;
			if(wall.x<14)
			{
				int bu = 9;
			}
			pos.x = arena.room[k].orig.x;
			pos.y = arena.room[k].orig.y + wallPos;
		}
		addWall(wall,pos,0);
		addRoom(k,wall,pos);
	}	//End while loop
}
void RandGen::addRoom(int k,struct tile len, struct tile pos)
{
	room tmp;
	
	if (len.x == 1)
	{
		//if split in x
		tmp.dim.x = arena.room[k].orig.x + arena.room[k].dim.x - pos.x;
		tmp.dim.y = arena.room[k].dim.y;
		tmp.orig.x = pos.x;
		tmp.orig.y = arena.room[k].orig.y;

		arena.room[k].dim.x = pos.x - arena.room[k].orig.x + 1;
	}
	else if (len.y == 1)
	{
		//if split in y
		tmp.dim.x =  arena.room[k].dim.x;
		tmp.dim.y =  arena.room[k].orig.y + arena.room[k].dim.y - pos.y;
		tmp.orig.x = arena.room[k].orig.x;
		tmp.orig.y = pos.y;

		arena.room[k].dim.y = pos.y - arena.room[k].orig.y + 1;
	}

	arena.room.push_back(tmp);
	arena.room_cnt++;
	//build walls in room: wall[4]: len 
}
int RandGen::randInt(int low, int high)
{
	int r = rand() % (high - low + 1) + low;
	return r;
}
void RandGen::updateBlackTiles(void)
{
	//From Physics
}
void RandGen::addBlackTile(struct tile pos)
{
	arena.blackTile[pos.x][pos.y] = 1;
}
void RandGen::addWall(struct tile len, struct tile pos, int extWall)
{
	tile p;			// temporary tile
	wall wallTmp;		// temporary wall
	for(int i=0; i < len.x; i++)		//x
	{
		for(int j=0; j < len.y; j++)	//y
		{
			p.x=pos.x + i;
			p.y=pos.y + j;
			addBlackTile(p);
		}
	}
	if(len.y == 1)		//wall in x direction?
	{
		wallTmp.dim.x =len.x;
		wallTmp.dim.y =1;
		wallTmp.pos.x =pos.x;// + (len.x/2);
		wallTmp.pos.y =pos.y;// + 0.5;
	}
	else if(len.x == 1)			//wall in y direction?
	{
		wallTmp.dim.x =1;
		wallTmp.dim.y =len.y;
		wallTmp.pos.x =pos.x;// + 0.5;
		wallTmp.pos.y =pos.y;// + (len.y/2);
	}
	wallTmp.type = extWall;
	arena.wall.push_back(wallTmp);
	arena.wall_cnt++;
}
bool RandGen::sizeOkForSplitting(int k)
{
	//Is the biggest room too big?
	//Size is ok for splitting if the room is bigger than the biggest allowed room
	if( (arena.room[k].dim.x * arena.room[k].dim.y) > arena.room_max_area )
	{
		//if( ( arena.room[k].dim.x > ((arena.room_min_size.x*2)+1)) || (arena.room[k].dim.y > ((arena.room_min_size.y*2)+1) ) )//room not too small?
		if( ( arena.room[k].dim.x >= ( (arena.room_min_size.x*2)-1) ) || ( arena.room[k].dim.y >= ( (arena.room_min_size.y*2)-1) ) )//room not too small?
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else 
	{
		return false;
	}
}
int RandGen::getBiggestRoom()
{
	int biggest_area=0;
	int biggest_index=0;
	for(int i =0; i<arena.room_cnt; i++)
	{
		if( (arena.room[i].dim.x * arena.room[i].dim.y) > biggest_area)
		{
			biggest_index = i;
			biggest_area = arena.room[i].dim.x * arena.room[i].dim.y;
		}
	}
	return biggest_index;
}
///////////////Other stuff/////////////////////////
void RandGen::doors(int k)
{
	// jag skriver över väggar när jag gör ett nytt rym!!!!
	int wallNum, doorsInRoom;
	int min, max;
	tile doorDim, doorPos;
	computeWalls(k); //for rooms 
	doorsInRoom = 0;
	wallNum = randInt(0,arena.room[k].wallSegCnt-1);			//rand start wall
	int wallItr = 0;
	while (doorsInRoom <= arena.room[k].door.cnt && wallItr < arena.room[k].wallSegCnt) //loop through wall segments for room 
	{
		if(arena.room[k].wall[wallNum].type == 0) //only interior walls has doors			
		{
			if(doorPossible(k,wallNum))			//Is wall big enough?
			{
				if(wallEmpty(k,wallNum))			//does wall have a door already form black tiles?
				{
					//is wall in x or y?
					if(arena.room[k].wall[wallNum].dim.x == 1) //wall in y direction
					{
						doorPos.x = arena.room[k].wall[wallNum].pos.x;
						min = (int)(arena.room[k].wall[wallNum].pos.y + 2);
						max = (int)(arena.room[k].wall[wallNum].pos.y + arena.room[k].wall[wallNum].dim.y - 12);
						if(min>max)
						{
							int lkj = 0;
						}
						doorPos.y = randInt(min,max);
						doorDim.x = 1;
						doorDim.y = 10;		
					}
					else if(arena.room[k].wall[wallNum].dim.y == 1)	//wall in x direction
					{
						min = (int)(arena.room[k].wall[wallNum].pos.x + 2);
						max = (int)(arena.room[k].wall[wallNum].pos.x + arena.room[k].wall[wallNum].dim.x - 12);
						if(min>max)
						{
							int iqpp = 0;
						}
						doorPos.x = randInt(min,max);
						doorPos.y = arena.room[k].wall[wallNum].pos.y;
						doorDim.x = 10;
						doorDim.y = 1;		
					}
					int wallIndex = getMapWallIndex(doorDim, doorPos);
					updateMapWalls(wallIndex, k, doorDim, doorPos);		//update walls loop through global walls and split them accordingly!
				}				//update black tiles in arena
				doorsInRoom++;
			}
		}
		if((wallNum+1) >= arena.room[k].wallSegCnt)
		{
			wallNum = 0;
		}
		else
		{
			wallNum = wallNum + 1;
		}
		wallItr++;
	}
}
bool RandGen::doorPossible(int k,int wallNum)
{
	if(arena.room[k].wall[wallNum].dim.y==1) // x direction
	{
		if(arena.room[k].wall[wallNum].dim.x >= arena.room_min_size.x)
		{

			return true;
		}
	}
	else if(arena.room[k].wall[wallNum].dim.x==1)	// y direction
	{
		if(arena.room[k].wall[wallNum].dim.y >= arena.room_min_size.y)
		{

			return true;
		}
	}
	return false;
}
int RandGen::getMapWallIndex(tile dim, tile pos)
{
	for(int i=0; i < arena.wall_cnt; i++)
	{
		if( arena.wall[i].dim.x == 1 && dim.x == 1 ) 	//wall in same directions? y direction
		{
			if( arena.wall[i].pos.x == pos.x )	//wall in same pos?
			{
				for(int j = arena.wall[i].pos.y; j < (arena.wall[i].pos.y + arena.wall[i].dim.y); j++)
				{
					if(j==pos.y)
					{
						return i;
					}
				}
			}
		}
		else if( (arena.wall[i].dim.y == 1 && dim.y == 1) ) //x direction
		{
			if( arena.wall[i].pos.y == pos.y )	//wall in same pos?
			{
				for(int j = arena.wall[i].pos.x; j < (arena.wall[i].pos.x + arena.wall[i].dim.x); j++)
				{
					if(j==pos.x)
					{
						return i;
					}
				}
			}
		}
	}
	return 42; //How many walls must a ninja walk through, before he will find any doors? 
}
void RandGen::computeWalls(int k)//Get walls of room
{
	wall wallTmp;
	arena.room[k].wallSegCnt = 0;
	arena.room[k].wall.clear();

	wallTmp.dim.x = 1;		
	wallTmp.dim.y = arena.room[k].dim.y;		
	wallTmp.pos.x = arena.room[k].orig.x;	
	wallTmp.pos.y = arena.room[k].orig.y;	
	wallTmp.type = getWallType(wallTmp.dim, wallTmp.pos);
	computeWallSegments(k,wallTmp.dim,wallTmp.pos,wallTmp.type);
	//arena.room[k].wall.push_back(wallTmp);

	wallTmp.dim.x = arena.room[k].dim.x;
	wallTmp.dim.y = 1;
	wallTmp.pos.x = arena.room[k].orig.x;
	wallTmp.pos.y = arena.room[k].orig.y;
	wallTmp.type = getWallType(wallTmp.dim, wallTmp.pos);
	computeWallSegments(k,wallTmp.dim,wallTmp.pos,wallTmp.type);
	//arena.room[k].wall.push_back(wallTmp);

	wallTmp.dim.x = 1;
	wallTmp.dim.y = arena.room[k].dim.y;
	wallTmp.pos.x = arena.room[k].orig.x + arena.room[k].dim.x - 1;
	wallTmp.pos.y = arena.room[k].orig.y;
	wallTmp.type = getWallType(wallTmp.dim, wallTmp.pos);
	computeWallSegments(k,wallTmp.dim,wallTmp.pos,wallTmp.type);
	//arena.room[k].wall.push_back(wallTmp);

	wallTmp.dim.x = arena.room[k].dim.x;
	wallTmp.dim.y = 1;
	wallTmp.pos.x = arena.room[k].orig.x;
	wallTmp.pos.y = arena.room[k].orig.y + arena.room[k].dim.y - 1;
	wallTmp.type = getWallType(wallTmp.dim, wallTmp.pos);	//is wall interior?
	computeWallSegments(k,wallTmp.dim,wallTmp.pos,wallTmp.type);
	//arena.room[k].wall.push_back(wallTmp);

	//deleteShortWalls(k);
}
void RandGen::deleteShortWalls(int k)
{
	for(int i=0; i<arena.room[k].wallSegCnt; i++)
	{
		if(!doorPossible(k,i))
		{
			//arena.room[k].wall.erase(i);
		}
	}
}
void RandGen::computeWallSegments(int k, tile dim, tile pos, int type)
{
	wall wOne;

	wOne.pos = pos;
	wOne.dim  = dim;
	wOne.type = type;
	arena.room[k].wall.push_back(wOne);
	arena.room[k].wallSegCnt++;
	
	if(type == 0) //only on interior walls
	{ 
		if(dim.x == 1) //y direction
		{
			for(int i=pos.y+1; i<pos.y + dim.y-1; i++)
			{
				if(arena.blackTile[pos.x+1][i]== 1 || arena.blackTile[pos.x-1][i] == 1)
				{
					wOne.pos.x = arena.room[k].wall.back().pos.x;
					wOne.pos.y = i;//arena.room[k].wall.back().pos.y + i;
					wOne.dim.x = arena.room[k].wall.back().dim.x;
					wOne.dim.y = arena.room[k].wall.back().pos.y + arena.room[k].wall.back().dim.y - i;
					wOne.type = arena.room[k].wall.back().type;

					arena.room[k].wall.back().dim.y = i - arena.room[k].wall.back().pos.y + 1;//???

					arena.room[k].wall.push_back(wOne);
					arena.room[k].wallSegCnt++;
				}
			}
		}
		else if(dim.y == 1) //x direction
		{
			for(int i=pos.x+1; i<pos.x + dim.x-1; i++)
			{
				if(arena.blackTile[i][pos.y+1]== 1 || arena.blackTile[i][pos.y-1] == 1)
				{
					wOne.pos.x = i;
					wOne.pos.y = arena.room[k].wall.back().pos.y;
					wOne.dim.x = arena.room[k].wall.back().pos.x + arena.room[k].wall.back().dim.x - i;
					wOne.dim.y = arena.room[k].wall.back().dim.y;
					wOne.type = arena.room[k].wall.back().type;

					arena.room[k].wall.back().dim.x = i - arena.room[k].wall.back().pos.x + 1;//???

					arena.room[k].wall.push_back(wOne);
					arena.room[k].wallSegCnt++;
				}
			}
		}
	}
}
int RandGen::getWallType(tile dim, tile pos)
{
	return arena.wall[getMapWallIndex(dim, pos)].type;
}
bool RandGen::wallEmpty(int k, int wallNum)
{
	bool doorFound = false;
	tile p;			// temporary tile
	for(int i=0; i < arena.room[k].wall[wallNum].dim.x; i++)		//x
	{
		for(int j=0; j < arena.room[k].wall[wallNum].dim.y; j++)	//y
		{
			p.x=arena.room[k].wall[wallNum].pos.x + i;
			p.y=arena.room[k].wall[wallNum].pos.y + j;
			if(arena.blackTile[p.x][p.y] == 0)
			{
				doorFound = true;
			}
		}
	}
	return !doorFound;
}
void RandGen::updateMapWalls(int wallIndex, int roomIndex, tile dim, tile pos)
{
	wall wallTmp;		// temporary wall
	tile p;			// temporary tile
	for(int i=0; i < dim.x; i++)		//x
	{
		for(int j=0; j < dim.y; j++)	//y
		{
			p.x=pos.x + i;
			p.y=pos.y + j;
			removeBlackTile(p);
		}
	}
	//int u = 0;
	//I do some thing wrong below!!!
	if(dim.y == 1)		//wall in x direction?
	{
		if(arena.wall[wallIndex].dim.x > arena.room[roomIndex].dim.x)
		{
			int gh=0;
		}
		wallTmp.dim.x = arena.wall[wallIndex].dim.x - (pos.x - arena.wall[wallIndex].pos.x + dim.x);
		wallTmp.dim.y = arena.wall[wallIndex].dim.y;
		wallTmp.pos.x = pos.x + dim.x;
		wallTmp.pos.y = arena.wall[wallIndex].pos.y;// + 0.5;

		arena.wall[wallIndex].dim.x = pos.x - arena.wall[wallIndex].pos.x;

		wallTmp.type = arena.wall[wallIndex].type;
		arena.wall.push_back(wallTmp);
		arena.wall_cnt++;
	}
	else if(dim.x == 1)			//wall in y direction?
	{
		if(arena.wall[wallIndex].dim.y > arena.room[roomIndex].dim.y)
		{
			int hg=0;
		}
		wallTmp.dim.x = arena.wall[wallIndex].dim.x;
		wallTmp.dim.y = arena.wall[wallIndex].dim.y - (pos.y - arena.wall[wallIndex].pos.y + dim.y);
		wallTmp.pos.x = arena.wall[wallIndex].pos.x;// + 0.5;
		wallTmp.pos.y = pos.y + dim.y;
		
		arena.wall[wallIndex].dim.y = pos.y - arena.wall[wallIndex].pos.y;
		
		wallTmp.type = arena.wall[wallIndex].type;
		arena.wall.push_back(wallTmp);
		arena.wall_cnt++;
	
	}

}
void RandGen::removeBlackTile(tile p)
{
	arena.blackTile[p.x][p.y] = 0;
}
void RandGen::desk(void)
{

}
void RandGen::chair(void)
{

}
void RandGen::shelf(void)
{

}
void RandGen::painting(void)
{

}
void RandGen::compute3DWallPos(void)
{
	tile pStart, pEnd;
	
	for(int i=4; i<arena.wall_cnt; i++)		//Start on interior walls
	{
		if(i==100) 
		{
			int rt=0;
		}
		pStart = arena.wall[i].pos;
		pEnd.x = arena.wall[i].pos.x + arena.wall[i].dim.x - 1;
		pEnd.y = arena.wall[i].pos.y + arena.wall[i].dim.y - 1;

		if (arena.wall[i].dim.y == 1) // x direction
		{

			if((arena.blackTile[pStart.x][pStart.y+1]==1 || arena.blackTile[pStart.x][pStart.y-1]==1) && (arena.blackTile[pEnd.x][pEnd.y+1]==1 || arena.blackTile[pEnd.x][pEnd.y-1]==1))
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x - 2;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
				arena.wall[i].pos3D.x = (float)arena.wall[i].pos.x + (arena.wall[i].dim3D.x/2) +1;
				arena.wall[i].pos3D.y = (float)(arena.wall[i].pos.y + 0.5);
			}
			else if((arena.blackTile[pStart.x][pStart.y+1]==1 || arena.blackTile[pStart.x][pStart.y-1]==1) )
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x - 1;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
				arena.wall[i].pos3D.x = (float)arena.wall[i].pos.x + (arena.wall[i].dim3D.x/2) + 1;
				arena.wall[i].pos3D.y = (float)(arena.wall[i].pos.y + 0.5);
			}	
			else if( (arena.blackTile[pEnd.x][pEnd.y+1]==1 || arena.blackTile[pEnd.x][pEnd.y-1]==1) )
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x - 1;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
				arena.wall[i].pos3D.x = (float)arena.wall[i].pos.x + (arena.wall[i].dim3D.x/2);// + 0.5;
				arena.wall[i].pos3D.y = (float)(arena.wall[i].pos.y + 0.5);
			}
			else
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
				arena.wall[i].pos3D.x = (float)arena.wall[i].pos.x + ((float)arena.wall[i].dim.x/2);
				arena.wall[i].pos3D.y = (float)(arena.wall[i].pos.y + 0.5);
			}
		}
		else if (arena.wall[i].dim.x == 1) // y direction
		{
			if((arena.blackTile[pStart.x+1][pStart.y]==1 || arena.blackTile[pStart.x-1][pStart.y]==1) && (arena.blackTile[pEnd.x+1][pEnd.y]==1 || arena.blackTile[pEnd.x-1][pEnd.y]==1))
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y - 2;
				arena.wall[i].pos3D.x = (float)(arena.wall[i].pos.x + 0.5);
				arena.wall[i].pos3D.y = (float)arena.wall[i].pos.y + (arena.wall[i].dim3D.y/2) +1;
			}
			else if( arena.blackTile[pStart.x+1][pStart.y]==1 || arena.blackTile[pStart.x-1][pStart.y]==1 )
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y -1;
				arena.wall[i].pos3D.x = (float)(arena.wall[i].pos.x + 0.5);
				arena.wall[i].pos3D.y = (float)arena.wall[i].pos.y + (arena.wall[i].dim3D.y/2) + 1;
			}
			else if( arena.blackTile[pEnd.x+1][pEnd.y]==1 || arena.blackTile[pEnd.x-1][pEnd.y]==1)
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y -1;
				arena.wall[i].pos3D.x = (float)(arena.wall[i].pos.x + 0.5);
				arena.wall[i].pos3D.y = (float)arena.wall[i].pos.y + (arena.wall[i].dim3D.y/2);// + 0.5;
			}
			else
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
				arena.wall[i].pos3D.x = (float)(arena.wall[i].pos.x + 0.5);
				arena.wall[i].pos3D.y = (float)arena.wall[i].pos.y + (arena.wall[i].dim3D.y/2);
			}
		}
	}
	for(int i=0; i<4; i++)		//Start on interior walls
	{

		if (arena.wall[i].dim.y == 1) // x direction
		{
			arena.wall[i].pos3D.x = (float)arena.wall[i].pos.x + ((float)arena.wall[i].dim.x/2);
			arena.wall[i].pos3D.y = (float)(arena.wall[i].pos.y + 0.5);	
			
			if(i<2)
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x -2;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
			}
			else
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
			}
		}
		else if (arena.wall[i].dim.x == 1) // y direction
		{
			arena.wall[i].pos3D.x = (float)(arena.wall[i].pos.x + 0.5);
			arena.wall[i].pos3D.y = (float)arena.wall[i].pos.y + ((float)arena.wall[i].dim.y/2);
			
			if(i<2)
			{
				arena.wall[i].dim3D.x = (float)(arena.wall[i].dim.x);
				arena.wall[i].dim3D.y = (float)(arena.wall[i].dim.y -2);
			}
			else
			{
				arena.wall[i].dim3D.x = (float)arena.wall[i].dim.x;
				arena.wall[i].dim3D.y = (float)arena.wall[i].dim.y;
			}
		}
	}
}
int RandGen::GetLongestWall(void)
{
		
	return 4;
}
