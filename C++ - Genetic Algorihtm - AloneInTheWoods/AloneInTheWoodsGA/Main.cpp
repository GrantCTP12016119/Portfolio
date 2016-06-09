
// Dark GDK - The Game Creators - www.thegamecreators.com

// the wizard has created a very simple project that uses Dark GDK
// it contains the basic code for a GDK application
#include "includes.h"
// whenever using Dark GDK you must ensure you include the header file

int spriteCounter = 1;
int mapSprite;
const int agentImageId = 40;
const int agentDeathImageId = 41;

const int mapImageId = 10;
const int treeImageId = 11;
const int treePlantImageId = 12;
const int treeStumpImageId = 13;

const int woodImageId = 21;
const int waterImageId = 22;
const int foodImageId = 23;

const int rabbitImageId = 31;
const int lionImageId = 32;

const int hutImageId = 50;

const int huntWaypointImageId = 100;
const int buildWaypointImageId = 101;
const int chopWaypointImageId = 102;

const int MAXNUMOFAGENTS = 1;
const int MAXNUMOFTREES = 10;
const int MAXNUMOFITEMS = 10;
const int MAXNUMOFPREY = 6;
const int MAXNUMOFPREDATORS = 0;
const int MAXNUMOFHUTS = 2;

const int AGENTSPERGEN = 4;


#include "Item.h"
#include "Tree.h"
#include "Prey.h"
#include "Hut.h"
#include "Agent.h"
#include "Predator.h"

//Agent *agent1;
Agent *agents;
Tree *trees;
Item *items;
Prey *prey;
Predator *predators;
Hut *huts;

void draw();
void update();
void cleanup();
void loadImages();
void calcNextGen();

AgentData agentData[4];
int latestScores[30];
int scoreCount;
int currentAgent = 0;
int highScore = 0;

