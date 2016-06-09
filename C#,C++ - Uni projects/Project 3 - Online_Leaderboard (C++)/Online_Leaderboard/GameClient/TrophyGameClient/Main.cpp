#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream> 
#pragma comment(lib, "wsock32.lib")

//structure for socket buffer
struct SOCKETBUF
{
public:
	SOCKET sock;
	char buf[100];
	int res;
};

//class for storing high score entries
class Entry
{
public:
	std::string name;
	std::string score;
	int iScore;

	Entry()
	{
		name = "";
		//entries are not dynamic meaning that there is always
		//100 of them, here score is set to -1 to indicate an
		//unused entry
		score = "-1";
		iScore = -1;
	}
	~Entry()
	{
	}
};

//static allocation of entries array
Entry etrs[100];

int recvTimeout(SOCKETBUF *sb, int timeout);

//function for recieving data from server
DWORD WINAPI recieve_data(void *lpParam);

// function for maintaining connection to server
void serverComms();

//useful function for converting integer to char*
char* intToChar(int num);

SOCKET sock;

#include "DarkGDK.h"

//DWORD thread;

//HANDLE threadHandle = NULL;

WSADATA wsaData;
int res;

int ret;
bool threadCreated;

sockaddr_in ser;
sockaddr addr;
int timer = 0;

int state = 1;

char hiscrData[512];

//states
void titleScreen();
void play();
void gameOver();
void highScores();

	int score = 0;
	
	//variables for gameplay-
	int gameTimer = 500;
	int delay = 250;
	int coolDown = 100;
	//-

	int targetX = 50;
	int targetY = 5;

	bool mousePrevPressed = false;

	std::string name;

	bool retrievedScores = false;
	bool connectionEstablished = false;

// the main entry point for the application is this function
void DarkGDK ( void )
{
		//have user enter name
	
	//dbSetCursor(dbScreenWidth()/2, dbScreenHeight()/2);
	//dbSync();
	dbPrint("Please enter your name: ");
	dbSetCursor(196, 0);
	name = dbInput();
	name.append(";");
	name.insert(0,";");


	srand (time(NULL)); // set random generator seed

	//load assets
	dbLoadImage("playButton.png", 1);
	dbLoadImage("highScoresButton.png", 2);
	dbLoadImage("submitScoreButton.png", 3);
	dbLoadImage("mainMenuButton.png", 4);
	dbLoadImage("target.png", 10); // target

	dbSprite(1,1000,1000,1);
	dbSprite(2,1000,1000,2);
	dbSprite(3,1000,1000,3);
	dbSprite(4,1000,1000,4);
	dbSprite(10,1000,1000,10);

	ret = WSAStartup(0x101, &wsaData); // use highest version of winsock avaliable

	if (ret != 0)
	{ 
		return;
	}

	threadCreated = false;

	//configuration for server connection
	ser.sin_family = AF_INET;
	//ser.sin_addr.s_addr = inet_addr("165.120.57.181");
	ser.sin_addr.s_addr = inet_addr("82.38.113.95");
	ser.sin_port = htons(11111);

	memcpy(&addr, &ser, sizeof(SOCKADDR_IN));


	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		return;
	}
	//std::cout << "Socket created\n";

	res = connect(sock, &addr, sizeof(addr));
	if (res != 0)
	{
		connectionEstablished = false;
	}
	else
	{
		connectionEstablished = true;
	}

	// main loop

	// turn on sync rate and set maximum rate to 60 fps
	dbSyncOn   ( );
	dbSyncRate ( 60 );
	while ( LoopGDK ( ) )
	{
		// update the screen
		serverComms();
		if(state == 1)
		{
			titleScreen();
		}
		else if(state == 2)
		{
			play();
		}
		else if(state == 3)
		{
			gameOver();
		}
		else if(state == 4)
		{
			highScores();
		}

		if(dbMouseClick())
		{
			mousePrevPressed = true;
		}
		else
		{
			mousePrevPressed = false;
		}

		if(dbEscapeKey())
		{
			send(sock,"!terminate", 10,0);
		}

		//Sleep(5);
		//res = send(sock, buf, sizeof(buf), 0);
		dbSync ( );
	}

	closesocket(sock);
	WSACleanup();

	// return back to windows
	return;
}

void deserialiseData() // deserialise the data retireved from the server
{
	if(hiscrData[0] == ';') // high score table empty
	{
		return;
	}
	int y = 0;
	int j = 0;
	for(int i = 0;i < 100; i++)
	{
		std::string curName = "";
		std::string curScore = "";
		for(; hiscrData[y]!=',';y++)
		{
			if(hiscrData[y] == ';')
			{
				return;
			}
			curName += hiscrData[y];
			j = y+2;
		}
		for(; hiscrData[j]!=',';j++)
		{
			if(hiscrData[j] == ';')
			{
				return;
			}
			curScore += hiscrData[j];
			y = j+2;
		}
		etrs[i].score = curScore;
		etrs[i].name = curName;

		if(hiscrData[y] == ';')
		{
			break;
		}
	}
}

