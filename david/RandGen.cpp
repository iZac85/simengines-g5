
#include "RandGen.h"
//#include "math.h"
#include "stdlib.h"
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
	addWall(wall,pos);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y + arena.room[0].dim.y-1;
	wall.x=arena.room[0].dim.x-1;
	wall.y=0;
	addWall(wall,pos);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x;
	pos.y=arena.room[0].orig.y;
	wall.x=0;
	wall.y=arena.room[0].dim.y-1;
	addWall(wall,pos);
	/*-------------------------------------*/
	pos.x=arena.room[0].orig.x + arena.room[0].dim.x-1;
	pos.y=arena.room[0].orig.y;
	wall.x=0;
	wall.y=arena.room[0].dim.y-1;
	addWall(wall,pos);
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
		addWall(wall,pos);
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
void RandGen::addWall(struct tile len, struct tile pos)
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
void RandGen::doors(void)
{
	for(int k = 0; k < arena.room_cnt; k++)
	{
		for( int i = 0; i < arena.room[k].door.cnt ;i++)
		{
			int wallNum = randInt(0,3);
			//get wall in room from black tiles
			//is wall interior?
			//does it have a door allready?
			//rand pos
			//get wall in arena 
			//update wall in arena
		}
	}
	
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

