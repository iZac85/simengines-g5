
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
void RandGen::generateMap(int dim_x, int dim_y, int room_cnt, int room_min_x, int room_min_y, int door_cnt, bool furniture_enable, int desk_cnt, int chair_cnt, int shelf_cnt, int painting_cnt)
{
    //Fill structures
	
	//Dimensions
	map arena;
	arena.dim.x = dim_x;
	arena.dim.y = dim_y;
	arena.room_cnt = room_cnt;

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
	int wallPos;
	int MinWalLen; 
	tile wall;
	tile pos;
	//one room input
	for(int i=1; i<arena.room_cnt; i++)
	{
		//two rooms
		sortRooms(arena);
		tile min = getSmallestRoom();
		if(!sizeOk(min))
		{
			removeWall();
		}
		//biggest one room
		while(room<MIN)
		{
			int max = getBiggestRoom();
			if(dim.x > dim.y)
			{
				wallPos = randInt(0,dim.x);
				if(wallPos < MinWalLen || (dim.x - wallPos) < MinWalLen)
				{
					split(dim,orig);
				}
				else
				{
					wall.x = 0;
					wall.y = dim.y;
					pos.x = orig.x + wall.x;
					pos.y = orig.y;
					addWall(wall,pos);
				}
			}
			else if(y > x)
			{
	
			}
			else
			{
	
			}
		}//End while loop
	}//End for loop
	//om inte antal rum är tillräckligt många: split else: return
	//wile loop
}
int RandGen::randInt(int low, int high)
{
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



