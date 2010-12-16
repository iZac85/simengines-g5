
#ifndef __RandGen_h_
#define __RandGen_h_

#include "math.h"
#include <vector>

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
	struct posFloat
	{
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
		posFloat pos3D;
		posFloat dim3D;
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
	int randInt(int low, int high);
	int GetLongestWall(void);

private:
	void updateMapWalls(int wallIndex, int roomIndex, tile dim, tile pos);
	void computeWallSegments(int k, tile dim, tile pos, int type);
	void addWall(tile len, tile pos, int extWall);
	int getMapWallIndex(tile dim, tile pos);
	void addRoom(int k,tile len, tile pos);
	bool doorPossible(int k, int wallNum);
	int getWallType(tile dim, tile pos);
	bool wallEmpty(int k, int wallNum);
	bool sizeOkForSplitting(int k);
	void removeBlackTile(tile p);
	void compute3DWallPos(void);
	void addBlackTile(tile pos);
	int getBiggestRoom(void);
	void computeWalls(int k);
	void doors(int k);
	void split(void);
	//Furniture//
	void painting(void);
	void shelf(void);
	void chair(void);
	void desk(void);
	
	
	
};

#endif