void highScores()
{
	//draw highscores
	if(connectionEstablished) // only if a connection was established
	{
		if(!retrievedScores)
		{
			//send retrieve request
			SOCKETBUF sb;
			sb.sock = sock;
			send(sock,"!retrieve", 20, 0);
			strcpy_s(sb.buf, "/");
			res = recvTimeout(&sb, 2);
			strcpy_s(hiscrData, sb.buf);
			
			//store header of recieved command
			char header[6] = {sb.buf[0], sb.buf[1], sb.buf[2], sb.buf[3], sb.buf[4], sb.buf[5]};
			//data starts at i = 6 as the first 6 characters is a header command so that
			//the client knows what to do with the recieved data
			for (int i = 0; i < 94; i++)
			{
				hiscrData[i] = sb.buf[i+6];
			}

			if (strstr(header, "!hiscr"))
			{
				retrievedScores = true;
				//dbPrint(data);
				deserialiseData();
			}
		}
	}
	else
	{
		dbSetCursor(dbScreenWidth()/2 - 90, dbScreenHeight()/2-64);
		dbPrint("Not connected to server");
	}

	if(connectionEstablished)
	{
		dbSetCursor(dbScreenWidth()/2 - 64, 4);
		dbPrint("High Scores");
		dbSetCursor(dbScreenWidth()/2, dbScreenHeight()/2);
		//dbPrint(hiscrData);

		//display high score table
		dbSetCursor(dbScreenWidth()*0.33-32, dbScreenHeight()/5-32);
		dbPrint("RANK");
		dbSetCursor(dbScreenWidth()/2-32, dbScreenHeight()/5-32);
		dbPrint("PLAYER");
		dbSetCursor(dbScreenWidth()*0.66-32, dbScreenHeight()/5-32);
		dbPrint("SCORE");

		for(int i = 0; i < 100; i++)
		{
			if(etrs[i].score != "-1")
			{
				dbSetCursor(dbScreenWidth()*0.33-32, dbScreenHeight()/5+(16*i));
				dbPrint(double(i+1));

				dbSetCursor(dbScreenWidth()/2-32, dbScreenHeight()/5+(16*i));
				char* cst = new char[etrs[i].name.length() + 1];
				strcpy(cst, etrs[i].name.c_str()); 
				dbPrint(cst);

				dbSetCursor(dbScreenWidth()*0.66-32, dbScreenHeight()/5+(16*i));
				char* cst2 = new char[etrs[i].score.length() + 1];
				strcpy(cst2, etrs[i].score.c_str());
				dbPrint(cst2);

				delete [] cst;
				delete [] cst2;
			}
		}
	}

		dbPasteSprite(4, dbScreenWidth()/2 - (dbSpriteWidth(4)/2), dbScreenHeight()/2 - (dbSpriteHeight(4)/2) + 128);

		if(dbMouseClick() && mousePrevPressed == false)
		{
			if(dbMouseX() > dbScreenWidth()/2 - (dbSpriteWidth(4)/2) && 
				dbMouseX() < dbScreenWidth()/2 + (dbSpriteWidth(4)/2) && 
				dbMouseY() > dbScreenHeight()/2 - (dbSpriteHeight(4)/2 + 128) && 
				dbMouseY() < dbScreenHeight()/2 + (dbSpriteHeight(4)/2) + 128)
			{
				//exit(1);
				//play game
				state = 1;
			}
		}

}

void titleScreen()
{
	dbPasteSprite(1, dbScreenWidth()/2 - (dbSpriteWidth(1)/2), dbScreenHeight()/2 - (dbSpriteHeight(1)/2));
	dbPasteSprite(2, dbScreenWidth()/2 - (dbSpriteWidth(2)/2) , (dbScreenHeight()/2+ 96 - (dbSpriteHeight(2)/2)));

	//check for button clicks
	if(dbMouseClick() && mousePrevPressed == false)
	{
		if(dbMouseX() > dbScreenWidth()/2 - (dbSpriteWidth(1)/2) && 
			dbMouseX() < dbScreenWidth()/2 + (dbSpriteWidth(1)/2) && 
			dbMouseY() > dbScreenHeight()/2 - (dbSpriteHeight(1)/2) && 
			dbMouseY() < dbScreenHeight()/2 + (dbSpriteHeight(1)/2))
		{
			//exit(1);
			//play game
			state = 2;
			score = 0;
		}
		else if(dbMouseX() > dbScreenWidth()/2 - (dbSpriteWidth(2)/2) && 
			dbMouseX() < dbScreenWidth()/2 + (dbSpriteWidth(2)/2) && 
			dbMouseY() > dbScreenHeight()/2 - (dbSpriteHeight(2)/2) +96 && 
			dbMouseY() < dbScreenHeight()/2 + (dbSpriteHeight(2)/2) +96)
		{
			//high score display
			state = 4;
			retrievedScores = false;
		}
		//else if () // check for high scores button click
	}
}

