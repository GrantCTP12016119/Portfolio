#include <string>
#include <stdlib.h>
#include <time.h>     
#include "pcgrng.h"
#include "DarkGDK.h"

//get distance between two vectors
float getDistance(int x1, int y1, int x2, int y2);

//record random number generator curent state
void recordRNGState();

// reverse the random number generator state by num
void reverseRNGState(int num);

int spriteCounter = 0;

pcg32_random_t rng;

//globals constants, change as you please
int const MAXFRAMESREC = 10000;
int const GAMEOBJECTS = 50;

//stores all previous states of random number generator
__int64 states[MAXFRAMESREC];
int curState = 0;

struct Bubble
{
public:
	int x, y;
	int spriteId;
};

class gameObject
{
private:
	int spriteId;
	float x, y;
	float velX, velY;
	int currentFrame;
	std::string type;
	//structure for data stored at each frame per object
	struct frame
	{
	public:
		float x;
		float y;
		float velX;
		float velY;
		bool isFrameReCalc; // when this is true it means this frame was when
		// the object was respawned / recalculated, this is for the benefit of 
		//the random number generator, so that the RNG knows when it needs 
		//to reverse it's states
	};
	frame frames[MAXFRAMESREC];

public:
	std::string getType()
	{
		return type;
	}
	gameObject::gameObject()
	{
		// defualt constructor
	}
	gameObject(char* filePath, int X, int Y, int VelX, int VelY) // overloaded
		//function, used for setting custom velocities and positions
	{
		int col = rand() % 2;
		if(col % 2 == 0)
		{
			type = "red";
			dbLoadImage("redP.png", spriteCounter+1);
		}
		else
		{
			type = "yellow";
			dbLoadImage("yellowP.png", spriteCounter+1);
		}
		for(int i = 0; i < MAXFRAMESREC;i++)
		{
			frames[i].isFrameReCalc = false;
		}

		currentFrame = 0;
		x = X;
		y = Y;
		velX = VelX;
		velY = VelY;
		spriteId = spriteCounter+1;
		dbSprite(spriteCounter+1, 1000,1000,spriteCounter+1);
		dbSizeSprite(spriteCounter+1, 6,6);
		spriteCounter++;
	}
	gameObject(char* filePath)
	{
		int col = rand() % 2;
		if(col % 2 == 0)
		{
			type = "red";
			dbLoadImage("redP.png", spriteCounter+1);
		}
		else
		{
			type = "yellow";
			dbLoadImage("yellowP.png", spriteCounter+1);
		}
		frames[0].y = 1000;
		frames[0].isFrameReCalc = true;
		currentFrame = 1;
		x = dbScreenWidth()/2;
		y = dbScreenHeight()/3;
		//velX = rand() % 15 - 7.5;
		//velY = rand() % 5 - 7.5;
		recordRNGState();
		velX = pcg32_boundedrand_r(&rng, 15) - 7.5;
		recordRNGState();
		velY = pcg32_boundedrand_r(&rng, 5) - 7.5;
		spriteId = spriteCounter+1;
		dbSprite(spriteCounter+1, 1000,1000,spriteCounter+1);
		dbSizeSprite(spriteCounter+1, 6,6);
		spriteCounter++;
	}
	void draw()
	{
		dbPasteSprite(spriteId,x,y);
	}
	void drawFrame(int frameNum) // 0-MAXFRAMESREC
	{
		x = frames[frameNum].x;
		y = frames[frameNum].y;
		velX = frames[frameNum].velX;
		velY = frames[frameNum].velY;
	}
	void reverse(int num)
	{
		if(currentFrame - num > 0)
		{
			int destFrame = currentFrame - num;
			if(frames[destFrame].isFrameReCalc)
			{
				reverseRNGState(2);
			}
			x = frames[destFrame].x;
			y = frames[destFrame].y;
			velX = frames[destFrame].velX;
			velY = frames[destFrame].velY;
			currentFrame-=num;
		}
		else // reset the stream and start recording again
		{
			if(frames[0].isFrameReCalc)
			{
				reverseRNGState(2);
			}
			x = frames[0].x;
			y = frames[0].y;
			velX = frames[0].velX;
			velY = frames[0].velY;
			currentFrame = 0;
		}
	}

	void update(Bubble bubble1)
	{
		if(getDistance(x,y,bubble1.x + (dbSpriteWidth(200) / 2), bubble1.y + (dbSpriteHeight(200) / 2)) > dbSpriteWidth(200) / 2)
		{
			//record frame
			frame newFrame;
			newFrame.x = x;
			newFrame.y = y;
			newFrame.velX = velX;
			newFrame.velY = velY;
			frames[currentFrame] = newFrame;
			//calc physics
			x = x+velX;
			y = y+velY;

			if(velX > 0)
			{
				velX = velX-0.20; // slowly decrease x velocity
			}
			else if(velX < 0)
			{
				velX = velX+0.20;
			}
			velY = velY+0.5; // apply a constant down force (gravity)

			if(y > 400) // if the obejct is below the despawn/reset point
			{
				//reapawn the object at the top
				x = dbScreenWidth()/2;
				y = dbScreenHeight()/3;
				//velX = rand() % 15 - 7.5;
				//velY = rand() % 5 - 7.5;
				recordRNGState();
				velX = pcg32_boundedrand_r(&rng, 15) - 7.5;
				recordRNGState();
				velY = pcg32_boundedrand_r(&rng, 5) - 7.5;
				//inform the RNG that it needs to reverse it's states after
				//crossing this frame during game time reverse
				frames[currentFrame].isFrameReCalc = true; 
			}
			else
			{
				frames[currentFrame].isFrameReCalc = false; 
			}
			currentFrame++;
			if(currentFrame > MAXFRAMESREC)
			{
				currentFrame = 0;
			}
		}
	}
};

