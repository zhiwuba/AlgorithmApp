#include "ParseText.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

ParseText::ParseText()
{

}

ParseText::~ParseText()
{

}

int ParseText::load_file(const char* file_path)
{
	assert(file_path!=NULL);
	FILE* file=fopen(file_path,"rb");
	if ( file!=NULL )
	{


		fclose(file);
	}
	return 0;
}
