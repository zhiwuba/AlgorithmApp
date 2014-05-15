#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "ParseDocument.h"
#include "CrawlData.h"

int main()
{
	CrawlData crawler;
	crawler.traverse_directory("D:\\Workspace\\App\\*.*");

	long startTime=GetTickCount();
	ParseDocument parser;
	parser.Parse("D:\\Workspace\\MySqlite\\sqlite3.h");
	long timeCost=GetTickCount()-startTime;
	printf("cost time : %d \n", timeCost);
	return 0;
}

