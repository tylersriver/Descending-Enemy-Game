// This program demonstrates animation, keyboard control, and sprites
#include "DarkGDK.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;
const int FRAME_RATE = 60;
// Identifiers required for each image we load.
// These identifiers are used to refer to the image after it's
// been loaded in memory.
const int FOREST_IMAGE_ID = 1;
const int BALL_IMAGE_ID = 11;
const int FOX_IMAGE_ID = 3;
const int FUDD_IMAGE_ID = 12;

// Identifiers required for each sprite. These identifiers are used
// to manipulate images that are used as sprites
const int BALL_SPRITE_ID = 11;
const int FOX_SPRITE_ID = 2;

struct fox
	{
		int foxX;
		int foxY;
		int speed;
		bool falling;
		bool deleted;
		int ID;
	};

void createImages();
void moveCannon(double& endX, double& endY, double& realAngle, double& angle, double cannonLength);
bool bombDeleted(double bombX, double bombY);
void moveBomb(double& bombX, double& bombY, double launchAngle, double endX, double endY, double time);
void createFoxes(fox foxes[]);
void displayFoxes(fox foxes[], int &numLives);
void checkCollisions(fox foxes[], int &numKills, int &numLevel);
void moveFoxes(fox foxes[], int numLevel);
void randMove(fox foxes[]);
bool allFalling(fox foxes[]);
//***********************allFalling(fox foxes[]);*************************
// DarkGDK function                              *
//************************************************
const int numFoxes = 10;

void DarkGDK()
{
	double launchAngle;
	double cannonLength = 60.0;
	double realAngle = 45.0;
	double angle = realAngle*(3.141592/180.0);
	double startX = 0.0;
	double startY = dbScreenHeight();
	double endX = cannonLength * cos(angle);
	double endY = dbScreenHeight() - (cannonLength * sin(angle));
	int numLives = 10;
	char lives[2];
	int numLevel = 1;
	char level[1];
	int numKills = 0;
	char kills[2];
	int count = 1;

	double bombX = endX;
	double bombY = endY;
	bool bombPresent = false;
	bool spaceBar = false;
	double time = 0.0;
	
	createImages();
	dbSyncOn();
	dbSyncRate(FRAME_RATE);

	
	int randomNums[10];
	fox foxes[10];
	createFoxes(foxes);

	while( LoopGDK())
	{
		if(numLives > 0 && numKills < 20)
		{
			dbPasteImage(FOREST_IMAGE_ID, 0, 0);
			dbLine(startX, startY, endX, endY);
			moveCannon(endX, endY, realAngle, angle, cannonLength);

	
			displayFoxes(foxes, numLives);
			checkCollisions(foxes, numKills, numLevel);
			if( count%120 == 0)
				randMove(foxes);
			count++;
			moveFoxes(foxes, numLevel);

			if(dbSpaceKey())
			{
					spaceBar = true;
			}
			if(spaceBar && !bombPresent)
				{
					time = 0;
					bombX = endX;
					bombY = endY;
					launchAngle = angle;
					dbSprite(BALL_SPRITE_ID, bombX, bombY, BALL_IMAGE_ID);
					bombPresent = true;
				}
			if(bombDeleted(bombX, bombY))
				{
					dbDeleteSprite(BALL_SPRITE_ID);
					spaceBar = false;
					bombPresent = false;
				}
			else if (bombPresent)
				{
					time += .15;
					moveBomb(bombX, bombY, launchAngle, endX, endY, time);
					dbSprite(BALL_SPRITE_ID, bombX, bombY, BALL_IMAGE_ID);
				}
	
			dbText(5, 5, "Level: ");
			dbText(5, 25, "Kills: ");
			dbText(5, 45, "Lives: ");
			itoa(numLives, lives, 10);
			dbText(60, 45, lives);
			itoa(numLevel, level, 10);
			dbText(60, 5, level);
			itoa(numKills, kills, 10);
			dbText(60, 25, kills);
		}

		else
		{
			if(numKills == 20)
			{
				dbPasteImage(FUDD_IMAGE_ID, 0, 0);
				dbText(20, 40, "NO ONE WILL EVER KNOW");
				dbText(60, 70, "WHAT THE FOX SAYS...");
			}
			else 
				dbText(240, dbScreenHeight()/2, "FOXES WON THIS TIME.");
		}
		dbSync();
	}
}

