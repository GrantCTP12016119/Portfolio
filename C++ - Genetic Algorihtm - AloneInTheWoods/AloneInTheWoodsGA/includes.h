#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include "DarkGDK.h"

void spawnItem(std::string type, int X, int Y, int lifeTime = 30);

float getDistance(int x1, int y1, int x2, int y2)
{
	float distanceX = x1-x2;
	distanceX *= distanceX;
	float distanceY = y1-y2;
	distanceY *= distanceY;

	float distance = sqrtf(distanceX+distanceY);		
	return distance;
}

struct Waypoint
{
	int x;
	int y;
};

struct AgentData
{
	int AI[100];

	Waypoint chopping;
	Waypoint hunting;
	Waypoint building;

	int score;
};