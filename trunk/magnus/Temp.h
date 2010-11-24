#ifndef __Temp_h_
#define __Temp_h_

class Pather
{
public:
    Pather(void);
    ~Pather(void);
        void createMap(void);
        void getCurrentNode(void);
		static void readPos(void);

        struct vec3
        {
            float x;
            float y;
			float z;
        };
		struct currentNode
		{
			float vel;
			float angle;
			float angVel;
			vec3 pos;
        };
		struct goalNode
		{
			vec3 pos;
        };
		struct nextNode
		{
			vec3 pos;
        };

private:
		void getGoalNode(void);
		void setVel(void);
        void distToGoalNode(void);
		void printPos(void);
};

#endif