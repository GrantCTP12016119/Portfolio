class Agent
{
private:
	int sprite;
	int deathSprite;
	int waypointSprite;
	int chopWaypointSprite;
	int huntWaypointSprite;
	int buildWaypointSprite;
	int x,y;
	bool enabled;
	int killedTimer;
	int ticker;
	int state;
	int id;
	/*int agentData[100];*/
	AgentData agentData;
	int currentAI;

	//stats
	int food;
	int hydration;
	int warmth;
	Waypoint waypoint;
	Waypoint sandbox;
	Waypoint water;
public:
	Agent() // defualt constructor
	{
		dbSprite(spriteCounter, 1000,1000, agentImageId);
		sprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter, 1000, 1000, agentDeathImageId);
		deathSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter,1000,1000,99);
		waypointSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter,1000,1000,chopWaypointImageId);
		chopWaypointSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter,1000,1000,huntWaypointImageId);
		huntWaypointSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter,1000,1000,buildWaypointImageId);
		buildWaypointSprite = spriteCounter;
		spriteCounter++;

		food = 0;
		hydration = 0;
		warmth = 0;
		//enabled = false;
		reset(agentData);
	}

	~Agent()
	{
	}

	void reset(AgentData AAgentData, bool useData = false)
	{
		/*x = 25;
		y = 25;*/
		x = rand() % (dbScreenWidth() - 64) + 32;
		while(y < 64)
		{
			y = rand() % (((dbScreenHeight() / 2) + 64) - 96);
		}
		enabled = true;
		killedTimer = 0;
		ticker = 301;
		state = 1;
		id = rand() % 999999;

		food = 70;
		hydration = 70;
		warmth = 70;

		sandbox.x = 256;
		sandbox.y = 256;

		if(!useData)
		{
			for(int i = 0; i < 100;i++)
			{
				//agentData.AI[i] = rand() % 4;
				//agentData.AI[i] = 2;
			}
			//agentData.AI[0] = 2;
			//agentData.AI[1] = 0;
			//agentData.AI[2] = 1;
			//agentData.AI[3] = 3;
			//agentData.AI[4] = 2;
			//agentData.AI[5] = 0;
			//agentData.AI[6] = 1;
			//agentData.AI[7] = 3;
			//agentData.AI[8] = 2;
			//agentData.AI[9] = 0;
			//agentData.AI[10] = 1;
			//agentData.AI[11] = 3;
			//agentData.AI[12] = 2;
			//agentData.AI[13] = 0;
			//agentData.AI[14] = 1;
			//agentData.AI[15] = 3;
			agentData.chopping.x = rand() % dbScreenWidth();
			agentData.chopping.y = rand() % dbScreenHeight();
			
			agentData.hunting.x = rand() % dbScreenWidth();
			agentData.hunting.y = rand() % dbScreenHeight();

			agentData.building.x = rand() % dbScreenWidth();
			agentData.building.y = rand() % dbScreenHeight();
		}
		else
		{
			agentData = AAgentData;
		}

		agentData.chopping.x = dbScreenWidth() / 2 - 32;
		agentData.chopping.y = dbScreenHeight() / 4;

		agentData.hunting.x = dbScreenWidth() / 2 + 32;
		agentData.hunting.y = dbScreenHeight() / 4;

		agentData.building.x = dbScreenWidth() / 4;
		agentData.building.y = dbScreenHeight() * 0.7;

		currentAI = 0;

		/*agentData.chopping.x = rand() % dbScreenWidth();
			agentData.chopping.y = rand() % dbScreenHeight();
			
			agentData.hunting.x = rand() % dbScreenWidth();
			agentData.hunting.y = rand() % dbScreenHeight();

			agentData.building.x = rand() % dbScreenWidth();
			agentData.building.y = rand() % dbScreenHeight();*/

		water.x = dbScreenWidth() * 0.7;
		water.y = dbScreenHeight() * 0.8;
	}

	AgentData getAgentData()
	{
		//AgentData tempData;
		//for(int i = 0;i < 100;i++)
		//{
		//	tempData.AI[i] = agentData.AI[i];
		//}
		//tempData.choppingX = chopping.x;
		//tempData.choppingY = chopping.y;

		//tempData.buildingX = building.x;
		//tempData.buildingY = building.y;

		//tempData.huntingX = hunting.x;
		//tempData.huntingY = hunting.y;

		return agentData;
	}

	//Agent(char* imagePath)
	//{
	//	dbLoadImage(imagePath, 1);
	//	dbSprite(spriteCounter, 1000,1000, 1);
	//	sprite = spriteCounter;
	//	spriteCounter++;
	//	x = 25;
	//	y = 25;
	//}
	void kill()
	{
		killedTimer = 5*20;
		enabled = false;
	}
	bool getEnabled()
	{
		return enabled;	
	}
	int getPosX()
	{
		return x;
	}
	int getPosY()
	{
		return y;
	}
	void update(Item *items, Tree *trees, Prey *prey, Hut *huts)
	{
		if(ticker > 1200)
		{
			ticker = 0;
		}
		if(enabled)
		{
			if(food > 100)
			{
				food = 100;
			}
			if(hydration > 100)
			{
				hydration = 100;
			}
			if(warmth > 100)
			{
				warmth = 100;
			}
			if(ticker % 30 == 0)
			{
				food--;
				hydration--;
			}
			if(ticker % 35 == 0)
			{
				warmth--;
			}
			if(food < 1 || hydration < 1 || warmth < 1)
			{
				kill();
			}
			//if(dbUpKey())
			//{
			//	y-=3;
			//}
			//if(dbDownKey())
			//{
			//	y+=3;
			//}
			//if(dbLeftKey())
			//{
			//	x-=3;
			//}
			//if(dbRightKey())
			//{
			//	x+=3;
			//}
			//Agent AI goes here
			//hunteat(items, prey);
			//drink();
			//build(items, trees, huts);
			//rest(items, trees, huts);
			ticker++; 
			if(ticker > 300)
			{
				ticker = 0;
				//state = rand() % 4;
				state = agentData.AI[currentAI];
				currentAI++;
				if(currentAI > 99)
				{
					currentAI = 0;
				}
			}
			//wander(chopping);
			if(state == 0)
			{
				hunteat(items, prey);
			}
			else if(state == 1)
			{
				drink();
			}
			else if(state == 2)
			{
				build(items, trees, huts);
			}
			else if(state == 3)
			{
				rest(items, trees, huts);
			}

		}
	}
	void rest(Item *items, Tree *trees, Hut *huts)
	{
		int hutDist = 1001;
		int hutNum = 0;
		for(int i = 0; i < MAXNUMOFHUTS; i++) // find hut that is closest and not in use
		{
			if(getDistance(x, y, huts[i].getPosX(), huts[i].getPosY()) < hutDist && huts[i].isActive())
			{
				if(id == huts[i].getOwnerId() || !huts[i].getInUse())
				{
					hutDist = getDistance(x,y,huts[i].getPosX(), huts[i].getPosY());
					hutNum = i;
				}
			}
		}
		if(hutDist != 1001)
		{
			if(hutDist > 0)
			{
				moveTo(huts[hutNum].getPosX() + 16, huts[hutNum].getPosY() + 24);
				if(getDistance(x,y,huts[hutNum].getPosX() + 16,huts[hutNum].getPosY()+ 24) < 8)
				{
					//replenish warmth
					if(ticker % 5 == 0)
					{
						warmth++;
					}
				}
			}
			huts[hutNum].occupy(id);
		}
		else
		{
			build(items, trees, huts);
		}
	}
	void build(Item *items, Tree *trees, Hut *huts)
	{
		//AI for building and gathering wood
		int woodDist = 1001;
		int woodNum = 0;
		for(int i = 0; i < MAXNUMOFITEMS; i++)
		{
			if(strstr(items[i].getType().c_str(), "wood") && items[i].isActive())
			{
				if(getDistance(agentData.building.x, agentData.building.y, items[i].getPosX(), items[i].getPosY()) < 128) // if there is wood in the stockpile
				{
					moveTo(items[i].getPosX(), items[i].getPosY()); // move to it
					if(getDistance(x,y, items[i].getPosX(), items[i].getPosY()) < 8)
					{
						items[i].disable();
						//build a hut
						for(int i = 0;i < MAXNUMOFHUTS; i++)
						{
							if(!huts[i].getActive())
							{
								huts[i].spawn(agentData.building.x, agentData.building.y);
								return;
							}
						}
					}
					huts[rand() % MAXNUMOFHUTS].fix();
					return;
				}
				if(getDistance(x,y, items[i].getPosX(), items[i].getPosY()) < woodDist)
				{
					woodDist = getDistance(x,y,items[i].getPosX(), items[i].getPosY());
					woodNum = i;
				}
			}
			if(woodDist != 1001 && woodDist < 128)
			{
				moveTo(items[woodNum].getPosX(), items[woodNum].getPosY()); // move to it
				if(getDistance(x,y,items[woodNum].getPosX(), items[woodNum].getPosY()) < 8) // if we collide
				{
					//items[woodNum].disable();
					items[woodNum].stockpile(agentData.building.x, agentData.building.y);
					// take wood
					
				}
				return;
			}
		}
		//float treeDist = getDistance(x,y,trees[0].getPosX(), trees[0].getPosY()); // store first distance
		float treeDist = 1001;
		int treeNum = 0; // element no.
		for(int i = 0; i < MAXNUMOFTREES; i++) // get a tree that can be chopped
		{
			if(trees[i].getState() == 2 && getDistance(x,y,trees[i].getPosX(), trees[i].getPosY()) < treeDist) // if closer then select it
			{
				treeDist = getDistance(x,y,trees[i].getPosX(), trees[i].getPosY()); // select the tree
				treeNum = i; // store element no.
			}
		}
		if(treeDist != 1001 && treeDist < 128) // a tree has been selected so move to it
		{
			//int treeDist = getDistance(x,y,trees[treeNum].getPosX(), trees[treeNum].getPosY());
			moveTo(trees[treeNum].getPosX(), trees[treeNum].getPosY()); // move to tree
			if(treeDist < 8) // check for collision
			{
				//chop down tree and spawn wood
				trees[treeNum].chop();
				spawnItem("wood", trees[treeNum].getPosX() - 8, trees[treeNum].getPosY() + 64);
			}
			return;
		}
		else
		{
			//possibly set a waypoint here
			wander(agentData.chopping);
		}
	}
	void drink()
	{
		wander(water);
		if(ticker % 5 == 0)
		{
			hydration++;
		}
	}
	void hunteat(Item *items, Prey *prey)
	{
		//AI for hunting and eating
		float foodDist = 1001;
		int foodNum = 0;
		for(int i = 0; i < MAXNUMOFITEMS; i++)
		{
			if(strstr(items[i].getType().c_str(), "food") && items[i].isActive())
			{
				if(getDistance(x,y,items[i].getPosX(), items[i].getPosY()) < foodDist)
				{
					foodDist = getDistance(x,y,items[i].getPosX(), items[i].getPosY());
					foodNum = i;
				}
			}
			if(foodDist != 1001 && foodDist < 512) // if we have found food and it's close enough
			{
				moveTo(items[foodNum].getPosX(), items[foodNum].getPosY()); // move to it
				if(getDistance(x,y,items[foodNum].getPosX(), items[foodNum].getPosY()) < 8) // if we collide
				{
					items[foodNum].disable();
					//replenish food bar
					food += 20;
					//hydration += 2;
				}
				return;
			}
		}
		float bestDist = getDistance(x,y,prey[0].getPosX(), prey[0].getPosY()); // store first distance
		int preyNum = 0; // element no.
		for(int i = 0; i < MAXNUMOFPREY; i++) // get the closest prey
		{
			if(getDistance(x,y,prey[i].getPosX(), prey[i].getPosY()) < bestDist && prey[i].isActive()) // if closer then
			{
				bestDist = getDistance(x,y,prey[i].getPosX(), prey[i].getPosY()); // overrite best distance
				preyNum = i; // store element no.
			}
		}
		if(prey[preyNum].isActive() && bestDist < 128) // if the prey is active then persue
		{
			int preyDist = getDistance(x,y,prey[preyNum].getPosX(), prey[preyNum].getPosY());
			moveTo(prey[preyNum].getPosX(), prey[preyNum].getPosY()); // persue
			if(preyDist < 128)
			{
				prey[preyNum].flee(x,y); // tell the prey to flee
			}
			if(preyDist < 8) // check for collision
			{
				//kill prey and spawn food
				prey[preyNum].kill();
				spawnItem("food", prey[preyNum].getPosX() + 25, prey[preyNum].getPosY() + 25);
			}
			return;
		}
		else
		{
			//possibly set a waypoint here
			wander(agentData.hunting);
		}
		
	}
	void wander(Waypoint target)
	{
		int radius = 128;
		if(getDistance(x,y, waypoint.x, waypoint.y) < 5 || getDistance(target.x,target.y, waypoint.x, waypoint.y) > radius + 8)
		{
			makeWaypoint(radius, target);
		}
		moveTo(waypoint.x, waypoint.y);
	}
	void makeWaypoint(int radius, Waypoint target)
	{
		//waypoint.x = rand() % (target.x - radius) + (radius * 2);
		//waypoint.y = rand() % (target.y - radius) + (radius * 2);
		waypoint.x = rand() % radius + (target.x - radius / 2);
		waypoint.y = rand() % radius + (target.y - radius / 2);
		while(getDistance(target.x,target.y, waypoint.x, waypoint.y) > radius)
		{
			//dbSetCursor(0,0);
			//dbPrint("its me");
			waypoint.x = rand() % radius + (target.x - radius / 2);
			waypoint.y = rand() % radius + (target.y - radius / 2);
		}
		/*while(getDistance(x,y,waypoint.x,waypoint.y) > radius && y < dbScreenHeight()/2 && x < dbScreenWidth())
		{
			waypoint.x = rand() % (dbScreenWidth() - 64) + 32;
			waypoint.y = rand() % (((dbScreenHeight() / 2) + 64) - 96);
		}*/
	}
	void moveTo(int X, int Y)
	{
		if(getDistance(x,y,X,Y) > 4)
		{
			float dx = X-x;
			float dy = Y-y;
			float length = sqrtf(dx*dx+dy*dy);
			dx/=length;
			dy/=length;
			

			x += dx*4;
			y += dy*4;
		}
	}
	int getHealth()
	{
		return (food + hydration + warmth);
	}
	void draw()
	{
		if(enabled)
		{
			dbPasteSprite(sprite, x, y);
			dbPasteSprite(waypointSprite, waypoint.x, waypoint.y);

			dbPasteSprite(huntWaypointSprite, agentData.hunting.x, agentData.hunting.y);
			dbPasteSprite(chopWaypointSprite, agentData.chopping.x, agentData.chopping.y);
			dbPasteSprite(buildWaypointSprite, agentData.building.x, agentData.building.y);

			dbSetCursor(2,2);
			dbPrint("Food: "), 
			dbSetCursor(48,2);
			dbPrint((double)food);
			dbSetCursor(88,2);
			dbPrint("Water: "),
			dbSetCursor(140,2);
			dbPrint((double)hydration);
			dbSetCursor(180,2);
			dbPrint("Warmth: "),
			dbSetCursor(240,2);
			dbPrint((double)warmth);

			for(int i = 0; i < 99; i++)
			{
				dbSetCursor(8*i, 12);
				dbPrint((double)agentData.AI[i]);
			}
		}
		else if (killedTimer > 0)
		{
			dbPasteSprite(deathSprite,x,y);
			//killedTimer--;
			if(killedTimer < 1)
			{
				//enabled = true;
			}
		}
	}
};