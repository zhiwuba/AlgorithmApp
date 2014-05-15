#include "CrawlData.h"
#include <stdio.h>
#include <io.h>

CrawlData::CrawlData(void)
{
}


CrawlData::~CrawlData(void)
{
}

int CrawlData::traverse_directory( const char* directory )
{
	_finddata_t fileinfo;
	long handle=_findfirst(directory,&fileinfo);
	if ( handle != -1 )
	{
		while ( !_findnext(handle, &fileinfo) )
		{
			printf("%s .\n",  fileinfo.name);
		}
		_findclose(handle);
	}

	return 0;
}





