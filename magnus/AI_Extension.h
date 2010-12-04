#ifndef __AI_Extension_h_
#define __AI_Extension_h_


#include <vector>
#include "micropather.h"
#include <time.h>
#include <iostream>
#include "microTalker.h"

class Pather
{
public:
	Pather();
    Pather(int,int);
    ~Pather(void);
		
		// Functions
        void getMap(std::vector<std::vector<int>>);
		void currentPos(void);
		int updateGoalPos(void);
		void getPath(void);
		void printPath(void);
		void AImain(std::vector<std::vector<int>>);
		void AIgo(void);

		// Structs
        struct vec2_
        {
            int x;
            int y;
        };
		struct goalNode_
		{
			vec2_ pos;
        };
		struct robot_
		{
			vec2_ currentPos;
			vec2_ nextPos;
			vec2_ lastPos;
		};
		struct player_
		{
			vec2_ currentPos;
			vec2_ nextPos;
			vec2_ lastPos;
		};
		struct map_
		{
			vec2_ size;
		};

		vec2_ goalNode;
		map_ map;
		player_ player;
		robot_ robot;

private:

		// Variables
		std::vector<std::vector<int>> iMap;
		std::vector<void*> path;

		// Functions
		void printRobotPos(void);
		void printPlayerPos(void);
		void printMap(void);
		int flee(int, int);
		int checkQuadrant(int,int,int,int);
		int sign(int);
		int odd(int);
		int fleeFast(int,int);
		int fleeSlow(int,int);
		int dontFlee(void);
		int updatePlayerPos(void);
		int updateRobotPos(void);
		int pathStatus(void);

};

#endif