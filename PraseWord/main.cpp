#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <windows.h>
#include "ParseText.h"

int main()
{
	long start_time=GetTickCount();
	ParseText parser;
	parser.load_file_and_stat("D:\\Workspace\\App\\PraseWord\\samples\\ghost_utf8.txt");
	parser.save_result_to_file("D:\\Workspace\\App\\PraseWord\\samples\\result.txt");
	long cost_time=GetTickCount()-start_time;
	printf("cost time is :%ld ms", cost_time);
	return 0;
}

