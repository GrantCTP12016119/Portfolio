class Button
{
private:
	int x,y;
	int width, height;
	int spriteId;
	int hlSpriteId; // highlighted sprite

	bool highlighted;

public:
	Button()
	{
	}
	Button(char* fileName, char* hlFileName, int X, int Y)
	{
		dbLoadImage(fileName, spriteCounter);
		dbSprite(spriteCounter,1000,1000,spriteCounter);
		spriteId = spriteCounter;
		spriteCounter++;

		dbLoadImage(hlFileName, spriteCounter);
		dbSprite(spriteCounter,1000,1000,spriteCounter);
		hlSpriteId = spriteCounter;
		spriteCounter++;

		highlighted = false;

		width = dbSpriteWidth(spriteId);
		height = dbSpriteHeight(spriteId);

		//relative to panel
		x = X;
		y = Y;
	}
	~Button()
	{
	}

	void setHighlighted(bool Highlighted)
	{
		highlighted = Highlighted;
	}

	bool getHightlighted()
	{
		return highlighted;
	}

	void toggleHighlighted()
	{
		highlighted = !highlighted;
	}

	bool checkButtonPress(int mouseX, int mouseY)
	{
		if(mouseX >= x && 
			mouseX < x+width &&
			mouseY >= y &&
			mouseY < y+height)
		{
			return true;
		}
		return false;
	}

	void draw()
	{
		if(highlighted)
		{
			dbPasteSprite(hlSpriteId,x,y);
		}
		else
		{
			dbPasteSprite(spriteId, x,y);
		}
	}
};