#include <stdafx.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <iostream> 
#include <string>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#pragma comment(lib, "wsock32.lib")

struct SOCKETBUF
{
public:
	SOCKET sock;
	char buf[100];
	int res;
};

//class to store and handle high score entries
class Entry
{
public:
	std::string name;
	std::string score;
	int iScore;

	Entry()
	{
		name = "";
		score = "-1";
		iScore = -1;
	}
	~Entry()
	{
	}
	void recordScore(std::string Name, std::string Score)
	{
		score = Score;
		name = Name;
		std::stringstream str;
		str << Score;
		int sc;
		str >> sc;
		iScore = sc;
	}
	void dispalyScore()
	{
		if(iScore!=-1)
		{
			std::cout << name << "  -  " << score << std::endl;
		}
	}
	void clear()
	{
		name = "";
		score = "-1";
		iScore = -1;
	}
};

Entry etrs[100];

//recieve commands and data from clients
//main thread assigned to each client
DWORD WINAPI recieve_cmds(LPVOID lpParam);
DWORD WINAPI recieve_data(void* lpParam);
int recvTimeout(SOCKETBUF *sb, int timeout);


DWORD WINAPI update(LPVOID lpParam);

//functions for handling scores
void saveScores();
void loadScores();
void clearScores();
//-

//check if a file exists
bool fileExists(const std::string& filename)
{
    struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

void clearScores()
{
	for(int i = 0; i <100;i++)
	{
		etrs[i].clear();
	}
}

void loadScores()
{
	//retireve the highscore data from the save file
	if(!fileExists("scores.txt"))
	{
		std::cout << "No scores to load\n";
		return;
	}
	std::ifstream fs;
	fs.open("scores.txt");
	std::string hiscrData;
	std::getline(fs, hiscrData);

	//std::cout << hiscrData << std::cout;

	clearScores();

	if(hiscrData[0] == ';') // high score table empty
	{
		std::cout << "No scores to load\n";
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
				std::cout << "Scores loaded\n";
				return;
			}
			curName += hiscrData[y];
			j = y+2;
		}
		for(; hiscrData[j]!=',';j++)
		{
			if(hiscrData[j] == ';')
			{
				std::cout << "Scores loaded\n";
				return;
			}
			curScore += hiscrData[j];
			y = j+2;
		}
		etrs[i].recordScore(curName, curScore);

		if(hiscrData[y] == ';')
		{
			std::cout << "Scores loaded\n";
			break;
		}
	}
}

void saveScores()
{
	//save score to file
	std::string scoreData = "";
	//store all of the data into a string
	for(int i = 0; i < 100;i++)
	{
		if(etrs[i].iScore != -1)
		{
			scoreData += etrs[i].name;
			scoreData += ",";
			scoreData += etrs[i].score;
			scoreData += ",";
		}
	}
	scoreData.append(";");

	//save to file
	std::ofstream fs;
	fs.open("scores.txt");
	fs.clear();
	fs << scoreData;
	fs.close();
}

void displayScores()
{
	for(int i = 0;i<100;i++)
	{
		etrs[i].dispalyScore();
	}
}

//sort scores from highest to lowest
void sortScores()
{
	for(int i = 0; i < 100; i++)
	{
		for(int y = 0;y < 99; y++)
		{
			if(etrs[y+1].iScore > etrs[y].iScore && etrs[y+1].iScore!=-1)
			{
				Entry temp = etrs[y];
				etrs[y] = etrs[y+1];
				etrs[y+1] = temp;
			}
			if(etrs[y+1].iScore == -1)
			{
				break;
			}
		}
	}
}

int main()
{
	std::cout << "Starting up\n";
	
	SOCKET sock;

	DWORD thread;

	WSADATA wsaData;
	sockaddr_in server;

	int ret = WSAStartup(0x101, &wsaData); // use highest version of winsock avaliable

	if (ret != 0)
	{
		return 0;
	}
//
	//server configuration
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY; 
	server.sin_port = htons(11111);

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		return 0;
	}
	std::cout << "Socket created\n";

	if (bind(sock, (sockaddr*)&server, sizeof(server)) != 0)
	{
		return 0;
	}
	std::cout << "Binding complete\n";

	if (listen(sock, 5) != 0)
	{
		return 0;
	}
	std::cout << "Listening...\n";

	SOCKET client;
	sockaddr_in from;
	int fromlen = sizeof(from);

	//load scores
	std::cout << "Loading latest scores...\n";
	loadScores();
	sortScores();

	//CreateThread(NULL, 1024, update, 0,0, &thread);
	while (true)
	{
		//listen for new client connections
		client = accept(sock, (struct sockaddr*)&from, &fromlen); // stays in this function until client connects
		std::cout << "Client Connected\n";

		//create thread for client
		CreateThread(NULL, 1024, recieve_cmds, (LPVOID)client, 0, &thread);

		//break;
	}

	closesocket(sock);
	WSACleanup();

	return 0;

}

