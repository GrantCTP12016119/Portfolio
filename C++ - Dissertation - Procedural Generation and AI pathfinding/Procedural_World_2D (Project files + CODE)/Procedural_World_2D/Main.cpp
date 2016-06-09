#include "DarkGDK.h"
#include <stdlib.h>   
#include <time.h>      

#include "pcg.h" // please see top line of this library

//constants
const double PI = 3.1415926535;
const int F = 60; // frequency of gradients (1 gradient per x block)
const int GTC = 3; // grids to calculate (squared)
const int GRX = 60; //grid resolution x
const int GRY = 60; //grid resolution y

const int CALCX = F * (GTC+1); // sample points to calculate
const int CALCY = F * (GTC+1);

int VIEWX_START = F; // where the view begins (here its one lattice across and down) 
int VIEWY_START = F;

int VIEWX_END = F;//F*2;
int VIEWY_END = F;//F*2;

float getDistance(int x1, int y1, int x2, int y2)
{
	float distanceX = x1-x2;
	distanceX *= distanceX;
	float distanceY = y1-y2;
	distanceY *= distanceY;

	float distance = sqrtf(distanceX+distanceY);		
	return distance;
}


float getDiff(int num1, int num2)
{
	float diff = num1-num2;
	diff*=diff;
	return sqrtf(diff);
}

//SPRITE SIZE
const int SS = 12;
const int SSX = 12;
const int SSY = 12;

int spriteCounter = 500;

//A* vars and structs

struct node
{
	int x,y; // cords on screen or waypoint
	int h; // heuristic, distance from goal node
	int g; // movement cost
	int pher; // pheromone level
	//int f; // h + g
	int parentX, parentY; // cords in array of parent node
	char flag; // n = null, o = open, c = closed
	int order; // the lower this number the earlier it is in the path
	bool hasPheromone;
};

node nodes[CALCX][CALCY]; // everyone needs to know about the nodes

#include "Terrain.h" // terrain needs to know about the nodes
Terrain *terrain;
#include "Agent.h" // agent needs to know about the terrain and nodes
Agent *agent1;
#include "Panel.h" // panel needs to know about the agent and terrain for buttons to affect them
Panel *panel;

// the main entry point for the application is this function
void DarkGDK ( void )
{
	dbSetDisplayMode(720,700,0);

	//initilaisations
	//sprite loading
	dbLoadImage("textures/grasslight.jpg", 1);
	dbLoadImage("textures/deepwater_12.bmp", 2);
	dbLoadImage("textures/shallowwater_12.bmp", 3);
	dbLoadImage("textures/dirt.bmp", 4);
	dbLoadImage("textures/grass1.jpg", 5);
	dbLoadImage("textures/pathNode.png", 10);
	dbLoadImage("textures/starman2.png", 100);
	dbLoadImage("textures/blockLand.png", 101);
	dbLoadImage("textures/blockWater.png", 102);
	dbLoadImage("textures/underWater.png", 200);
	dbLoadImage("textures/pheromone.png", 300);
	agent1 = new Agent(100, 200);
	terrain = new Terrain();
	panel = new Panel(0, dbScreenHeight() - 150);

	dbSprite(1, -32,-32, 1);
	dbSprite(2, -32,-32, 2);
	dbSprite(3, -32,-32, 3);
	dbSprite(4, -32,-32, 4);
	dbSprite(5, -32,-32, 5);
	dbSprite(10, -32,-32, 10);

	dbSizeSprite(1,SS,SS);
	dbSizeSprite(2,SS,SS);
	dbSizeSprite(3,SS,SS);
	dbSizeSprite(4,SS,SS);
	dbSizeSprite(5,SS,SS);
	dbSizeSprite(10,SS,SS);

	//generate a world seed
	terrain->generateWorldSeed();
	// turn on sync rate and set maximum rate to 30 fps
	dbSyncOn   ( );
	dbSyncRate ( 30 );

	//record number of gradients
	int gradsView;

	bool mousePrevClicked = false;

	//initialise node struct
	agent1->clearNodes(true);

	// our main loop
	while ( LoopGDK ( ) )
	{
		dbCLS();

		double av = 0;

		//calculate gradients
		for(int i = 0; i < CALCX; i++)
		{
			for(int j = 0;j < CALCY;j++)
			{
				terrain->g[i][j] = 0.0;
			}
		}

		terrain->perlin(F, 20); // octave 1
		terrain->perlin(20, 10); // octave 2
		terrain->perlin(10, 5); // octave 3
		terrain->perlin(5, 2.5); // octave 4
		terrain->drawTerrain();
		agent1->update();
		agent1->draw();

		gradsView = 0;
		for(int i = VIEWX_START;i<VIEWY_END;i++)
		{
			for(int j = VIEWY_START;j<VIEWY_END;j++)
			{
				if((i+terrain->offsetX) % F == 0 && (j+terrain->offsetY) % F == 0)
				{
					gradsView++;
				}
			}
		}

		if(dbMouseY() > dbScreenHeight() - 150)
		{	
			panel->draw();
		}

		// update the screen
		
		dbSync ( );

		if ( dbMouseClick() == 1)
		{
			if(!mousePrevClicked)
			{
				panel->checkCollision(dbMouseX(), dbMouseY());
				mousePrevClicked = true;
			}
		}
		else
		{
			if(mousePrevClicked)
			{
				mousePrevClicked = false;
			}
		}

		bool buttonPressed = false;
		if(dbKeyState(205))
		{
			//offsetX++;
			terrain->posX++;
			terrain->viewOffsetX++;
			buttonPressed = true;
		}
		if(dbKeyState(208))
		{
			//offsetY++;
			terrain->posY++;
			terrain->viewOffsetY++;
			buttonPressed = true;
		}
		if(dbKeyState(203))
		{
			if(terrain->posX > 0)
			{
				//offsetX--;
				terrain->posX--;
				terrain->viewOffsetX--;
			}
			buttonPressed = true;
		}
		if(dbKeyState(200))
		{
			if(terrain->posY > 0)
			{
				//offsetY--;
				terrain->posY--;
				terrain->viewOffsetY--;
			}
			buttonPressed = true;
		}
		if(buttonPressed)
		{
			agent1->clearNodes(false, true);
		}
		if(terrain->viewOffsetX >= F)
		{
			terrain->viewOffsetX = 0;
		}
		if(terrain->viewOffsetX < 0)
		{
			terrain->viewOffsetX = F-1;
		}
		if(terrain->viewOffsetY >= F)
		{
			terrain->viewOffsetY = 0;
		}
		if(terrain->viewOffsetY < 0)
		{
			terrain->viewOffsetY = F-1;
		}

		if(buttonPressed)
		{
			dbWait(0);
			buttonPressed = false;
		}
	}
	// return back to windows
	delete agent1;
	delete terrain;
	delete panel;
	return;
}