void play()
{
	dbSetCursor(5,5);
	dbPrint("SCORE:");
	dbSetCursor(64, 5);
	dbPrint(double(score));

	if(gameTimer < 0)
	{
		//timer = 500 - delay;
		//gameOver
		state = 3;
		delay = 200;
		gameTimer = 500 - delay;
		coolDown = 100;
		//submit score
	}
	gameTimer--;
	
	if(coolDown > 0)
	{
		coolDown--;
	}
	else
	{
		dbPasteSprite(10,targetX,targetY);

		if(dbMouseClick() && mousePrevPressed == false)
		{
			if(dbMouseX() > targetX && 
				dbMouseX() < targetX + dbSpriteWidth(10) &&
				dbMouseY() > targetY &&
				dbMouseY() < targetY + dbSpriteHeight(10))
			{
				//clicked on the target
				score++;
				if(delay < 381)
				{
					delay+=10;
				}
				gameTimer = 500 - delay;


				targetX = rand() % (dbScreenWidth() - 32);
				targetY = rand() % (dbScreenHeight() - 32);
				coolDown = 100;
			}
			else
			{
				//gameOver
				state = 3;
				delay = 200;
				gameTimer = 500 - delay;
				coolDown = 100;
				//submit score
				//char* num = {intToChar(score)};
				std::stringstream str;
				str << score;
				std::string dataStr(str.str());
				dataStr.append(name);
				dataStr.insert(0, "!score");
				const char* data = dataStr.c_str();
				send(sock, data, 100,0);
			}
		}
	}
}

void gameOver()
{
	dbSetCursor(dbScreenWidth()/2 - 32, dbScreenHeight()/2 - 128);
	dbPrint("GAME OVER!");
	dbSetCursor(dbScreenWidth()/2 - 32, dbScreenHeight()/2 - 112);
	dbPrint("Your Score: ");
	dbSetCursor(dbScreenWidth()/2 - 32, dbScreenHeight()/2 - 96);
	dbPrint(double(score));

	dbPasteSprite(4, dbScreenWidth()/2 - (dbSpriteWidth(4)/2), dbScreenHeight()/2 - (dbSpriteHeight(4)/2));
	//dbPasteSprite(3, dbScreenWidth()/2 - (dbSpriteWidth(3)/2) , (dbScreenHeight()/2+ 96 - (dbSpriteHeight(3)/2)));

		//check for button clicks
	if(dbMouseClick() && mousePrevPressed == false)
	{
		if(dbMouseX() > dbScreenWidth()/2 - (dbSpriteWidth(3)/2) && 
			dbMouseX() < dbScreenWidth()/2 + (dbSpriteWidth(3)/2) && 
			dbMouseY() > dbScreenHeight()/2 - (dbSpriteHeight(3)/2) && 
			dbMouseY() < dbScreenHeight()/2 + (dbSpriteHeight(3)/2))
		{
			//exit(1);
			//play game
			state = 1;
		}
		//else if () // check for high scores button click
	}
}

void serverComms()
{
		if ((timer / 100) > 10 && connectionEstablished) // only continue if there is a connection
		{
			timer = 0;
			char knock[100] = "!knockknock";
			res = send(sock, knock, sizeof(knock), 0);
		}
		timer++;

		//if the server stops recieving these pings from a particular client
		//then it will eventually assume the client has lost connection
		//and terminate the thread assigned to that client
}

int recvTimeout(SOCKETBUF *sb, int timeout)
{
	DWORD thread;
	HANDLE threadHandle;
	int timer = 0;
	threadHandle = CreateThread(NULL, 0, recieve_data, sb, 0, &thread);
	//std::cout << "Recieveing data..." << std::endl;
	while (strstr(sb->buf, "/"))
	{
		Sleep(10);
		timer++;
		if ((timer / 100) > timeout)
		{
			TerminateThread(threadHandle, 0);
			return 1;
		}
	}
	TerminateThread(threadHandle, 0);
	return 0;
}

DWORD WINAPI recieve_data(void *lpParam)
{
	static SOCKETBUF* sockbuf = (SOCKETBUF*)lpParam;
	sockbuf->res = recv(sockbuf->sock, sockbuf->buf, sizeof(sockbuf->buf), 0);
	return 0;
}

//convert int to char*
char* intToChar(int num)
{
	char* data = NULL;
	itoa(num, data, 10);
	return data;
}