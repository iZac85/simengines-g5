#ifndef __AI_Extension_h_
#define __AI_Extension_h_


class Pather
{
public:
    Pather(void);
    ~Pather(void);
		
		// Functions
        void getMap(int map[20][20]);
        void getCurrentNode(void);
		void readPos(void);

        struct vec3_
        {
            float x;
            float y;
			float z;
        };
		struct currentNode_
		{
			float vel;
			float angle;
			float angVel;
			vec3_ pos;
        };
		struct goalNode_
		{
			vec3_ pos;
        };
		struct nextNode_
		{
			vec3_ pos;
        };

		int gMap[20][20];

private:



		// Functions
		void getGoalNode(void);
		void setVel(void);
        void distToGoalNode(void);
		void printPos(currentNode_ currentNode);
		void printMap(int map[20][20]);
};

#endif