// the main entry point for the application is this function
void DarkGDK ( void )
{
	int scoreCount = 0;
	dbSetDisplayMode(800,624,0);
	dbSetWindowSize(800,624);
	dbSyncOn   ( );
	dbSyncRate ( 0 );
	srand (time(NULL));
	loadImages();
	dbLoadImage("waypoint.png", 99);

	//agent1 = new Agent("agent.png");
	agents = new Agent[MAXNUMOFAGENTS];
	trees = new Tree[MAXNUMOFTREES];
	items = new Item[MAXNUMOFITEMS];
	prey = new Prey[MAXNUMOFPREY];
	predators = new Predator[MAXNUMOFPREDATORS];
	huts = new Hut[MAXNUMOFHUTS];
	//dbLoadImage("agent.png", 2);
	//dbSprite(2,25,25,2);
	//dbLoadImage("agent.bmp", 1);
	// turn on sync rate and set maximum rate to 60 fps

	currentAgent = 0;

	for(int i = 0; i < MAXNUMOFTREES; i++)
	{
		trees[i].replant();
		trees[i].grow();
	}

	for(int i = 0;i < 30; i ++)			
	{
		latestScores[i] = 0;
	}

	for(int i = 0; i < 100;i++)
	{
		//agentData.AI[i] = rand() % 4;
		agentData[0].AI[i] = 0;
		agentData[1].AI[i] = 1;
		agentData[2].AI[i] = 2;
		agentData[3].AI[i] = 3;
	}

	for(int i = 0; i < 4; i++)
	{
		agentData[i].chopping.x = rand() % dbScreenWidth();
		agentData[i].chopping.y = rand() % dbScreenHeight();
		agentData[i].building.x = rand() % dbScreenWidth();
		agentData[i].building.y = rand() % dbScreenHeight();
		agentData[i].hunting.x = rand() % dbScreenWidth();
		agentData[i].hunting.y = rand() % dbScreenHeight();
	}
	agents[0].reset(agentData[0], true);

	int ticker = 0;
	int day = 1;
	int time = 0;
	int generation = 1;
	int agent = 1;
	int timer = 0;
	// our main loop
	while ( LoopGDK ( ) )
	{	
		dbCLS();

		if(ticker > 1200)
		{
			ticker = 0;
		}
		//day/night timer
		for(int i = 0;i< MAXNUMOFAGENTS; i++)
		{
			if(agents[i].getEnabled())
			{
				timer++;
				time = (timer % 900) / 20;
				if((timer % 900) == 0)
				{
					timer = 0;
					day++;
				}
				break;
			}
		}
		
		if(!agents[0].getEnabled())
		{
			//clear the map
			for(int i = 0; i < MAXNUMOFHUTS; i++)
			{
				huts[i].destroy();
			}
			for(int i = 0; i < MAXNUMOFTREES; i++)
			{
				trees[i].replant();
				trees[i].grow();
			}
			for(int i = 0; i < MAXNUMOFITEMS; i++)
			{
				items[i].disable();
			}
			for(int i = 0; i < MAXNUMOFPREY; i++)
			{
				prey[i].spawn();
			}
			//store agent data
			agentData[currentAgent] = agents[0].getAgentData();
			agentData[currentAgent].score = (((day-1) * 45) + time) + agents[0].getHealth();
			latestScores[scoreCount] = agentData[currentAgent].score;
			scoreCount++;
				if(scoreCount > 29)
				{
					for(int i = 0;i < 30; i ++)
					{
						latestScores[i] = 0;
					}
					scoreCount = 0;
				}
		
			if(agentData[currentAgent].score > highScore)
			{
				highScore = agentData[currentAgent].score;
			}
			time = 0;
			timer = 0;
			day = 1;
			currentAgent++;
			if(currentAgent > 3)
			{
				generation++;
				currentAgent = 0;
				calcNextGen();
			}
			//start new agent
			if(generation == 1)
			{
				agents[0].reset(agentData[currentAgent], true); // parameter doesn't do anything in this case
			}
			else
			{
				agents[0].reset(agentData[currentAgent], true);
				// new gen
			}
		}

		if(time == 35)
		{
			for(int i = 0; i < MAXNUMOFPREDATORS;i++)
			{
				if(!predators[i].isActive())
				{
					predators[i].spawn();
				}
			}
		}
		if(time == 0)
		{
			for(int i = 0; i < MAXNUMOFPREDATORS;i++)
			{
				if(predators[i].isActive())
				{
					predators[i].kill();
				}
			}
		}

		dbSetCursor(600,2);
		dbPrint("Time: ");
		dbSetCursor(650,2);
		dbPrint((double)time);

		dbSetCursor(700,2);
		dbPrint("Day: ");
		dbSetCursor(750,2);
		dbPrint((double)day);

		dbSetCursor(280,2);
		dbPrint("Agent: ");
		dbSetCursor(330,2);
		dbPrint((double)currentAgent+1);

		dbSetCursor(350,2);
		dbPrint("Generation: ");
		dbSetCursor(450,2);
		dbPrint((double)generation);

		dbSetCursor(470,2);
		dbPrint("High Score: ");
		dbSetCursor(570,2);
		dbPrint((double)highScore);



		if(dbUpKey())
		{
			trees[rand() % 10].grow();
		}
		if(dbDownKey())
		{
			trees[rand() % 10].chop();
		}
		if(dbLeftKey())
		{
			trees[rand() % 10].disable();
		}
		if(dbRightKey())
		{
			trees[rand() % 10].replant();
		}
		if(dbShiftKey())
		{
			for(int i = 0; i < MAXNUMOFPREY; i++)
			{
				if(!prey[i].isActive())
				{
					prey[i].spawn();
					break;
				}
			}
		}
		update();
		draw();
		//dbPasteSprite(2,30,30);
		// update the screen
		dbSync ( );
	}
	cleanup();
	// return back to windows
	return;
}

