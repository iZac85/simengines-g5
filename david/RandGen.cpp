
#include "RandGen.h"

//-------------------------------------------------------------------------------------
RandGen::RandGen(void)
{
	//map arena;
	//arena = new map;
}
//-------------------------------------------------------------------------------------
RandGen::~RandGen(void)
{
	//delete map;
}

//-------------------------------------------------------------------------------------
void RandGen::generateMap(int dim_x, int dim_y, int room_min_x, int room_min_y, int door_cnt, bool furniture_enable, int desk_cnt, int chair_cnt, int shelf_cnt, int painting_cnt)
{
    //Fill structures
	
	//Dimensions
	map arena;
	arena.dim.x			   = dim_x;
	arena.dim.y			   = dim_y;
	arena.office[0].dim.x  = dim_x;
	arena.office[0].dim.y  = dim_y;
	arena.office[0].orig.x =	 0;
	arena.office[0].orig.y =	 0;
	arena.room_cnt         =	 1;

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
	//Generate Walls
	room room[MAX_ROOMS];
	tile origo;
	origo.x = 0;
	origo.y = 0;
	arena.office->orig = origo;
	split(arena);
}
void RandGen::split(map arena)
{ 
	room tmp_room[2];
	int k;	//index to active room
	int wallPos;
	tile wall;
	tile pos;
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
				addWall(wall,pos);
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
				addWall(wall,pos);
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
					addWall(wall,pos);
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
					addWall(wall,pos);
					arena.room_cnt++;
				}
			}
		}
	}//End while loop
}
int RandGen::randInt(int low, int high)
{
	return 0;
}
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
void RandGen::updateBlackTiles(void)
{
}
void RandGen::addBlackTile(tile pos)
{
}
void RandGen::addWall(tile len, tile pos)
{
}
bool RandGen::sizeOk(map arena, int k)
{
	return false;
}
int RandGen::getBiggestRoom(map arena)
{
	return 0;
}
