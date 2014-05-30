#include <stdio.h>
#include <stdlib.h>


#include "bp_tree.h"

/*    Features
**  B+Tree 为前开后闭
**
**
**/

BP_Tree::BP_Tree()
{

}

BP_Tree::~BP_Tree()
{

}


int BP_Tree::insert(int key, Value* value)
{
	Node* parent=search_index(key);
	Node* leaf=search_leaf(parent,key);



	return 0;
}



int BP_Tree::init_meta()
{
	m_meta=new Meta;
	m_meta->degree=20;
	m_meta->height=1;

	m_meta->inner_node_num=0;
	m_meta->leaf_node_num=0;
	m_meta->key_size=0;
	m_meta->value_size=0;

	m_meta->root=create_node();

	return 0;
}

Node* BP_Tree::search_index(int key)
{
	int height=m_meta->height;
	Node* node=m_meta->root;
	while ( height>1 )
	{
		int i=lower_bound(node->node_record, node->count, key);
		node=node->children_node[i];
		--height;
	}
	return node;
}


Node* BP_Tree::search_leaf(Node* parent, int key)
{
	int i=lower_bound( parent->node_record, parent->count, key);
	Node* node=parent->children_node[i];
	return node;
}

Node* BP_Tree::create_node()
{
	Node* node=new Node;
	node->count=0;
	node->Parent=NULL;
	node->node_record=NULL;
	node->children_node=NULL;

	return node;
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

template<class T>
int Search_DB::lower_bound(T* nodes, int count, int key)
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
	return start;
}

template<class T>
int Search_DB::upper_bound(T* nodes, int count, int key)
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
	return end;
}

