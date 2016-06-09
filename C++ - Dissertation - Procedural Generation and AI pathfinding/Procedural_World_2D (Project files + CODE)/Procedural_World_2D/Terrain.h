class Terrain
{
private:
	int CALCX_START;
	int CALCY_START;

	pcg32_random_t world;

	long long worldSeed;

public:

	bool stayOnContours;

	int grads;

	int waterCost;
	int shallowCost;
	int grassCost;
	int hillCost;
	int mountainCost;

	int posX;
	int posY;

	long long offsetX;
	long long offsetY;

	int viewOffsetX;
	int viewOffsetY;

	double g[CALCX][CALCY];

	Terrain()
	{
		CALCX_START;
		CALCY_START;

		posX = 0;
		posY = 0;

		stayOnContours = false;

		waterCost = 48;
		shallowCost = 16;
		grassCost = 0;
		hillCost = 8;
		mountainCost = 15;

		viewOffsetX = 0;
		viewOffsetY = 0;

		offsetX = 0;
		offsetY = 0;

		grads = 0;
	}

	~Terrain()
	{
	}

	void toggleStayOnContours()
	{
		stayOnContours = !stayOnContours;
	}
	void setStayOnContours(bool val)
	{
		stayOnContours = val;
	}

	void generateWorldSeed()
	{
		worldSeed = pcg32_random() % 100000000;
		world.state = worldSeed;
	}

	void perlin(int freq, int amp)
	{
	//calculate point of origin and calculation bounds

	CALCX_START = 0;//posX - (posX % F)-F;
	CALCY_START = 0;//posY - (posY % F)-F;
	
	//these stay as F because the offset needs to know the maximum size of the lattices
	offsetX = posX-(posX % F) - F;
	offsetY = posY-(posY % F) - F;

	grads = 0;
	for(int i = CALCX_START;i<CALCX;i++)
	{
		for(int j = CALCY_START;j<CALCY;j++)
		{
			//g[i][j] = -90.0;
			if((i) % freq == 0 && (j) % freq == 0)
			{
				g[i][j] += RNG(i+offsetX,j+offsetY,amp);
				grads++;
			}
		}
	}

	for(int i = CALCX_START;i<CALCX;i++)
	{
		
		for(int j = CALCY_START;j<CALCY;j++)
		{
			double FF = freq;
			//calculate the displacement of the offset (gradient offset)
			int gox = 0;//offsetX % F;
			int goy = 0;//offsetY % F;

			// if there is already data here it must be a gradient so skip it
			if((i) % freq == 0 && (j) % freq == 0)
			{
			}
			else
			{

				//truncate to find the lower lattice
				int xc = (i - (i % freq));
				int yc = (j - (j % freq));

				//apply the gradient offset so we are on a gradient value
				//xc -= gox;
				//yc -= goy;

				//obtain the 4 gradients of the region
				double Q11 = g[xc][yc];
				double Q12 = g[xc][yc+freq];
				double Q21 = g[xc+freq][yc];
				double Q22 = g[xc+freq][yc+freq];

				//interpolation
				// Q11 /   Q21  /   Q12  /   Q22
				//xc yc/ xc+4 yc/ xc yc+4/ xc+4 yc+4

				//find the coordinates taking into account the gradient offset
				int xm = i-gox;
				int ym = j-goy;
				//Q
				//mx my

				double Q1;
				double Q2;

				//cosine interpolation
				double m = (xm - xc) / ((xc+FF) - xc);

				double angle = m * PI;
				double a = (1 - cos(angle)) * 0.5;

				Q1 = Q11+(a*(Q21-Q11));

				Q2 = Q12+(a*(Q22-Q12));

				//   Q1 /	 Q2
				// xm yc / xm yc+4 

				//Z 
				m = (ym - yc) / ((yc+FF) - yc);
				angle = m * PI;
				a = (1 - cos(angle)) * 0.5;
				int z = Q1+(a*(Q2-Q1));

				//apply the interpolated value to the gradient
				g[i-gox][j-goy] = z;
			}
		}
	}
}

void drawTerrain()
{
	int px,py;
	px = SS;
	py = SS;

	VIEWX_START = F + viewOffsetX;
	VIEWY_START = F + viewOffsetY;

	VIEWX_END = VIEWX_START + F;
	VIEWY_END = VIEWY_START + F;

	// view size is 32 by 32

	int printX = 0;
	int printY = 0;

	for(int i = VIEWX_START;i<VIEWX_END;i++)
	{
		for(int j = VIEWY_START;j<VIEWY_END;j++)
		{
			// we need to store all of the gradient vectors as nodes...
			//nodes[i-VIEWX_START][j-VIEWY_START].g = g[i][j]; // store the g/height value of the nodes
			//store the on screen cords of all nodes
			nodes[i-VIEWX_START][j-VIEWY_START].x = (printX)*(px);
			nodes[i-VIEWX_START][j-VIEWY_START].y = (printY)*(py);

			//between -10 - 9
			int spriteId = 0;
			if(g[i][j] <= -3)
			{
				nodes[i-VIEWX_START][j-VIEWY_START].g = waterCost;
				spriteId = 2;
			}
			else if (g[i][j] >= -2 && g[i][j] <= -1)
			{
				nodes[i-VIEWX_START][j-VIEWY_START].g = shallowCost;
				spriteId = 3;
			}
			else if(g[i][j] >= 0 && g[i][j] <= 11)
			{
				nodes[i-VIEWX_START][j-VIEWY_START].g = grassCost;
				spriteId = 5;
			}
			else if(g[i][j] >= 12 && g[i][j] <= 13)
			{
				nodes[i-VIEWX_START][j-VIEWY_START].g = hillCost;
				spriteId = 1;
			}
			else if(g[i][j] >= 14)
			{
				nodes[i-VIEWX_START][j-VIEWY_START].g = mountainCost;
				spriteId = 4;
			}
			if(stayOnContours)
			{
				nodes[i-VIEWX_START][j-VIEWY_START].g = g[i][j]*16; // set the g cost to that of the perlin data
			}
			dbPasteSprite(spriteId,(printX)*(px),((printY)*(py)) - (dbSpriteHeight(spriteId) - SS));
			printY++;
		}
		printX++;
		printY = 0;
	}
}

double RNG(int x, int y, int range)
{
	srand(x);
	x = rand() % 10000+1000;
	srand(y);
	
	y = rand() % 10000+1000;

	srand(worldSeed);
	
	x++;y++;

	int a = rand() % x + 1;
	int b = rand() % y + 1;

	int seed = (a+b);// * worldSeed;

	srand(seed);

	double randomNum = rand() % (range*2) - range;
	//double finalValue = rndomNum / 10;

	return randomNum;
}
};
