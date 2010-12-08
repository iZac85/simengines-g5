
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
	arena.dim.x = dim_x;
	arena.dim.y = dim_y;
	tmp.dim.x  = dim_x;
	tmp.dim.y  = dim_y;
	tmp.orig.x = 0;
	tmp.orig.y = 0;
	arena.office.push_back(tmp);
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
	split();
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
void RandGen::split()
{ 
	int k;	//index to active room
	int wallPos=0;
	tile wall;
	tile pos;
	
	//Generate exterior walls
	pos.x=arena.office[0].orig.x;
	pos.y=arena.office[0].orig.y;
	wall.x=arena.office[0].dim.x-1;
	wall.y=0;
	addWall(wall,pos);
	/*-------------------------------------*/
	pos.x=arena.office[0].orig.x;
	pos.y=arena.office[0].orig.y + arena.office[0].dim.y-1;
	wall.x=arena.office[0].dim.x-1;
	wall.y=0;
	addWall(wall,pos);
	/*-------------------------------------*/
	pos.x=arena.office[0].orig.x;
	pos.y=arena.office[0].orig.y;
	wall.x=0;
	wall.y=arena.office[0].dim.y-1;
	addWall(wall,pos);
	/*-------------------------------------*/
	pos.x=arena.office[0].orig.x + arena.office[0].dim.x-1;
	pos.y=arena.office[0].orig.y;
	wall.x=0;
	wall.y=arena.office[0].dim.y-1;
	addWall(wall,pos);
	/*-------------------------------------*/
	//Generate interior walls
	while(sizeOkForSplitting(getBiggestRoom()))//Is the biggest room too big? room with longest wall?
	//for(int i=0;i<5;i++)
	{
		k = getBiggestRoom();
		wallPos = 0;
		//is x the longest wall or random selected?
		if(arena.office[k].dim.x > arena.office[k].dim.y || (arena.office[k].dim.x = arena.office[k].dim.y) && randInt(0,1)==1)
		{
			while( (wallPos < arena.room_min_size.x || (arena.office[k].dim.x - wallPos) < arena.room_min_size.x) )
			{
				wallPos = randInt(0, arena.office[k].dim.x);
			}
			wall.x = 0;
			wall.y = arena.office[k].dim.y-1;
			pos.x = arena.office[k].orig.x + wallPos;
			pos.y = arena.office[k].orig.y;
		}
		//is y the longest wall or random selected?
		else
		{
			while( (wallPos < arena.room_min_size.y || (arena.office[k].dim.y - wallPos) < arena.room_min_size.y) )
			{
				wallPos = randInt(0, arena.office[k].dim.y);
			}
			wall.x = arena.office[k].dim.x-1;
			wall.y = 0;
			pos.x = arena.office[k].orig.x;
			pos.y = arena.office[k].orig.y + wallPos;
		}
		//if(!((wallPos < arena.room_min_size.x || (arena.office[k].dim.x - wallPos) < arena.room_min_size.x) ) || ((wallPos < arena.room_min_size.y || (arena.office[k].dim.y - wallPos) < arena.room_min_size.y) ))//Room too small?
		//if( !(wallPos < ) )
		//{
			addWall(wall,pos);
			addRoom(k,wall,pos);
		//}
	}	//End while loop
}
void RandGen::addRoom(int k,struct tile len, struct tile pos)
{
	room tmp;
	
	if (len.x==0)
	{
		//if split in x
		tmp.dim.x = arena.office[k].dim.x - pos.x;
		tmp.dim.y = arena.office[k].dim.y;
		tmp.orig.x = pos.x;
		tmp.orig.y = arena.office[k].orig.y;

		arena.office[k].dim.x = pos.x - arena.office[k].orig.x;
	}
	else if (len.y==0)
	{
		//if split in y
		tmp.dim.x = arena.office[k].dim.x;
		tmp.dim.y = arena.office[k].dim.y - pos.y;
		tmp.orig.x = arena.office[k].orig.x;
		tmp.orig.y =pos.y;

		arena.office[k].dim.y = pos.y - arena.office[k].orig.y;
	}

	arena.office.push_back(tmp);
	arena.room_cnt++;
}
int RandGen::randInt(int low, int high)
{
	srand ( time(NULL) );
	int r = rand() % (high - low + 1) + low;
	return r;
	//return 0;
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
	tile p;
	for(int i=0; i<=len.x; i++)		//x
	{
		for(int j=0; j<=len.y; j++)	//y
		{
			p.x=pos.x + i;
			p.y=pos.y + j;
			if( p.x>49 ||p.y>49)
			{
				int ost=2;
			}
			addBlackTile(p);
		}
	}
}
bool RandGen::sizeOkForSplitting(int k)
{
	//Is the biggest room too big?
	//Size is ok for splitting if the room is bigger than the biggest allowed room
	if( (arena.office[k].dim.x * arena.office[k].dim.y) > arena.room_max_area )
	{
		if( ( arena.office[k].dim.x > (arena.room_min_size.x*2+1)) || (arena.office[k].dim.y > (arena.room_min_size.y*2+1) ) )//room not too small?
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
		if( (arena.office[i].dim.x * arena.office[i].dim.y) > biggest_area)
		{
			biggest_index = i;
			biggest_area = arena.office[i].dim.x * arena.office[i].dim.y;
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

