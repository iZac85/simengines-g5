
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
	arena.dim.x = dim_x;	//funkar inte struct in struct???
	arena.dim.y = dim_y;
	arena.office[0].dim.x  = dim_x;
	arena.office[0].dim.y  = dim_y;
	arena.office[0].orig.x = 0;
	arena.office[0].orig.y = 0;
	arena.room_cnt         = 1;
	arena.room_min_size.x  = room_min_x;
	arena.room_min_size.y  = room_min_y;
	arena.room_max_area	= room_max_area;
	//Black tiles
	/*for(int i=0; i<MAX_TILES; i++)
	{
		for(int j=0; j<MAX_TILES; j++)
		{
			tile pos;
			pos.x=i;
			pos.y=j;
			addBlackTile(pos,arena);
		}
	}*/
	std::vector<int> rows (dim_x,0);
	arena.blackTile = std::vector<std::vector<int>> (dim_y,rows);
		//Generate Walls
	split(arena);
	//doors and Furniture 
	for (int i = 0; i < arena.room_cnt; i++)
	{
		arena.office[i].door.cnt = door_cnt;
		if (furniture_enable)
		{
				
			arena.office[i].desk.cnt = desk_cnt;
			arena.office[i].chair.cnt = chair_cnt;
			arena.office[i].shelf.cnt = shelf_cnt;
			arena.office[i].painting.cnt = painting_cnt;
		}
		else
		{
			arena.office[i].desk.cnt = 0;
			arena.office[i].chair.cnt = 0;
			arena.office[i].shelf.cnt = 0;
			arena.office[i].painting.cnt = 0;
		}
	}
}
void RandGen::split(struct map arena)
{ 
	room tmp_room[2];
	int k;	//index to active room
	int wallPos;
	tile wall;
	tile pos;
	
	//Generate exterior walls
	pos.x=arena.office[0].orig.x;
	pos.y=arena.office[0].orig.y;
	wall.x=arena.office[0].dim.x;
	wall.y=0;
	addWall(wall,pos,arena);
	/*-------------------------------------*/
	pos.x=arena.office[0].orig.x;
	pos.y=arena.office[0].orig.y + arena.office[0].dim.y;
	wall.x=arena.office[0].dim.x;
	wall.y=0;
	addWall(wall,pos,arena);
	/*-------------------------------------*/
	pos.x=arena.office[0].orig.x;
	pos.y=arena.office[0].orig.y;
	wall.x=0;
	wall.y=arena.office[0].dim.y;
	addWall(wall,pos,arena);
	/*-------------------------------------*/
	pos.x=arena.office[0].orig.x + arena.office[0].dim.x;
	pos.y=arena.office[0].orig.y;
	wall.x=0;
	wall.y=arena.office[0].dim.y;
	addWall(wall,pos,arena);

	//Generate interior walls
	while(sizeOk(arena, getBiggestRoom(arena)))//Is the biggest room too big? 
	{
		k = getBiggestRoom(arena);
		//is x the longest wall?
		if(arena.office[k].dim.x > arena.office[k].dim.y)
		{
			wallPos = randInt(0, arena.office[k].dim.x);
			//Room too small?
			if( !(wallPos < arena.room_min_size.x || (arena.office[k].dim.x - wallPos) < arena.room_min_size.x) )
			{
				wall.x = 0;
				wall.y = arena.office[k].dim.y;
				pos.x = arena.office[k].orig.x + wall.x;
				pos.y = arena.office[k].orig.y;
				addWall(wall,pos,arena);
				arena.room_cnt++;
			}
		}
		//is y the longest wall?
		else if(arena.office[k].dim.y > arena.office[k].dim.x)
		{
			wallPos = randInt(0, arena.office[k].dim.y);
			//Room too small?
			if( !(wallPos < arena.room_min_size.y || (arena.office[k].dim.y - wallPos) < arena.room_min_size.y) )
			{
				wall.y = 0;
				wall.x = arena.office[k].dim.y;
				pos.y = arena.office[k].orig.y + wall.y;
				pos.x = arena.office[k].orig.x;
				addWall(wall,pos,arena);
				arena.room_cnt++;
			}
		}
		//the walls are equal then!
		else
		{
			if(randInt(0,1)==1)//Flip a coine! Does x win?
			{
				wallPos = randInt(0, arena.office[k].dim.x);
				//Room too small?
				if( !(wallPos < arena.room_min_size.x || (arena.office[k].dim.x - wallPos) < arena.room_min_size.x) )
				{
					wall.x = 0;
					wall.y = arena.office[k].dim.y;
					pos.x = arena.office[k].orig.x + wall.x;
					pos.y = arena.office[k].orig.y;
					addWall(wall,pos,arena);
					arena.room_cnt++;
				}
			}
			//y winns
			else
			{
				wallPos = randInt(0, arena.office[k].dim.y);
				//Room too small?
				if( !(wallPos < arena.room_min_size.y || (arena.office[k].dim.y - wallPos) < arena.room_min_size.y) )
				{
					wall.y = 0;
					wall.x = arena.office[k].dim.y;
					pos.y = arena.office[k].orig.y + wall.y;
					pos.x = arena.office[k].orig.x;
					addWall(wall,pos,arena);
					arena.room_cnt++;
				}
			}
		}
	}//End while loop
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
void RandGen::addBlackTile(struct tile pos, struct map arena)
{
	arena.blackTile[pos.x][pos.y] = 1;
}
void RandGen::addWall(struct tile len, struct tile pos, struct map arena)
{
	tile p;
	if (len.x==0)
	{
		for(int i=0; i<len.y; i++)
		{
			p.x=pos.x;
			p.y=pos.y + i;
			addBlackTile(p, arena);
		}
	}
	else
	{
		for(int i=0; i<len.x; i++)
		{
			p.x=pos.x + i;
			p.y=pos.y;
			addBlackTile(p, arena);
		}
	}
}
bool RandGen::sizeOk(struct map arena, int k)
{
	//Is the biggest room too big?
	if( (arena.office[k].dim.x * arena.office[k].dim.y) < arena.room_max_area )return true;
	else return false;
}
int RandGen::getBiggestRoom(struct map arena)
{
	int biggest_area=0;
	int biggest_index=0;
	for(int i =0; i<arena.room_cnt; i++)
	{
		if( (arena.office[i].dim.x * arena.office[i].dim.y) > biggest_area)
		{
			biggest_index = i;
		}
	}
	return biggest_index;
}
///////////////Other stuff/////////////////////////
void RandGen::doors(void)
{
}
void RandGen::desk(void)
{
}
void RandGen::chear(void)
{
}
void RandGen::shelf(void)
{
}
void RandGen::painting(void)
{
}

