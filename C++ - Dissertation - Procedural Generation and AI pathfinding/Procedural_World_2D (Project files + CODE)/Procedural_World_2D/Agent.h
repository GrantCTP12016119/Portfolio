class Agent
{
private:
	int spriteId;
	int spriteId2;
	int x, y; // x and y cordinates
	int gridX, gridY; // tile cordinates
	node pathNodes[GRX*GRY]; // path node array with elements size equal to the number of grid locations

	int costOfPath;

	int startX, startY;

	int goalNodeX;
	int goalNodeY;

	bool reachedGoal;
	int targetNodeI;
	bool findNextNode;
	int curPathLen;

	bool stayOnContours;
	bool placeBlockOrGoal;
	bool placePheromone;
public:
	bool forceRecalc;
	Agent()
	{
		//default constructor
	}
	Agent(int imageId, int imageId2) // image id 2 for under water sprite
	{
		dbSprite(spriteCounter, 1000,1000, imageId);
		spriteId = spriteCounter;
		dbSizeSprite(spriteId, SS*2,SS*2);
		spriteCounter++;

		dbSprite(spriteCounter, 1000,1000, imageId2);
		spriteId2 = spriteCounter;
		dbSizeSprite(spriteId2, SS*2,SS*2);
		spriteCounter++;

		dbSprite(101, 1000, 1000, 101);
		//blockland sprite id = 101
		dbSizeSprite(101, SS,SS);

		dbSprite(102, 1000, 1000, 102);
		//blockwater sprite id = 102
		dbSizeSprite(102, SS,SS);

		//pheromone sprite id  = 300
		dbSprite(300,1000,1000,300);
		dbSizeSprite(300,SS,SS);

		x = 25;
		y = 25;

		costOfPath = 0;

		stayOnContours = false;
		placeBlockOrGoal = false;
		placePheromone = false;

		gridX = (x - (x % SS)) / SS; // gets the position on the grid
		gridY = (y - (y % SS)) / SS;

		goalNodeX = 10;
		goalNodeY = 10;

		clearNodes();

		reachedGoal = false;
		findNextNode = true;
		forceRecalc = false;

	}
	~Agent()
	{
	}
	void toggleStayOnContours()
	{
		stayOnContours = !stayOnContours;
	}
	void togglePlacePheromone()
	{
		placePheromone = !placePheromone;
	}
	int getPathCost()
	{
		return costOfPath;
	}
	void setStayOnContours(bool val)
	{
		stayOnContours = val;
	}
	void toggleMouseAction()
	{
		placeBlockOrGoal = !placeBlockOrGoal;
	}
	void update()
	{
		gridX = (x - (x % SS)) / SS; // gets the position on the grid
		gridY = (y - (y % SS)) / SS;

		if(dbMouseClick() && dbMouseY() < dbScreenHeight() - 150)
		{
			if(placePheromone)
			{
				//place a pheromone
				int targetNodeX = (dbMouseX() - (dbMouseX() % SS)) / SS;
				int targetNodeY = (dbMouseY() - (dbMouseY() % SS)) / SS;
				nodes[targetNodeX][targetNodeY].hasPheromone = true;
				//if the node has a block on it then remove it
				if(nodes[targetNodeX][targetNodeY].flag == 'b')
				{
					nodes[targetNodeX][targetNodeY].flag = 'n';
				}
				forceRecalc = true;
			}
			else if(!placeBlockOrGoal)
			{
				//set goal node
				clearNodes();
				goalNodeX = (dbMouseX() - (dbMouseX() % SS)) / SS;
				goalNodeY = (dbMouseY() - (dbMouseY() % SS)) / SS;
				reachedGoal = false;
				findNextNode = true;
			}
			else
			{
				//place a block
				int targetNodeX = (dbMouseX() - (dbMouseX() % SS)) / SS;
				int targetNodeY = (dbMouseY() - (dbMouseY() % SS)) / SS;
				nodes[targetNodeX][targetNodeY].flag = 'b';
				forceRecalc = true;
			}
		}

		if(nodes[goalNodeX][goalNodeY].flag == 'n')
		{
			calcPathAStar(gridX, gridY, goalNodeX, goalNodeY);
		}
		if(forceRecalc)
		{
			clearNodes();
			calcPathAStar(gridX, gridY, goalNodeX, goalNodeY);
			forceRecalc = false;
		}

		//move agent through path
		if(!reachedGoal)
		{
			if(!findNextNode)
			{
				//move to target node
				moveTo(pathNodes[targetNodeI].x, pathNodes[targetNodeI].y);
				if(getDistance(x,y,pathNodes[targetNodeI].x,pathNodes[targetNodeI].y) < 10)
				{
					if(pathNodes[targetNodeI].flag == '*')
					{
						reachedGoal = true;
					}
					pathNodes[targetNodeI].flag = '/'; // flag the node as visited
					findNextNode = true; // tell the algorithm to find the next node to follow
				}
			}
			else
			{
				//filter through to find the next node in the order
				findNextNode = false;
				for(int i = curPathLen-1; i > 0 ; i--)
				{
					if(pathNodes[i].flag == 'p' || pathNodes[i].flag == '*')
					{
						targetNodeI = i;
						break;
					}
				}
			}
		}
	}
	void draw()
	{
		//draw path nodes
		for(int i = 0; i < GRX; i++)
		{
			for(int j = 0; j < GRY; j++)
			{
				if(nodes[i][j].flag == 'p')
				{
					dbPasteSprite(10,i*SS,j*SS - (dbSpriteHeight(10) - SS));
				}
				if(nodes[i][j].flag == 'b')
				{
					if(nodes[i][j].g < 16)
					{
						//paste block sprite
						dbPasteSprite(101,i*SS,j*SS - (dbSpriteHeight(10) - SS));
					}
					else
					{
						dbPasteSprite(102,i*SS,j*SS - (dbSpriteHeight(10) - SS));
					}
				}
				if(nodes[i][j].hasPheromone)
				{
					dbPasteSprite(300,i*SS,j*SS - (dbSpriteHeight(10) - SS));
				}
			}
		}
		if(nodes[gridX][gridY].g == terrain->waterCost || stayOnContours == true && nodes[gridX][gridY].g <= -3)
		{
			//draw agent
			dbPasteSprite(spriteId2, x,y);
		}
		else
		{
			//draw under water sprite
			dbPasteSprite(spriteId, x,y);
		}
	}

	void setParentNodes(int offsetX, int offsetY, int curX, int curY) // offset of neighbour, x-1 = one to the left
	{
	int distance;
	if(nodes[curX+offsetX][curY+offsetY].flag == 'b')
	{
		return;
	}
	if(nodes[curX+offsetX][curY+offsetY].hasPheromone)
	{
		//apply the pheromone levels to grid locations within a radius of 10 from the pheromone on the x and y axis
		int pheromoneLevel = 1000;
		nodes[curX+offsetX][curY+offsetY].pher = pheromoneLevel;
		for(int i = 0; i < 10; i++)
		{
			if(curX+offsetX-i >= 0)
			{
				nodes[curX+offsetX-i][curY+offsetY].pher += pheromoneLevel - ((pheromoneLevel*0.1)*i);
			}
			if(curX+offsetX+i <= GRX)
			{
				nodes[curX+offsetX+i][curY+offsetY].pher += pheromoneLevel - ((pheromoneLevel*0.1)*i);
			}
			if(curY+offsetY-i >= 0)
			{
				nodes[curX+offsetX][curY+offsetY-i].pher += pheromoneLevel - ((pheromoneLevel*0.1)*i);
			}
			if(curY+offsetY+i <= GRY)
			{
				nodes[curX+offsetX][curY+offsetY+i].pher += pheromoneLevel - ((pheromoneLevel*0.1)*i);
			}
		}
	}
	if(nodes[curX+offsetX][curY+offsetY].flag == 'n' || nodes[curX+offsetX][curY+offsetY].flag == 'o')
	{
		if(nodes[curX+offsetX][curY+offsetY].flag == 'o')
			{
			if(nodes[curX+offsetX][curY+offsetY].g-nodes[curX+offsetX][curY+offsetY].pher > getDiff(nodes[curX+offsetX][curY+offsetY].g-nodes[curX+offsetX][curY+offsetY].pher, nodes[curX][curY].g-nodes[curX][curY].pher))
			{
				//calculate the heuristic
				distance = getDistance(curX+offsetX, curY+offsetY, goalNodeX, goalNodeY);
				nodes[curX+offsetX][curY+offsetY].h = distance;
				if(stayOnContours)
				{
					nodes[curX+offsetX][curY+offsetY].g = getDiff(nodes[curX+offsetX][curY+offsetY].g, nodes[startX][startY].g); // uses the difference between the height of the node and the current hieight of the agent
				}
				//with this method the agent will try to stay on the same level along the way, hills, contour lines, patches of grass and avoid descending and ascending just to cross a small grass land when the goal node is
				//on a elevated area anyway
				nodes[curX+offsetX][curY+offsetY].flag = 'o'; // set the node to open
				nodes[curX+offsetX][curY+offsetY].parentX = curX; // store the parent node array cords
				nodes[curX+offsetX][curY+offsetY].parentY = curY;
			}
		}
		else
		{
			//calculate the heuristic
			distance = getDistance(curX+offsetX, curY+offsetY, goalNodeX, goalNodeY);
			nodes[curX+offsetX][curY+offsetY].h = distance;
			if(stayOnContours)
			{
				nodes[curX+offsetX][curY+offsetY].g = getDiff(nodes[curX+offsetX][curY+offsetY].g, nodes[startX][startY].g);
			}
			nodes[curX+offsetX][curY+offsetY].flag = 'o'; // set the node to open
			nodes[curX+offsetX][curY+offsetY].parentX = curX; // store the parent node array cords
			nodes[curX+offsetX][curY+offsetY].parentY = curY;
		}
	}
}

int calcPathAStar(int StartX, int StartY, int endX, int endY)
{
	bool pathFound = false;
	int curX, curY; // current location
	curX = startX = StartX;
	curY = startY = StartY;
	int gCost = 99999;
	int count = 0;

	while(!pathFound)
	{
		//set the flag of the new node being analyzed to closed
		nodes[curX][curY].flag = 'c';
		//open neighbouring nodes and set the parents
		if(curX > 0)
		{
			setParentNodes(-1,0, curX, curY);
		}
		if(curX < GRX-1)
		{
			setParentNodes(1,0, curX, curY);
		}
		if (curY > 0)
		{
			setParentNodes(0,-1, curX, curY);
		}
		if(curY < GRY-1)
		{
			setParentNodes(0,1, curX, curY);
		}
		count++;

		//if the node is the goal node then set it to a path node and exit the loop
		if (curX == endX && curY == endY || count > GRX*GRY-1)
		{
			int count = 0;
			int order = GRX*GRY;
			
			costOfPath = 0;

			nodes[curX][curY].flag = 'p';
			pathFound = true;

			pathNodes[count] = nodes[curX][curY];
			pathNodes[count].flag = '*';
			count++;

			//trace back and select path
			while(true)
			{
				costOfPath += nodes[curX][curY].g;
				curX = nodes[curX][curY].parentX;
				curY = nodes[curX][curY].parentY;
				nodes[curX][curY].flag = 'p';

				//add node to path node array
				pathNodes[count] = nodes[curX][curY];
				pathNodes[count].flag = 'p';

				count++;
				curPathLen = count;
				if(curX == startX && curY == startY)
				{
					return 0;
				}
				if(count > GRX*GRY-1)
					return 0;
			}
			//continue;
		}

		gCost = 99999;
		//find the next open node to analyze
		for(int i = 0; i < GRX; i++)
		{
			for(int j = 0; j < GRY; j++)
			{
				//if the node has a lesser cost and is in the open list
				//g movement cost + heuristic - pheromone level
				if(nodes[i][j].g+nodes[i][j].h-nodes[i][j].pher < gCost && nodes[i][j].flag == 'o')
				{
					//set it as the next node to analyze
					gCost = nodes[i][j].g+nodes[i][j].h - nodes[i][j].pher;
					curX = i;
					curY = j;
				}
			}
		}
	}
	return 0;
}

void clearNodes(bool resetCosts = false, bool clearBlocks = false)
{
	for(int i = 0; i < GRX; i++)
	{
		for(int j = 0; j < GRY; j++)
		{
			if(resetCosts)
			{
				nodes[i][j].g = 0;
				nodes[i][j].h = 0;
			}
			if(clearBlocks)
			{
				nodes[i][j].pher = 0; // always clear pheromone level when clearBlocks is true
			}
			if(nodes[i][j].hasPheromone == true && clearBlocks == true)
			{
				nodes[i][j].hasPheromone = false;
			}
			if(nodes[i][j].flag == 'b' && clearBlocks == true) // clear blocks if the boolean has been passed to do so
			{
				nodes[i][j].flag = 'n';
			}
			else
			{
				if(nodes[i][j].flag != 'b')
				{
					nodes[i][j].flag = 'n';
				}
			}
			nodes[i][j].parentX = gridX;
			nodes[i][j].parentY = gridY;
		}
	}

	for(int i = 0; i < GRX*GRY; i++)
	{
		pathNodes[i].flag = 'n';
	}
}

	void moveTo(int X, int Y) // for moving the agent toward a vector
	{
		if(getDistance(x,y,X,Y) > 4)
		{
			float dx = X-x;
			float dy = Y-y;
			float length = sqrtf(dx*dx+dy*dy);
			dx/=length;
			dy/=length;
			

			x += dx*8;
			y += dy*8;
		}
	}
};