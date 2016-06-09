class Item
{
private:
	int woodSprite;
	int waterSprite;
	int foodSprite;
	std::string type;
	int x,y;
	int life; // seconds life life
	int ticker; // keeps track of game seconds
	bool active;
public:
	Item()
	{
		dbSprite(spriteCounter, 1000,1000, woodImageId);
		woodSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter, 1000,1000, waterImageId);
		waterSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter, 1000,1000, foodImageId);
		foodSprite = spriteCounter;
		spriteCounter++;

		x = 0;
		y = 0;
		type = "N";
		life = 0;
		ticker = 0;
		active = false;
	}
	~Item()
	{
	}
	void update()
	{
		if(ticker > 1200)
		{
			ticker = 0;
		}
		ticker++;
		if(ticker >= 20)
		{
			ticker = 0;
			life--;
			if(life <= 0)
			{
				life = 0;
				type = "N";
			}
		}
	}
	void draw()
	{
		if(strstr(type.c_str(), "wood"))
		{
			dbPasteSprite(woodSprite, x,y);
		}
		else if(strstr(type.c_str(), "water"))
		{
			dbPasteSprite(waterSprite,x,y);
		}
		else if(strstr(type.c_str(), "food"))
		{
			dbPasteSprite(foodSprite,x,y);
		}
	}
	void stockpile(int X, int Y) // pos of stockpile
	{
		x = rand() % 64 + X;
		y = rand() % 64 + Y;
		life = 240;
	}
	void disable()
	{
		active = false;
		type = "N";
		x = 1000;
		y = 1000;
	}
	void spawn(std::string Type, int X, int Y, int lifeTime = 60)
	{
		active = true;
		type = Type;
		x = X;
		y = Y;
		life = lifeTime;
	}
	int getPosX()
	{
		return x;
	}
	int getPosY()
	{
		return y;
	}
	std::string getType()
	{
		return type;
	}
	bool isActive()
	{
		if(!active)//strstr(type.c_str(), "N"))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
};