class Prey
{
private:
	int rabbitSprite;
	int waypointSprite;
	bool active;
	int x,y;
	int ticker;
	int ticker2;
	int waitTimer;
	bool fleeing;
	int threatX;
	int threatY;
	Waypoint waypoint;
public:
	Prey()
	{
		dbSprite(spriteCounter,1000,1000,99);
		waypointSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter, 1000,1000, rabbitImageId);
		rabbitSprite = spriteCounter;
		spriteCounter++;

		x = 0;
		y = 0;
		active = false;
		fleeing = false;
		ticker = 0;

		spawn();
		//makeWaypoint();
	}
	~Prey()
	{
	}
	void spawn()
	{
		active = true;
		x = rand() % (dbScreenWidth() - 64) + 32;
		y = rand() % (dbScreenHeight() / 2 - 64) + (32);
		//while(y < 64)
		//{
		//	y = rand() % (((dbScreenHeight() / 2) - 64) + 32);
		//}
		makeWaypoint();
	}
	void kill()
	{
		active = false;
		ticker2 = 0;
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
	void flee(int ThreatX, int ThreatY)
	{
		fleeing = true;
		threatX = ThreatX;
		threatY = ThreatY;
	}
	void moveAway(int X, int Y)
	{
		if(getDistance(x,y,X,Y) > 4)
		{
			float dx = X-x;
			float dy = Y-y;
			float length = sqrtf(dx*dx+dy*dy);
			dx/=length;
			dy/=length;
			
			x -= dx*2;
			y -= dy*2;
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
	void update(Item *items)
	{
		if(ticker > 1200)
		{
			ticker = 0;
		}
		if(ticker2 > 1200)
		{
			ticker2 = 0;
		}
		ticker2++;
		if(!active)
		{
			ticker2++;
			if(ticker2 > 100)
			{
				ticker2 = 0;
				spawn();
				return;
			}
		}
		if(fleeing)
		{
			moveAway(threatX, threatY);
			fleeing = false;
			return;
		}
		ticker++; // keep track of the frames
		if(ticker > 19)
		{
			// happens every second
			if(waitTimer > 0)
			{
				waitTimer--; // count down the timer
			}
			else if((rand() % 15) == 1)
			{
				waitTimer = rand() % 5+5;
			}
			ticker = 0;
		}
		//update AI
		if(waitTimer > 0)
		{
			return;
		}
		wander();

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
		while(getDistance(x,y,waypoint.x,waypoint.y) > 64)
		{
			waypoint.x = rand() % (dbScreenWidth() - 64) + 32;
			waypoint.y = rand() % (dbScreenHeight() / 2 - 64) + (32);
		}
	}
	int getPosX()
	{
		return x;
	}
	int getPosY()
	{
		return y;
	}
	void setWaypoint(int X, int Y)
	{
		waypoint.x = X;
		waypoint.y = Y;
	}
	void draw()
	{
		if(active)
		{
			dbPasteSprite(rabbitSprite,x,y);
			dbPasteSprite(waypointSprite,waypoint.x,waypoint.y);
		}
		else
		{
			x = 1000;
			y = 1000;
		}
	}
};