void recordRNGState()
{
	states[curState] = rng.state;
	curState++;
	if(curState >= MAXFRAMESREC)
	{
		curState = 0;
	}
}

void reverseRNGState(int num)
{
	if(curState-num > 0)
	{
		rng.state = states[curState-num];
		curState-=num;
	}
	else
	{
		rng.state = states[0];
		curState = 0;
	}
}

struct frame
{
public:
	int x;
	int y;
};

int frameCount = 0;

// the main entry point for the application is this function
void DarkGDK ( void )
{
	//srand (time(NULL));
	pcg32_srandom_r(&rng, 54u, 42u);
	dbSetDisplayMode(800,600,0);


	std::vector<gameObject> gameObjects(GAMEOBJECTS);
	

	//gameObject temp = gameObject("player.png", 50,50,20,-5);
	//gameObjects[0] = temp;
	dbLoadImage("bubble.png", 200);
	dbLoadImage("rewind.png", 300);
	dbLoadImage("pause.png", 301);
	//dbLoadImage("background.png", 0);
	//dbLoadImage("player.png", 2);
	// turn on sync rate and set maximum rate to 60 fps
	dbSyncOn   ( );
	dbSyncRate ( 15 );

	Bubble bubble;
	bubble.spriteId = 200;
	bubble.x = 1000;
	bubble.y = 1000;

	dbSprite(0,1000,1000,0);
	//dbSetSpriteAlpha(1, 50);
	dbSprite(1,1000,1000,1);
	dbSprite(200,1000,1000,200);
	dbSprite(300,1000,1000,300);
	dbSprite(301,1000,1000,301);
	dbSetSpriteAlpha(200, 75);


	int x = 1;
	int y = 1;

	int speed = 10;

	// our main loop
	int numOfObjects = 0;
	while ( LoopGDK ( ) )
	{
		if(numOfObjects < GAMEOBJECTS)
		{
			gameObject temp = gameObject("player.png");
			gameObjects[numOfObjects] = temp;
			numOfObjects++;
		}
		dbCLS();

		if(dbDownKey())
		{
			//y+=speed;
		}
		if(dbUpKey())
		{
			//y-=speed;
			dbPasteSprite(300, dbScreenWidth() - 128, 64);
		}
		if(dbLeftKey())
		{
			dbPasteSprite(300, dbScreenWidth() - 128, 64);
			for(int i = 0; i < numOfObjects;i++)
			{
				gameObjects[i].reverse(1);
			}
			//x-=speed;
		}
		if(dbRightKey())
		{
			//x+=speed;
			dbPasteSprite(300, dbScreenWidth() - 128, 64);
		}

		if(dbMouseClick())
		{
			bubble.x = dbMouseX() - (dbSpriteWidth(200) / 2);
			bubble.y = dbMouseY() - (dbSpriteHeight(200) / 2);
		}

		if(dbLeftKey() && !dbDownKey())
		{
			for(int i = 0; i < numOfObjects;i++)
			{
				gameObjects[i].reverse(1);
				gameObjects[i].draw();
			}
			//x-=speed;
		}
		else if(!dbDownKey())
		{
			for(int i = 0; i < numOfObjects; i++)
			{
				if(dbUpKey())
				{
					if(gameObjects[i].getType() == "red")
					{
						gameObjects[i].reverse(1);
					}
					else
					{
						gameObjects[i].update(bubble);
					}
				}
				else if(dbRightKey())
				{
					if(gameObjects[i].getType() == "yellow")
					{
						gameObjects[i].reverse(1);
					}
					else
					{
						gameObjects[i].update(bubble);
					}
				}
				else
				{
					gameObjects[i].update(bubble);
				}
				gameObjects[i].draw();
			}
		}
		else
		{
			for(int i = 0; i < numOfObjects; i++)
			{
				gameObjects[i].draw();
			}
			if(!dbUpKey() && !dbLeftKey() && !dbRightKey())
			dbPasteSprite(301, dbScreenWidth() - 128, 64);
		}
		dbPasteSprite(bubble.spriteId, bubble.x, bubble.y);
		//dbPasteSprite(1,x,y);
		// update the screen
		dbSync ( );
	}

	// return back to windows
	return;
}

//returns the distance between two vectors
float getDistance(int x1, int y1, int x2, int y2)
{
	float distanceX = x1-x2;
	distanceX *= distanceX;
	float distanceY = y1-y2;
	distanceY *= distanceY;

	float distance = sqrtf(distanceX+distanceY);		
	return distance;
}