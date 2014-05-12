#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SkipList.h"

int main()
{
	SkipList skip_list;

	for ( int i=0; i<100; i++ )
	{
		char* data=new char[30];
		sprintf(data,"asdfasdfasd__%d", i);
		skip_list.add_node(i, data);
	}

	void* data=NULL;
	skip_list.search_node(40,&data);
	printf(" result:  %s \n", (char*)data);


	return 0;
}
