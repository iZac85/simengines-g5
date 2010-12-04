
#ifndef __RandGen_h_
#define __RandGen_h_

#include "math.h"
#include <vector>
//using namespace std;

class RandGen
{
public:
	RandGen(void);
	~RandGen(void);
	static const int MAX_TILES = 200;
	static const int MAX_ROOMS = 100;
	//#define MAX_TILES 200;
	//#define MAX_ROOMS 100;
	struct tile
	{
		int x;
		int y;
	};
	struct furniture
	{
		int cnt;
		tile pos;
	};
	struct room
	{
		furniture door;
		furniture desk;
		furniture chair;
		furniture shelf;
		furniture painting;
		tile dim;
		tile orig;
		//tile blackTile[MAX_TILES][MAX_TILES];//MAXTILES];
	};
	struct map
	{
		tile dim;
		int room_cnt;
		tile room_min_size;
		int room_max_area;
		int wall_cnt;
		room office[MAX_ROOMS];
		std::vector<std::vector<int>> blackTile;//One or zeroo: One means obstable
	};
	

	map arena;
	//map arena;
	void generateMap(int dim_x=50, int dim_y=50, int room_min_x=4, int room_min_y=4, int room_max_area=25,int door_cnt=2, bool furniture_enable=0, int desk_cnt=1, int chair_cnt=1, int shelf_cnt=1, int painting_cnt=1);
	//void generateMap(struct map *arena, int dim_x, int dim_y, int room_min_x, int room_min_y, int room_max_area,int door_cnt, bool furniture_enable, int desk_cnt, int chair_cnt, int shelf_cnt, int painting_cnt);
	//void generateMap();
	void updateBlackTiles(void);
private:
	//#define MAXTILES 100;
	//void split(int x, int y);
	void split(struct map arena);
	int randInt(int low, int high);
	void doors(void);
	void desk(void);
	void chear(void);
	void shelf(void);
	void painting(void);
	void addBlackTile(struct tile pos, struct map arena);
	void addWall(struct tile len, struct tile pos, struct map arena);
	bool sizeOk(struct map arena,int k);
	int getBiggestRoom(struct map arena);
};

#endif
