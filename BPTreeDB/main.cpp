#include <stdlib.h>
#include <stdio.h>

#include "bp_tree.h"



int main()
{
	BP_Tree* tree=new BP_Tree;
	tree->init_meta();

	for ( int i=0 ;i<100; ++i )
	{
		tree->insert(i,NULL);
	}
	
	return 0;
}