void calcNextGen()
{
	int bestScore = 0;
	int bestAgentNum1 = -1;
	int bestAgentNum2 = -1;
	for(int c = 0;c < 2; c++) // get two best scores (fitness function)
	{
		for(int i = 0; i < AGENTSPERGEN; i++)
		{
			if(agentData[i].score > bestScore)// && i != bestAgentNum1)
			{
				bestScore = agentData[i].score;
				agentData[i].score = 0;
				bestAgentNum1 = i;
			}
		}
		if(bestAgentNum2 != -1)
		{
			break;
		}
		else
		{
			//highScore = agentData[bestAgentNum1].score;
			bestAgentNum2 = bestAgentNum1;
			bestScore = 0;
			//agentData[bestAgentNum1].score = 999999;
		}
	}
	
	//bestAgentNum1
	//bestAgentNum2

	AgentData parent1 = agentData[bestAgentNum1];
	AgentData parent2 = agentData[bestAgentNum2];

	int child1 = 0;
	int child2 = 1;

	for(int i = 0; i < 49; i++) // cross over
	{
		agentData[child1].AI[i] = parent1.AI[i];
		agentData[child1].AI[99-i] = parent2.AI[99-i];
		agentData[child2].AI[99-i] = parent1.AI[99-i];
		agentData[child2].AI[i] = parent2.AI[i];
	}
	agentData[child1].chopping = parent2.chopping;
	agentData[child1].building = parent1.building;
	agentData[child1].hunting = parent2.hunting;

	agentData[child2].chopping = parent1.chopping;
	agentData[child2].building = parent2.building;
	agentData[child2].hunting = parent1.hunting;

	//mutation
	for(int i = 0; i < 99; i++)
	{
		if(rand() % 20 == 0)
		{
			agentData[child1].AI[i] = rand() % 4;
			//highScore++;
		}
		if(rand() % 20 == 0)
		{
			agentData[child2].AI[i] = rand() % 4;
		}
	}

		for(int i = 0; i < 2; i++)
		{
			if(rand() % 100 < 50)
			{
				agentData[i].chopping.x = rand() % dbScreenWidth();
				agentData[i].chopping.y = rand() % dbScreenHeight();
			}
			if(rand() % 100 < 50)
			{
				agentData[i].hunting.x = rand() % dbScreenWidth();
				agentData[i].hunting.y = rand() % dbScreenHeight();
			}
			if(rand() % 100 < 50)
			{
				agentData[i].building.x = rand() % dbScreenWidth();
				agentData[i].building.y = rand() % dbScreenHeight();
			}
		}

		agentData[2] = parent1;
		agentData[3] = parent2;
}

void loadImages()
{
	dbLoadImage("map.png", mapImageId);
	dbSprite(spriteCounter, 1000,1000,mapImageId);
	mapSprite = spriteCounter;
	spriteCounter++;

	dbLoadImage("agent.png", agentImageId);
	dbLoadImage("brownbread.png", agentDeathImageId);

	dbLoadImage("tree.png", treeImageId);
	dbLoadImage("plant.png", treePlantImageId);
	dbLoadImage("stump.png", treeStumpImageId);

	dbLoadImage("wood.png", woodImageId);
	dbLoadImage("water.png", waterImageId);
	dbLoadImage("food.png", foodImageId);

	dbLoadImage("rabbit.png", rabbitImageId);
	dbLoadImage("lion.png", lionImageId);

	dbLoadImage("hut.png", hutImageId);

	dbLoadImage("waypointbuild.png", buildWaypointImageId);
	dbLoadImage("waypointhunt.png", huntWaypointImageId);
	dbLoadImage("waypointchop.png", chopWaypointImageId);
}

void spawnItem(std::string type, int X, int Y, int lifeTime)
{
	for(int i = 0; i < 10; i++)
	{
		if(!items[i].isActive())
		{
			items[i].spawn(type, X, Y, lifeTime);
			break;
		}
	}
}

void update()
{
	for(int i = 0;i<MAXNUMOFAGENTS;i++)
	{
		agents[i].update(items, trees, prey, huts);
	}
	for(int i = 0;i<MAXNUMOFTREES;i++)
	{
		trees[i].update(items);
	}
	for(int i = 0;i<MAXNUMOFITEMS;i++)
	{
		items[i].update();
	}
	for(int i = 0;i<MAXNUMOFPREY;i++)
	{
		prey[i].update(items);
	}
	for(int i = 0;i<MAXNUMOFPREDATORS;i++)
	{
		predators[i].update(items, agents);
	}
	for(int i = 0;i<MAXNUMOFHUTS;i++)
	{
		huts[i].update();
	}
}

void draw()
{
	dbPasteSprite(mapSprite,32,24);
	for(int i = 0; i < MAXNUMOFTREES; i++)
	{
		trees[i].draw();
	}
	for(int i = 0; i < MAXNUMOFITEMS; i++)
	{
		items[i].draw();
	}
	for(int i = 0;i<MAXNUMOFPREY;i++)
	{
		prey[i].draw();
	}
	for(int i = 0;i<MAXNUMOFPREDATORS;i++)
	{
		predators[i].draw();
	}
	for(int i = 0;i<MAXNUMOFHUTS;i++)
	{
		huts[i].draw();
	}
	for(int i = 0;i<MAXNUMOFAGENTS;i++)
	{
		agents[i].draw();
	}
	for(int i = 0;i < 30; i ++)
	{
		dbSetCursor(0,24+((i+1)*16));
		dbPrint((double)latestScores[i]);
	
	}
}

void cleanup()
{
	delete agents;
	delete trees;
	delete items;
	delete prey;
	delete predators;
	delete huts;
}