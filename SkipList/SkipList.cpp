#include "SkipList.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int kMaxLevel=8;

SkipList::SkipList(void)
{
	srand(time(0));
	m_root_node=create_node(0,NULL);
}


SkipList::~SkipList(void)
{

}

void SkipList::create_list()
{

}

void  SkipList::add_node(int node_id, void* node_data)
{
	ListNode* node=create_node(node_id,node_data);

	if ( m_root_node->level_next==NULL )
	{
		m_root_node->level_next=node;
		return;
	}
	else
	{
		ListNode* cur_node=m_root_node->level_next;
		while (  )
		{

		}
	}
}

void* SkipList::search_node(int node_id)
{
	return NULL;
}

void  SkipList::delete_node(int node_id)
{

}

ListNode*  SkipList::create_node(int node_id, void* node_data)
{
	ListNode* node=new ListNode;
	node->node_id=node_id;
	node->node_data=node_data;
	node->sibling_next=node->level_next=NULL;
	return node;
}

int  SkipList::rand_level()
{
	int level=1;
	while( (rand()%2) )level++;
	level=level>kMaxLevel?kMaxLevel:level;
	return level;
}

