#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SkipList.h"

int main()
{
	EqualChanger<char*> changer;
	SkipList<char*, EqualChanger<char*>> skip_list(changer);

	for ( int i=0; i<100; i++ )
	{
		char* data=new char[30];
		sprintf(data,"asdfasdfasd__%d", i);
		skip_list.add_node(i, data);
	}

	char* data=NULL;
	skip_list.search_node(40,&data);
	printf(" result:  %s \n", data);

	return 0;
}
