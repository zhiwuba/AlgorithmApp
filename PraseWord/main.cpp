#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "ParseText.h"

int main()
{
	ParseText parser;
	parser.load_file_and_stat("D:\\Workspace\\App\\PraseWord\\samples\\fighting_utf8.txt");
	parser.save_result_to_file("D:\\Workspace\\App\\PraseWord\\samples\\result.txt");

	return 0;
}

