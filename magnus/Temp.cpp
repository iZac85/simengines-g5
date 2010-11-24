

#include "stdafx.h"
#include "Temp.h"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

Pather::currentNode hej;

//-------------------------------------------------------------------------------------
Pather::Pather(void)
{
}
//-------------------------------------------------------------------------------------
Pather::~Pather(void)
{
}

//-------------------------------------------------------------------------------------
void Pather::createMap()
{
}
void Pather::getCurrentNode()
{ 
}
void Pather::getGoalNode()
{
}

void setVel()
{
}

void Pather::distToGoalNode(void)
{
}

void Pather::readPos()
{

	string mystr;
	cout << "Next Position: X\n";
	getline (cin,mystr);
	stringstream(mystr) >> hej.pos.x;
	cout << "Next Position: Y\n";
	getline (cin,mystr);
	stringstream(mystr) >> hej.pos.y;
	cout << "Next Position: Z\n";
	getline (cin,mystr);
	stringstream(mystr) >> hej.pos.z;

	cout << hej.pos.x << "\n";
}

void Pather::printPos()
{
}




int _tmain(int argc, _TCHAR* argv[])
{
	Pather::readPos();
	Pather::readPos();
	
}