void createImages()
{
	dbLoadImage("stars.jpg", FOREST_IMAGE_ID);
	dbLoadImage("ball.png", BALL_IMAGE_ID);
	dbLoadImage("alien.png", FOX_IMAGE_ID);
	dbLoadImage("fudd.jpg", FUDD_IMAGE_ID);
}

void checkCollisions(fox foxes[], int &numKills, int &numLevel)
{
	int count = 0;
	
	for(int i = 0; i < numFoxes; i++)
	{
		if(dbSpriteCollision(BALL_SPRITE_ID, foxes[i].ID))
		{
			dbDeleteSprite(foxes[i].ID);
			foxes[i].deleted = true;
			numKills++;
			count ++;
		}

	}

	if(numKills % 10 == 0 && count != 0)
	{	
		numLevel ++;
		count = 0;
		createFoxes(foxes);
	}
}
void moveCannon(double& endX, double& endY, double& realAngle, double& angle, double cannonLength)
{
	if( dbLeftKey() && realAngle <= 87 )
		realAngle++;
	if( dbRightKey() && realAngle >= 3)
		realAngle--;
	angle = realAngle*(3.141592/180);
	endX = cannonLength * cos(angle);
	endY = dbScreenHeight() - (cannonLength * sin(angle));
}	
void moveBomb(double& bombX, double& bombY, double launchAngle, double endX, double endY, double time)
{
	double speed = 93.0;
	double gravity = 12;
	double vBombX = speed * cos(launchAngle);
	double vBombY = speed * sin(launchAngle);
	
	bombX = vBombX * time + endX;
	bombY = endY - (vBombY * time) + (.5 * gravity * (time * time));
	
}
bool bombDeleted(double bombX, double bombY)
{
	bool gone = false;
	if( bombX > dbScreenWidth() )
		gone = true;
	if( bombY > dbScreenHeight() )
		gone = true;
	return gone;
}

void createFoxes(fox foxes[])
{
	double x = 114;
	double idNumber = 1;
	for(int i = 0; i < numFoxes; i++)
	{
		foxes[i].foxX = x;
		foxes[i].foxY = 5;
		foxes[i].speed = 30;
		foxes[i].ID = idNumber;
		foxes[i].deleted = false;
		foxes[i].falling = false;
		idNumber++;
		x += 52;
	}
}
void displayFoxes(fox foxes[], int &numLives)
{
		for(int i = 0; i < numFoxes; i++)
		{
			if(foxes[i].foxY > dbScreenHeight())
			{
				numLives--;
				foxes[i].foxY = 0;
				//foxes[i].deleted = true;
			}
			if(!foxes[i].deleted)
				dbSprite(foxes[i].ID, foxes[i].foxX, foxes[i].foxY, FOX_IMAGE_ID);
		}	
}
void moveFoxes(fox foxes[], int numLevel)
{
	int speed = 3;
	if(numLevel ==2)
		speed = 5;

	for(int i = 0; i < numFoxes; i++)
	{
		if(foxes[i].falling && !foxes[i].deleted)
			foxes[i].foxY += speed;
	}
}
bool allFalling(fox foxes[])
{
	bool c = true;
	for (int i = 0; i < numFoxes; i++)
	{
		if(!foxes[i].falling)
			c = false;
	}
	return c;
}

void randMove(fox foxes[])
{
	unsigned int seedValue = time(0);
	srand(seedValue);
	
	int randFox = rand()%10;
	while(foxes[randFox].falling && !allFalling(foxes))
		randFox = rand()%10;
	foxes[randFox].falling = true;
}

