#ifndef __AI_Extension_h_
#define __AI_Extension_h_


#include <vector>
#include "micropather.h"
#include <time.h>
#include <iostream>
#include "microTalker.h"
#include "Animation.h"
#include <algorithm>

class Pather
{
public:
	Pather();
    Pather(int,int);
    ~Pather(void);
		
		// Public Functions
		void AIinit(std::vector<std::vector<int>>, int, int, int, int, int, int);
		Ogre::Vector3 AIframe(int, int, int, int);
		Ogre::Vector3 randPlayerPos(void);
		Ogre::Vector3 randNinjaPos(void);
		Ogre::Vector3 centerOfMap(void);
		bool inMap(int,int);
		
		float ninjaSpeed;

private:

		// Variables
		std::vector<std::vector<int>> iMap;
		std::vector<void*> pathVector;
		std::deque<void*> pathDeck;
		int MAPXin;
		int MAPYin;
		int startPosX;
		int startPosY;

		int SLOWFLEERADIUS;
		int FASTFLEERADIUS;
		int SLOWFLEEDIST;
		int FASTFLEEDIST;
		int DONTFLEEDIST;
		int AISPEED;

		// Functions
		void setMap(std::vector<std::vector<int>>);
		int pathPlanning(void);
		void flee(int);
		int checkQuadrant(void);
		void fleeFast(void);
		void fleeSlow(void);
		void dontFlee(void);
		void fillPathDeck(void);
		bool lineOfSight(void);

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

};

#endif