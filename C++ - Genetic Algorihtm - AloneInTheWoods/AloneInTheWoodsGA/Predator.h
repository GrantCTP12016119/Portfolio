class Predator
{
private:
	int lionSprite;
	int waypointSprite;
	bool active;
	int x,y;
	int ticker;
	int waitTimer;
	Waypoint waypoint;
public:
	Predator()
	{
		dbSprite(spriteCounter, 1000, 1000, 99);
		waypointSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter, 1000, 1000, lionImageId);
		lionSprite = spriteCounter;
		spriteCounter++;

		x = 0;
		y = 0;
		active = false;
		ticker = 0;

		//spawn();
		//makeWaypoint();
	}
	~Predator()
	{
	}
	void spawn()
	{
		active = true;
		x = rand() % (dbScreenWidth() - 64) + 32;
		y = rand() % (dbScreenHeight() / 2 - 64) + (32);
		//x = rand() % (dbScreenWidth() - 64) + 32;
		//while(y < 64)
		//{
		//	y = rand() % (((dbScreenHeight() / 2) + 64) - 96);
		//}
		makeWaypoint();
	}
	void kill()
	{
		active = false;
	}
	bool isActive()
	{
		if(active)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void persueAgent(Agent *agent)
	{
		waypoint.x = agent->getPosX();
		waypoint.y = agent->getPosY();
		float dx = agent->getPosX()-x;
		float dy = agent->getPosY()-y;
		float length = sqrtf(dx*dx+dy*dy);
		dx/=length;
		dy/=length;
		
		x += dx*3;
		y += dy*3;
	}
	void update(Item *items, Agent *agents)
	{
		if(ticker > 1200)
		{
			ticker = 0;
		}
		if(!active)
		{
			return;
		}
		for(int i = 0;i < MAXNUMOFAGENTS;i++)
		{
			int dist = getDistance(x, y, agents[i].getPosX(), agents[i].getPosY());
			if(dist < 128 && agents[i].getEnabled())
			{
				if(dist < 10)
				{
					agents[i].kill();
					return;
				}
				persueAgent(&agents[i]);
				return;
			}
		}
		ticker++; // keep track of the frames
		//if(ticker > 19)
		//{
		if(ticker % 20 == 0)
		{// happens every second
			if(waitTimer > 0)
			{
				waitTimer--; // count down the timer
			}
			else if((rand() % 15) == 1)
			{
				waitTimer = rand() % 5+5;
			}
			//ticker = 0;
		}
		//if(ticker % 300 == 0)
		//{
		//	kill();
		//}
		//update AI
		if(waitTimer > 0)
		{
			return;
		}
		wander();
		//float dx = waypoint.x-x;
		//float dy = waypoint.y-y;
		//float length = sqrtf(dx*dx+dy*dy);
		//dx/=length;
		//dy/=length;
		//
		//x += dx*2;
		//y += dy*2;

		//float distanceX = x-waypoint.x;
		//distanceX *= distanceX;
		//float distanceY = y-waypoint.y;
		//distanceY *= distanceY;

		//float distance = sqrtf(distanceX+distanceY);

		//if(distance < 5)
		//{
		//	makeWaypoint();
		//}
	}
	void wander()
	{
		if(getDistance(x,y, waypoint.x, waypoint.y) < 5)
		{
			makeWaypoint();
		}
		moveTo(waypoint.x, waypoint.y);
	}
	void makeWaypoint()
	{
		waypoint.x = rand() % (dbScreenWidth() - 128) + 128;
		waypoint.y = rand() % (((dbScreenHeight() / 2) - 128) + 128);
		while(getDistance(x,y,waypoint.x,waypoint.y) > 64 && y < dbScreenHeight()/2 && x < dbScreenWidth())
		//while(getDistance(x,y,waypoint.x,waypoint.y) > 64)
		{
			waypoint.x = rand() % (dbScreenWidth() - 64) + 32;
			waypoint.y = rand() % (dbScreenHeight() / 2 - 64) + (32);
		}
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
			
			x += dx*2;
			y += dy*2;
		}
	}
	//void makeWaypoint(int radius = 64)
	//{
	//	waypoint.x = rand() % (dbScreenWidth() - 64) + 32;
	//	waypoint.y = rand() % (((dbScreenHeight() / 2) + 64) - 96);
	//	while(getDistance(x,y,waypoint.x,waypoint.y) > radius && y < dbScreenHeight()/2 && x < dbScreenWidth())
	//	{
	//		waypoint.x = rand() % (dbScreenWidth() - 64) + 32;
	//		waypoint.y = rand() % (((dbScreenHeight() / 2) + 64) - 96);
	//	}
	//}
	void setWaypoint(int X, int Y)
	{
		waypoint.x = X;
		waypoint.y = Y;
	}
	void draw()
	{
		if(active)
		{
			dbPasteSprite(lionSprite,x,y);
			dbPasteSprite(waypointSprite,waypoint.x,waypoint.y);
		}
	}
};