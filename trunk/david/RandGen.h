
#ifndef __RandGen_h_
#define __RandGen_h_

class RandGen
{
public:
    RandGen(void);
    ~RandGen(void);
	void generateMap(int dim_x=50, int dim_y=50, int room_min_x=4, int room_min_y=4, int door_cnt=2, bool furniture_enable=0, int desk_cnt=1, int chair_cnt=1, int shelf_cnt=1, int painting_cnt=1);
	void updateBlackTiles(void);
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
		tile blackTile[MAX_TILES];//MAXTILES];
	};
	struct map
	{
		tile dim;
		int room_cnt;
		tile room_min_size;
		int wall_cnt;
		room office[MAX_ROOMS];
	};

private:
	//#define MAXTILES 100;
	//void split(int x, int y);
	void split(map arena);
	int randInt(int low, int high);
	void doors(void);
	void desk(void);
	void chear(void);
	void shelf(void);
	void painting(void);
	void addBlackTile(tile pos);
	void addWall(tile len, tile pos);
	bool sizeOk(map arena,int k);
	int getBiggestRoom(map arena);
};

#endif
