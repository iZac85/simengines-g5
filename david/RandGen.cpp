
#include "RandGen.h"
#include "time.h"

//-------------------------------------------------------------------------------------
RandGen::RandGen(void)
{
	//map *arena;
	//arena = new map;//arena = new map;
}
//-------------------------------------------------------------------------------------
RandGen::~RandGen(void)
{
	//delete map;
}

//-------------------------------------------------------------------------------------
void RandGen::generateMap(int dim_x, int dim_y, int room_min_x, int room_min_y, int room_max_area, int door_cnt, bool furniture_enable, int desk_cnt, int chair_cnt, int shelf_cnt, int painting_cnt)
{
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
	}
}
void RandGen::split()
{ 
	int k;	//index to active room
	int wallPos=0;
	tile wall;
	tile pos;
	
	//Generate exterior walls
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y;
	wall.x=arena.room[0].dim.x-1;
	wall.y=0;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y + arena.room[0].dim.y-1;
	wall.x=arena.room[0].dim.x-1;
	wall.y=0;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y;
	wall.x=0;
	wall.y=arena.room[0].dim.y-1;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x + arena.room[0].dim.x-1;
	pos.y=arena.room[0].orig.y;
	wall.x=0;
	wall.y=arena.room[0].dim.y-1;
	addWall(wall,pos,1);
	/*-------------------------------------*/
	//Generate interior walls
	while(sizeOkForSplitting(getBiggestRoom()))//Is the biggest room too big? room with longest wall?
	{
		k = getBiggestRoom();
		wallPos = 0;
		//is x the longest wall or random selected?
		if(arena.room[k].dim.x > arena.room[k].dim.y || (arena.room[k].dim.x == arena.room[k].dim.y) && randInt(0,1)==1)
		{
			wallPos = randInt(0 + arena.room_min_size.x, arena.room[k].dim.x - arena.room_min_size.x);
			wall.x = 0;
			wall.y = arena.room[k].dim.y-1;
			pos.x = arena.room[k].orig.x + wallPos;
			pos.y = arena.room[k].orig.y;
		}
		//is y the longest wall or random selected?
		else
		{
			wallPos = randInt(0 + arena.room_min_size.y, arena.room[k].dim.y - arena.room_min_size.y);
			wall.x = arena.room[k].dim.x-1;
			wall.y = 0;
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
	
	if (len.x==0)
	{
		//if split in x
		tmp.dim.x = arena.room[k].orig.x + arena.room[k].dim.x - pos.x;
		tmp.dim.y = arena.room[k].dim.y;
		tmp.orig.x = pos.x;
		tmp.orig.y = arena.room[k].orig.y;

		arena.room[k].dim.x = pos.x - arena.room[k].orig.x + 1;
	}
	else if (len.y==0)
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
	srand ( time(NULL) );
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
	for(int i=0; i<=len.x; i++)		//x
	{
		for(int j=0; j<=len.y; j++)	//y
		{
			p.x=pos.x + i;
			p.y=pos.y + j;
			addBlackTile(p);
		}
	}
	if(len.y==0)		//wall in x direction?
	{
		wallTmp.dim.x =len.x;
		wallTmp.dim.y =1;
		wallTmp.pos.x =pos.x + (len.x/2);
		wallTmp.pos.y =pos.y;
	}
	else if(len.x==0)			//wall in y direction?
	{
		wallTmp.dim.x =1;
		wallTmp.dim.y =len.y;
		wallTmp.pos.x =pos.x;
		wallTmp.pos.y =pos.y + (len.y/2);
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
		if( ( arena.room[k].dim.x > ((arena.room_min_size.x*2)+1)) || (arena.room[k].dim.y > ((arena.room_min_size.y*2)+1) ) )//room not too small?
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
	int wallNum, doorsInRoom;
	tile doorDim, doorPos;
	computeWalls(k); //for rooms 
	doorsInRoom = 0;
	wallNum = randInt(0,arena.room[k].wallSegCnt-1);			//rand start wall
	int wallItr = 0;
	while (doorsInRoom <= arena.room[k].door.cnt && wallItr < arena.room[k].wallSegCnt) //loop through wall segments for room 
	{
		if(arena.room[k].wall[wallNum].type == 0) //only interior walls has doors			
		{
			if(wallEmpty(k,wallNum))			//does wall have a door already form black tiles?
			{
				//is wall in x or y?
				if(arena.room[k].wall[wallNum].dim.x == 1) //wall in y direction
				{
					doorPos.x = arena.room[k].wall[wallNum].pos.x;
					doorPos.y = randInt( arena.room[k].wall[wallNum].pos.y + 2, (arena.room[k].wall[wallNum].pos.y + arena.room[k].wall[wallNum].dim.y - 12) );
					doorDim.x = 1;
					doorDim.y = 10;		
				}
				else if(arena.room[k].wall[wallNum].dim.y == 1)	//wall in x direction
				{
					doorPos.x = randInt( arena.room[k].wall[wallNum].pos.x + 2, (arena.room[k].wall[wallNum].pos.x + arena.room[k].wall[wallNum].dim.x - 12) );
					doorPos.y = arena.room[k].wall[wallNum].pos.y;
					doorDim.x = 10;
					doorDim.y = 1;		
				}
				int index = getMapWallIndex(doorDim, doorPos);
				updateMapWalls(index, doorDim, doorPos);		//update walls loop through global walls and split them accordingly!
			}				//update black tiles in arena
			doorsInRoom++;
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
int RandGen::getMapWallIndex(tile dim, tile pos)
{
	for(int i=0; i < arena.wall_cnt; i++)		//x
	{
		//if( arena.wall[i].type == 0 )		//interior wall?
		//{
			if( arena.wall[i].dim.x == 1 && dim.x == 1 ) 	//wall in same directions? y direction
			{
				if( arena.wall[i].pos.x == pos.x )	//wall in same pos?
				{
					return i;
				}
			}
			else if( (arena.wall[i].dim.y == 1 && dim.y == 1) ) //x direction
			{
				if( arena.wall[i].pos.y == pos.y )	//wall in same pos?
				{
					return i;
				}
			}
		//}
		//else
		//{
		//	return i;
		//}
	}
}
void RandGen::computeWalls(int k)//Get walls of room
{
	wall wallTmp;
	arena.room[k].wallSegCnt = 0;

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
	wallTmp.pos.x = arena.room[k].orig.x + arena.room[k].dim.x-1;
	wallTmp.pos.y = arena.room[k].orig.y;
	wallTmp.type = getWallType(wallTmp.dim, wallTmp.pos);
	computeWallSegments(k,wallTmp.dim,wallTmp.pos,wallTmp.type);
	//arena.room[k].wall.push_back(wallTmp);

	wallTmp.dim.x = arena.room[k].dim.x;
	wallTmp.dim.y = 1;
	wallTmp.pos.x = arena.room[k].orig.x;
	wallTmp.pos.y = arena.room[k].orig.y + arena.room[k].dim.y-1;
	wallTmp.type = getWallType(wallTmp.dim, wallTmp.pos);	//is wall interior?
	computeWallSegments(k,wallTmp.dim,wallTmp.pos,wallTmp.type);
	//arena.room[k].wall.push_back(wallTmp);
}
void RandGen::computeWallSegments(int k, tile dim, tile pos, int type)
{
	wall wOne;

	wOne.pos = pos;
	wOne.dim  = dim;
	wOne.type = type;
	arena.room[k].wall.push_back(wOne);
	arena.room[k].wallSegCnt++;
	
	if(type==0) //only on interior walls
	{
		if(dim.x==1) //y direction
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

					arena.room[k].wall.back().dim.y = i - arena.room[k].wall.back().pos.y;

					arena.room[k].wall.push_back(wOne);
					arena.room[k].wallSegCnt++;
				}
			}
		}
		else if(dim.y==1) //x direction
		{
			for(int i=pos.x+1; i<pos.x + dim.x-1; i++)
			{
				if(arena.blackTile[pos.y+1][i]== 1 || arena.blackTile[pos.y-1][i] == 1)
				{
					wOne.pos.x = i;
					wOne.pos.y = arena.room[k].wall.back().pos.y;
					wOne.dim.x = arena.room[k].wall.back().pos.x + arena.room[k].wall.back().dim.x - i;
					wOne.dim.y = arena.room[k].wall.back().dim.y;
					wOne.type = arena.room[k].wall.back().type;

					arena.room[k].wall.back().dim.x = i - arena.room[k].wall.back().pos.x;

					arena.room[k].wall.push_back(wOne);
					arena.room[k].wallSegCnt++;
				}
			}
		}
	}
	/*else if(type==1) //only on interior walls
	{
		wOne.pos = pos;
		wOne.dim  = dim;
		wOne.type = type;
		arena.room[k].wall.push_back(wOne);
		arena.room[k].wallSegCnt++;
	}*/
}
void RandGen::computeWallJunction(void)
{

}
int RandGen::getWallType(tile dim, tile pos)
{
	return arena.wall[getMapWallIndex(dim, pos)].type;
	/*if(((pos.x == arena.dim.x) || (pos.x == 0)) && dim.x==1)
	{
		return 1;
	}
	else if(((pos.y == arena.dim.y) || (pos.y == 0)) && dim.y==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}*/
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
			if(arena.blackTile[i][j] == 0)
			{
				doorFound = true;
			}
		}
	}
	return !doorFound;
}
void RandGen::updateMapWalls(int k, tile dim, tile pos)
{
	wall wallTmp;		// temporary wall
	tile p;			// temporary tile
	for(int i=0; i<dim.x; i++)		//x
	{
		for(int j=0; j<dim.y; j++)	//y
		{
			p.x=pos.x + i;
			p.y=pos.y + j;
			removeBlackTile(p);
		}
	}
	//I do some thing wrong below!!!
	if(dim.y==1)		//wall in x direction?
	{
		wallTmp.dim.x = arena.wall[k].dim.x - (pos.x - arena.wall[k].pos.x + dim.x);
		wallTmp.dim.y =dim.y;
		wallTmp.pos.x =( (pos.x - arena.wall[k].pos.x) + dim.x ) + ( wallTmp.dim.x/2 ); //start + len /2
		wallTmp.pos.y =pos.y;

		arena.wall[k].dim.x = pos.x - arena.wall[k].pos.x;
	}
	else if(dim.x==1)			//wall in y direction?
	{
		wallTmp.dim.x = dim.x;
		wallTmp.dim.y = arena.wall[k].dim.y - (pos.y - arena.wall[k].pos.y + dim.y);
		wallTmp.pos.x =pos.x;
		wallTmp.pos.y =( (pos.y - arena.wall[k].pos.y) + dim.y ) + (wallTmp.dim.y/2); //start + len /2

		arena.wall[k].dim.y = pos.y - arena.wall[k].pos.y;
	}
	wallTmp.type = arena.wall[k].type;
	arena.wall.push_back(wallTmp);
	arena.wall_cnt++;
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


