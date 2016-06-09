class Tree
{
private:
	int treeSprite;
	int plantSprite;
	int stumpSprite;
	int state;
	int ticker;
	int timer;
	int x,y;
public:
	Tree()
	{
		state = 0;
		x = 0;
		y = 0;
		dbSprite(spriteCounter, 1000,1000, treeImageId); // tree
		treeSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter, 1000,1000, treePlantImageId);
		plantSprite = spriteCounter;
		spriteCounter++;

		dbSprite(spriteCounter, 1000,1000, treeStumpImageId);
		stumpSprite = spriteCounter;
		spriteCounter++;
	}
	~Tree()
	{
	}
	void draw()
	{
		if(state == 1)
		{
			dbPasteSprite(plantSprite,x,y);
		}
		else if(state == 2)
		{
			dbPasteSprite(treeSprite,x,y);
		}
		else if(state == 3)
		{
			dbPasteSprite(stumpSprite,x,y);
		}
	}
	void update(Item *items)
	{
		if(ticker > 1200)
		{
			ticker = 0;
		}
		if(dbControlKey() && state == 2)
		{
			chop();
			for(int i = 0; i < 10; i++)
			{
				if(!items[i].isActive())
				{
					items[i].spawn("wood", x - 8, y + 64, 5);
					break;
				}
			}
		}

		
		if(state != 2)
		{
			ticker++;
			if(ticker > 200)
			{
			ticker = 0;
			if(state == 3)
			{
				state = 4;
			}
			else if(state == 4)
			{
				replant();
			}
			else if(state == 1)
			{
				state = 2;
			}
		}
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
	int getState()
	{
		return state;
	}
	void replant()
	{
		x = rand() % (dbScreenWidth() - 64) + 32;
		while(y < 64)
		{
			y = rand() % (((dbScreenHeight() / 2) + 64) - 96);
		}
		state = 1;
	}
	void grow()
	{
		state = 2;
	}
	void chop()
	{
		state = 3;
		ticker = 0;
	}
	void disable()
	{
		state = 0;
	}
};