DWORD WINAPI update(LPVOID lpParam)
{
	std::cout << "Update Thread started\n";
	//load scores
	std::cout << "Loading latest scores...";
	loadScores();


	int timer = 61;
	while(true)
	{
		if(timer > 60)
		{
			std::cout << "Saving scores\n";
			saveScores();
			std::cout << "Scores saved\n";
			timer = 0;
		}
		//timer++;
	}
}

// main thread for clients
//each client is assigned one by the server
DWORD WINAPI recieve_cmds(LPVOID lpParam)
{
	std::cout << "Thread created\n";
	// set our socket to the socket passed in the parameter
	SOCKET current_client = (SOCKET)lpParam;
	// CreateThread(NULL, 0, timeout, &sockbufT, 0, &timeoutThread);

	// buffer to hold our recieved data

	// buffer to hold our sent data
	//char sendData[100];

	//for error checking
	int res = 0;
	int timer = 0;
	static SOCKETBUF sockbuf;
	sockbuf.sock = current_client;

	while (true)
	{
		char buf[100] = "";
		//recv/send cmds

		//if (sockbufT.timeout == true)
		strcpy_s(sockbuf.buf, "/");
		res = recvTimeout(&sockbuf, 60);
		if (res == 1)
		{
				std::cout << "Client timed out, terminating thread" << std::endl;
				closesocket(current_client);
				ExitThread(0);
		}

		strcpy_s(buf, sockbuf.buf);
		res = sockbuf.res;
		//store header of recieved command
		char header[6] = {buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]};
		//data starts at i = 6
		char data[100];
		for (int i = 0; i < 94; i++)
		{
			data[i] = buf[i+6];
		}

		if(strstr(header, "!score"))
		{
			//store name then score after the ;
			std::string name = "";
			std::string score = "";
			int y;
			for(int i = 0;data[i]!=';';i++)
			{
				score += data[i];
				y = i+2;
			}
			for(;data[y]!=';';y++)
			{
				name += data[y];
			}

			//store the score details in class instance
			for(int i = 0; i < 100; i++)
			{
				if(etrs[i].iScore == -1)
				{
					etrs[i].recordScore(name,score);
					break;
				}
			}
			sortScores();

			//save scores
			std::cout << "Score submitted by " << name << ": " << score << std::endl;
			std::cout << "Saving scores...\n";
			saveScores();
			std::cout << "Scores saved\n";

			displayScores();


		}
		else if (strstr(buf, "!retrieve"))
		{
			std::cout << "!retrieve" << std::endl;
			std::string scoreData = "";
			//score all of the data into a string
			for(int i = 0; i < 100;i++)
			{
				if(etrs[i].iScore != -1)
				{
					scoreData += etrs[i].name;
					scoreData += ",";
					scoreData += etrs[i].score;
					scoreData += ",";
				}
			}
			//insert the header
			scoreData.insert(0, "!hiscr");
			//termiante the string
			scoreData.append(";");
			//send the data
			res = send(current_client, scoreData.c_str(), scoreData.length()+1, 0);
			std::cout << "sent scores to client!" << std::endl;
		}
		else if (strstr(buf, "!terminate"))
		{
			closesocket(current_client);
			std::cout << "Client ended session, Thread closed\n";
			ExitThread(0);
			break;
		}
		else if (strstr(buf, "!knockknock")) // recieve validation of client connection
		{
			char validate[100] = "whosthere?";
			res = send(current_client, validate, sizeof(validate), 0);
			std::cout << buf << std::endl;
		}
		else
		{
			std::cout << buf;// << std::endl;
			strcpy_s(buf, "/");
		}
	}
	return 0;
}

//two functions for handling incoming data from clients
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