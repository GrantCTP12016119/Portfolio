// SMACrossoverTrader.cpp : main project file.

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include <math.h>
#include <sstream>

void initilaize();
void run();
int getLatestTickId();
void readMarketData(std::string fileName);
float SMA(int period = 0, int latestTickId = 0); // simple moving average = period in ticks
void findTradeSMACrossover(float SMA = 0, int latestTickId = 0);
bool sendTrade(std::string fileQueuePath, std::string symbol, std::string type, std::string amount, std::string stop, std::string limit); // send trade to queue file

const int MAXTICKS = 512;
const int SMAPERIOD = 32;
const int READDATASLEEP = 100;

const std::string SYMBOL = "EURGBP";
const std::string PSYMBOL = "EUR/GBP";
const std::string TRADEQUEUEPATH = SYMBOL + "_TRADEQUEUE.csv";

float tickdata[MAXTICKS];
float marketPriceAtLastTrade = 0.0; // keeps track of the market price at the time of taking a trade so that the trade is only taken once

int main()
{
	//sendTrade("queue.csv", "GBP/USD", "Long", "5", "1.25000", "1.30000");
	initilaize();
	run();
	return 0;
	
	
	//std::string s = "";
	//float myFloat = 1.2345;
	//s = std::to_string(myFloat);
	//s = s.substr(0, 7);
	//std::cout << s;

	initilaize();
	while (true)
	{
		readMarketData("BTCUSD.txt");
		system("cls");

		for (int i = 0; i < MAXTICKS; i++)
		{
			if (tickdata[i] != -1)
				std::cout << tickdata[i] << std::endl;
		}

		Sleep(READDATASLEEP);
	}
}

void initilaize()
{
	for (int i = 0; i < MAXTICKS; i++)
	{
		tickdata[i] = -1;
	}
}

void run()
{
	int latestTickId = 0;
	while (true)
	{
		readMarketData(SYMBOL + ".txt");

		latestTickId = getLatestTickId();
		if (latestTickId >= SMAPERIOD)
		{
			findTradeSMACrossover(SMA(SMAPERIOD, latestTickId), latestTickId);
		}

		Sleep(READDATASLEEP);
	}
}

//with multiple get line
//void readMarketData(std::string fileName)
//{
//	try
//	{
//		std::fstream file;
//		file.open(fileName, std::ios::in);
//
//		std::string line;
//		bool bStoredData = false;
//
//		while (true)
//		{
//			if (!file.eof())
//			{
//				getline(file, line);
//
//				if (line.length() == 0) // no data
//				{
//					break;
//				}
//
//				bStoredData = false;
//				for (int i = 0; i < MAXTICKS; i++)
//				{
//					if (tickdata[i] == -1)
//					{
//						tickdata[i] = atof(line.c_str());
//						bStoredData = true;
//						break;
//					}
//				}
//
//				//move tickdata along to make room for new data
//				if (!bStoredData)
//				{
//					for (int i = 0; i < MAXTICKS - 1; i++)
//					{
//						tickdata[i] = tickdata[i + 1];
//					}
//					tickdata[MAXTICKS - 1] = atof(line.c_str());
//				}
//			}
//			else
//				break;
//		}
//
//		file.close();
//
//		//clear market data file ready for new data
//		file.open(fileName, std::ios::out | std::ios::trunc);
//		file.close();
//
//	}
//	catch (std::string ex)
//	{
//		std::cout << "Failed to read market data: " << ex;
//	}
//	return;
//}

// with one getline
void readMarketData(std::string fileName)
{
	try
	{
		std::fstream file;
		std::string line;
		bool bStoredData = false;
		std::string buf = "";

		file.open(fileName, std::ios::in);

		if (!file.is_open())
		{
			throw "Failed to open file: " + fileName;
		}

		getline(file, line);

		while (true)
		{
			if (line.length() > 0)
			{
				buf = "";
				//get chars
				for (int i = 0; i < 10; i++)
				{
					if (line[i] != ';') // until delimiter is reached
					{
						buf += line[i]; // add characters to buffer
					}
					else
					{
						line = line.substr(i + 1, line.length()); // remove the buffered string from the main string
						break;
					}
				}

				bStoredData = false;
				for (int i = 0; i < MAXTICKS; i++) // look for space in tickdata and add data
				{
					if (tickdata[i] == -1)
					{
						tickdata[i] = atof(buf.c_str());
						bStoredData = true;
						break;
					}
				}

				//move tickdata along to make room for new data
				if (!bStoredData)
				{
					for (int i = 0; i < MAXTICKS - 1; i++)
					{
						tickdata[i] = tickdata[i + 1];
					}
					tickdata[MAXTICKS - 1] = atof(buf.c_str());
				}
			}
			else
				break;
		}

		file.close();

		//clear market data file ready for new data
		file.open(fileName, std::ios::out | std::ios::trunc); // delete file contents
		file.close();

	}
	catch (std::string ex)
	{
		std::cout << "Failed to read market data: " << ex;
	}
	return;
}

