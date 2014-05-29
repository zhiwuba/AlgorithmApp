#include <stdlib.h>
#include <assert.h>
#include <list>
#include <algorithm>

#include "search_db.h"

using std::swap;
using std::binary_search;
using std::lower_bound;
using std::upper_bound;



int Search_DB::bp_tree_search(int key, Value* value )
{
	return 0;
}

int Search_DB::bp_tree_insert( int key, Value* value )
{


	return 0;
}


InnerNode* Search_DB::search_index(int key)
{   //在所有inner节点里查找
	int height=m_bp_tree->height;
	InnerNode* node=m_bp_tree->root;
	while ( height>1 )
	{
		
		height--;
	}

	return 0;
}


template<class T>
int Search_DB::binary_search(T* nodes,int count, int key)
{
	int start=0;
	int end=count-1;
	while( start<=end )
	{
		int mid=(end+start)/2;
		if ( nodes[mid]->key == key )
		{
			return mid;
		}
		else if ( nodes[mid]->key < key )
		{
			start=mid+1;
		}
		else
		{
			end=mid-1;
		}
	}
	return -1;
}




