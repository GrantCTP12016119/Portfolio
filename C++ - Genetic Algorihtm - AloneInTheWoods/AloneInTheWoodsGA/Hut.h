class Hut
{
private:
	int sprite;
	bool active, inUse;
	int useTimer;
	int ticker;
	int x, y;
	int life;
	int ownerId;
public:
	Hut()
	{
		dbSprite(spriteCounter, 1000, 1000, hutImageId);
		sprite = spriteCounter;
		spriteCounter++;
		ownerId = -1;
		//spawn();
		active = false;
	}
	~Hut()
	{
	}
	void spawn(int X, int Y)
	{
		active = true;
		x = rand() % dbScreenWidth();
		y = rand() % dbScreenHeight();
		while(getDistance(x,y,X,Y) > 128)//  && x > (dbScreenWidth() - 64) || y > (dbScreenHeight() - 64))
		{
			x = rand() % dbScreenWidth();
			y = rand() % dbScreenHeight();
		}
		ticker = 0;
		life = 100;
	}
	void fix()
	{
		life = 100;
	}
	int getOwnerId()
	{
		return ownerId;
	}
	void destroy()
	{
		active = false;
	}
	int getPosX()
	{
		return x;
	}
	int getPosY()
	{
		return y;
	}
	bool getInUse()
	{
		return inUse;
	}
	bool isActive()
	{
		return active;
	}
	void occupy(int Id)
	{
		inUse = true;
		useTimer = 20;
		ownerId = Id; // set the owner agent
	}
	bool getActive()
	{
		return active;
	}
	void update()
	{
		if(ticker > 1200)
		{
			ticker = 0;
		}
		if(active)
		{
			if(inUse) // time out the use of the hut so it will free itself up again if it is not being used
			{
				useTimer--;
				if(useTimer <= 0) // free the hut
				{
					useTimer = 0;
					inUse = false;
					ownerId = -1;
				}
			}
			ticker++;
			if(ticker >= 20)
			{
				ticker = 0;
				life--;
				if(life <= 0)
				{
					life = 0;
					active = false;
				}
			}
		}
	}
	void draw()
	{
		if(active)
		{
			dbPasteSprite(sprite,x,y);
		}
	}
};