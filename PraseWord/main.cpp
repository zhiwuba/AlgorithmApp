#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <windows.h>
#include "ParseText.h"

int main()
{
	ParseText* parser=new ParseUTF8Text();

	
	long start_time=GetTickCount();
	parser->load_file_and_stat("D:\\Workspace\\App\\PraseWord\\samples\\ghost_utf8.txt");
	parser->save_result_to_file("D:\\Workspace\\App\\PraseWord\\samples\\result.txt");
	long cost_time=GetTickCount()-start_time;
	printf("cost time is :%ld ms", cost_time);


	delete parser;
	return 0;
}

