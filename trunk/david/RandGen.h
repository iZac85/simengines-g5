
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
		//int x;
		//int y;
		float x;
		float y;
	};
	struct furniture
	{
		int cnt;
		tile pos;
	};
	struct wall
	{
		tile pos3D;
		tile pos;
		tile dim;	//m
		int type; //0 = interior wall, 1 = exterior wall
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
		std::vector<wall> wall;
		int wallSegCnt;
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
	void generateMap(int dim_x=150, int dim_y=150, int room_min_x=14, int room_min_y=14, int room_max_area=200,int door_cnt=2, bool furniture_enable=0, int desk_cnt=1, int chair_cnt=1, int shelf_cnt=1, int painting_cnt=1);
	void updateBlackTiles(void);
	int randInt(int low, int high);
private:

	void split(void);
	void doors(int k);
	void desk(void);
	void chair(void);
	void shelf(void);
	void painting(void);
	void addBlackTile(tile pos);
	void addWall(tile len, tile pos, int extWall);
	bool sizeOkForSplitting(int k);
	int getBiggestRoom(void);
	void addRoom(int k,tile len, tile pos);
	void computeWalls(int k);
	int getWallType(tile dim, tile pos);
	bool wallEmpty(int k, int wallNum);
	void updateMapWalls(int k, tile dim, tile pos);
	void removeBlackTile(tile p);
	int getMapWallIndex(tile dim, tile pos);
	void computeWallJunction(void);
	void computeWallSegments(int k, tile dim, tile pos, int type);
	bool doorPossible(int k, int wallNum);
	void deleteShortWalls(int k);
	void compute3DWallPos(void);
};

#endif
