#ifndef __AI_Extension_h_
#define __AI_Extension_h_


#include <vector>
#include "micropather.h"
#include <time.h>
#include <iostream>
#include "microTalker.h"
#include "Animation.h"

class Pather
{
public:
	Pather();
    Pather(int,int);
    ~Pather(void);
		
		// Functions
        void setMap(std::vector<std::vector<int>>,int,int);
		int pathPlanning(void);
		void printPath(void);
		void AIinit(std::vector<std::vector<int>>);
		Ogre::Vector3 AIframe(int, int, int, int);

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

		vec2_ vec2;
		vec2_ goalNode;
		map_ map;
		player_ player;
		robot_ robot;

private:

		// Variables
		std::vector<std::vector<int>> iMap;
		std::vector<void*> pathVector;
		std::deque<void*> pathDeck;

		// Functions
		void printRobotPos(void);
		void printPlayerPos(void);
		void printMap(void);
		void flee(void);
		int checkQuadrant(void);
		int sign(int);
		int odd(int);
		void fleeFast(void);
		void fleeSlow(void);
		void dontFlee(void);
		void fillPathDeck(void);

		int MAPXin;
		int MAPYin;

};

#endif