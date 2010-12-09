
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
	struct wall
	{
		tile pos;
		tile dim;	//m
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
		wall wall;
	};
	struct map
	{
		tile dim;
		int room_cnt;
		tile room_min_size;
		int room_max_area;
		int wall_cnt;
		std::vector<wall> wall;
		std::vector<room> room;
		std::vector<std::vector<int>> blackTile;//One or zero: One means obstacle
	};
	map arena;
	void generateMap(int dim_x=50, int dim_y=50, int room_min_x=4, int room_min_y=4, int room_max_area=200,int door_cnt=2, bool furniture_enable=0, int desk_cnt=1, int chair_cnt=1, int shelf_cnt=1, int painting_cnt=1);
	void updateBlackTiles(void);
	int randInt(int low, int high);
private:

	void split(void);
	void doors(void);
	void desk(void);
	void chair(void);
	void shelf(void);
	void painting(void);
	void addBlackTile(struct tile pos);
	void addWall(struct tile len, struct tile pos);
	bool sizeOkForSplitting(int k);
	int getBiggestRoom(void);
	void addRoom(int k,struct tile len, struct tile pos);
	void isWallsInterior(tile dim, tile pos);
	void wallHasDoor(void);
	void updateWalls(void);
	void removeBlackTile(void);
};

#endif