float SMA(int period, int latestTickId)
{
	try {
		if (period == 0 || latestTickId == 0)
		{
			throw "period = " + period + "  and   latestTickId = " + latestTickId;
		}


		//calculate SMA
		float total = 0;
		for (int i = 0; i < period; i++) // add up all ticks within period
		{
			total += tickdata[latestTickId - i];
		}

		//return average
		return total / period;
	}
	catch (std::string ex)
	{
		std::cout << "Cannot get SMA: " << ex << std::endl;
		Sleep(3000);
	}
}

int getLatestTickId()
{
	try
	{
		//find latest tick
		int latestTickId = MAXTICKS - 1; // defualt value at the end of the tickdata array, latestTickId = element of latest tick in tick data
		for (int i = 0; i < MAXTICKS; i++)
		{
			if (tickdata[i] == -1)
			{
				i--;
				latestTickId = i;
				break;
			}
		}
		return latestTickId;
	}
	catch (std::string ex)
	{
		std::cout << "Cannot find latest tick in tickdata: " << ex << std::endl;
		Sleep(3000);
		return 0;
	}
}

void findTradeSMACrossover(float SMA, int latestTickId)
{
	std::string strType, strAmount, strStop, strLimit;
	float stop, limit;
	try
	{
		if (SMA == 0 || latestTickId == 0)
		{
			throw "SMA = " + SMA + "   and   latestTickId = " + latestTickId;
		}

		if (marketPriceAtLastTrade == tickdata[latestTickId]) // then we've already sent this trade, return
			return;

		//find trades for SMA crossover
		if (tickdata[latestTickId - 1] < SMA && tickdata[latestTickId] >= SMA)
		{
			//go long
			strType = "Long";
			strAmount = "5";

			//calc stop
			stop = tickdata[latestTickId] - 0.002;

			//calc limit
			limit = tickdata[latestTickId] + 0.0005;
		}
		else if (tickdata[latestTickId - 1] > SMA && tickdata[latestTickId] <= SMA)
		{
			// go short
			strType = "Short";
			strAmount = "5";

			//calc stop
			stop = tickdata[latestTickId] + 0.002;

			//calc limit
			limit = tickdata[latestTickId] - 0.0005;
		}
		else
		{
			//no trade
			return;
		}

		std::ostringstream ssStop;
		std::ostringstream ssLimit;

		ssStop << stop;
		ssLimit << limit;

		strStop = ssStop.str();
		strLimit = ssLimit.str();

		strStop = strStop.substr(0, 7);
		strLimit = strLimit.substr(0, 7);

		//attempt to send trade until success
		while (!sendTrade(TRADEQUEUEPATH, PSYMBOL, strType, strAmount, strStop, strLimit))
		{
			continue;
		}

		marketPriceAtLastTrade = tickdata[latestTickId];
		std::cout << "Sent trade: " << SYMBOL << ", " << strType << ", " << strAmount << ", " << strStop << ", " << strLimit << std::endl;
	}
	catch (std::string ex)
	{
		std::cout << "Failed to scan for trades: " << ex << std::endl;
		Sleep(3000);
		return;
	}
}

bool sendTrade(std::string fileQueuePath, std::string symbol, std::string type, std::string amount, std::string stop, std::string limit)
{
	std::fstream file;
	bool bFileExistsAlready = false;
	try
	{
		file.open(fileQueuePath, std::ios::out | std::ios::app);
		if (!file.is_open())
		{
			throw "Failed to open file";
		}

		std::string newline = symbol + "," + type + "," + amount + "," + stop + "," + limit + "\n";
		file << newline;
		file.close();
		return true;
	}
	catch (std::string ex)
	{
		std::cout << "Failed to send trade to trade queue file: " << ex << std::endl;
		return false;
	}
}

