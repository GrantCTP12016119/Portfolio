#include "Button.h"

class Panel // UI Panel
{
private:
	int x,y;
	int spriteId;
	Button *settingButton1; // mountaineer button
	Button *settingButton2; // amphibian button
	Button *settingButton3; // prefers grass button

	Button *contourLinesButton;

	Button *mouseSettingButton;
	Button *pheromoneButton;

	Button *newSeedButton;

public:
	Panel()
	{
	}
	Panel(int X, int Y)
	{
		x = X;
		y = Y;
		dbLoadImage("textures/panel.png", 898);
		dbSprite(898, 1000,1000,898);

		settingButton1 = new Button("textures/settingButton1.png","textures/hlSettingButton1.png",x+20,y+40);
		settingButton2 = new Button("textures/settingButton2.png","textures/hlSettingButton2.png",x+110,y+40);
		settingButton3 = new Button("textures/settingButton3.png","textures/hlSettingButton3.png",x+200,y+40);

		contourLinesButton =  new Button("textures/contourButton.png","textures/hlContourButton.png",x+110,y+90);
		contourLinesButton->setHighlighted(false);
		settingButton3->setHighlighted(true);

		mouseSettingButton = new Button("textures/toggleButton1hl1.png", "textures/toggleButton1hl2.png", x+410,y+40);
		mouseSettingButton->setHighlighted(false);

		pheromoneButton = new Button("textures/pheromoneButton.png","textures/hlPheromoneButton.png", x+310, y+40);
		pheromoneButton->setHighlighted(false);

		newSeedButton = new Button("textures/newSeedButton.png", "textures/newSeedButton.png", x+580, y+80);

		terrain->mountainCost = 30;
		terrain->grassCost = 0;
		terrain->waterCost = 96;
		terrain->shallowCost = 32;
		terrain->hillCost = 16;
	}
	~Panel()
	{
		delete settingButton1;
		delete settingButton2;
		delete settingButton3;

		delete contourLinesButton;
		delete mouseSettingButton;
		delete pheromoneButton;
		delete newSeedButton;
	}
	void checkCollision(int mouseX, int mouseY)
	{
		if(newSeedButton->checkButtonPress(mouseX, mouseY))
		{
			terrain->generateWorldSeed();
		}
		if(mouseSettingButton->checkButtonPress(mouseX, mouseY))
		{
			mouseSettingButton->toggleHighlighted();
			agent1->toggleMouseAction();
		}
		if(pheromoneButton->checkButtonPress(mouseX, mouseY))
		{
			pheromoneButton->toggleHighlighted();
			agent1->togglePlacePheromone();
		}
		if(settingButton1->checkButtonPress(mouseX, mouseY))
		{
			settingButton1->setHighlighted(true);
			settingButton2->setHighlighted(false);
			settingButton3->setHighlighted(false);
			contourLinesButton->setHighlighted(false);
			agent1->setStayOnContours(false);
			terrain->setStayOnContours(false);

			terrain->mountainCost = 0; // configure the grid type cost
			terrain->grassCost = 30;
			terrain->waterCost = 96;
			terrain->shallowCost = 32;
			terrain->hillCost = 16;
		}
		if(settingButton2->checkButtonPress(mouseX, mouseY))
		{
			settingButton2->setHighlighted(true);
			settingButton1->setHighlighted(false);
			settingButton3->setHighlighted(false);
			contourLinesButton->setHighlighted(false);
			agent1->setStayOnContours(false);
			terrain->setStayOnContours(false);

			terrain->mountainCost = 96;
			terrain->grassCost = 30;
			terrain->waterCost = 0;
			terrain->shallowCost = 16;
			terrain->hillCost = 32;
		}
		if(settingButton3->checkButtonPress(mouseX, mouseY))
		{
			settingButton3->setHighlighted(true);
			settingButton2->setHighlighted(false);
			settingButton1->setHighlighted(false);
			contourLinesButton->setHighlighted(false);
			agent1->setStayOnContours(false);
			terrain->setStayOnContours(false);

			terrain->mountainCost = 30;
			terrain->grassCost = 0;
			terrain->waterCost = 96;
			terrain->shallowCost = 32;
			terrain->hillCost = 16;
		}
		if(contourLinesButton->checkButtonPress(mouseX, mouseY))
		{
			contourLinesButton->setHighlighted(true);
			agent1->setStayOnContours(true);
			terrain->setStayOnContours(true);
			settingButton3->setHighlighted(false);
			settingButton2->setHighlighted(false);
			settingButton1->setHighlighted(false);
		}
	}
	void draw() // draw the panel with all information and configuration buttons
	{
		dbPasteSprite(898,x,y);
		
		dbSetCursor(x+121, y+13);
		dbPrint("Setting");
		settingButton1->draw();
		settingButton2->draw();
		settingButton3->draw();

		contourLinesButton->draw();

		dbSetCursor(x+390, y+13);
		dbPrint("Goal Node / Blocker");
		mouseSettingButton->draw();

		dbSetCursor(x+300, y+13);
		dbPrint("Pheromone");
		pheromoneButton->draw();

		newSeedButton->draw();

		dbSetCursor(x+550,y+10);
		dbPrint("Gradients: "); 
		dbSetCursor(x+660,y+10);
		dbPrint((double)terrain->grads);

		dbSetCursor(x+550,y+20);
		dbPrint("posX: ");
		dbSetCursor(x+660,y+20);
		dbPrint((double)terrain->posX);
			
		dbSetCursor(x+550,y+30);
		dbPrint("posY: ");
		dbSetCursor(x+660,y+30);
		dbPrint((double)terrain->posY);

		dbSetCursor(x+550,y+40);
		dbPrint("viewOffsetX: ");
		dbSetCursor(x+660,y+40);
		dbPrint((double)terrain->viewOffsetX);
			
		dbSetCursor(x+550,y+50);
		dbPrint("viewOffsetY: ");
		dbSetCursor(x+660,y+50);
		dbPrint((double)terrain->viewOffsetY);

		dbSetCursor(x+550,y+60);
		dbPrint("Cost of path: ");
		dbSetCursor(x+660,y+60);
		dbPrint((double)agent1->getPathCost());
	}
};