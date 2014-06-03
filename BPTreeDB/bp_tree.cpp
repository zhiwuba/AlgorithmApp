#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include "bp_tree.h"

/*    Features
**  B+Tree 为前开后闭
**
**
**/

bool record_cmp(Record* record, int key)
{
	return record->key > key;
}


BP_Tree::BP_Tree()
{

}

BP_Tree::~BP_Tree()
{

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

	m_meta->root=create_node(NULL,false);

	return 0;
}

int BP_Tree::insert(int key, Value* value)
{
	Node* node=search_node(key);

	if ( node->count==m_meta->degree )
	{   //full
		Node* new_leaf_node=create_node(node->parent,true);
		
		int point=node->count/2;
		bool  place_left= node->node_record[point]->key >= key ;

		//new_leaf_node->children_node.resize(node->children_node.size()-point);
		//std::copy(node->children_node.begin()+point, node->children_node.end(), new_leaf_node->children_node.begin() );
		//node->children_node.erase(node->children_node.begin()+point, node->children_node.end() );

		new_leaf_node->node_record.resize(node->node_record.size()-point);
		std::copy(node->node_record.begin()+point, node->node_record.end(), new_leaf_node->node_record.begin())	;
		node->node_record.erase(node->node_record.begin()+point, node->node_record.end());

		if ( place_left )
		{
			insert_to_leaf_no_split(node,key, value);
		}
		else
		{
			insert_to_leaf_no_split(new_leaf_node,key,value);
		}

		insert_to_index(node->parent, node->node_record.back()->key);

	}
	else
	{
		insert_to_leaf_no_split(node,key,value);
	}

	return 0;
}


int BP_Tree::insert_to_index(Node* node, int key)
{
	if ( node->count==m_meta->degree )
	{
		Node* new_index=create_node(node->parent, false);

		int point=node->count/2;
		bool  place_left= node->node_record[point]->key >= key ;

		new_index->children_node.resize(node->children_node.size()-point);
		std::copy(node->children_node.begin()+point, node->children_node.end(), new_index->children_node.begin() );
		node->children_node.erase(node->children_node.begin()+point, node->children_node.end() );

		new_index->node_record.resize(node->children_node.size()-point);
		std::copy(node->node_record.begin()+point, node->node_record.end(), new_index->node_record.begin())	;
		node->node_record.erase(node->node_record.begin()+point, node->node_record.end());

		if ( place_left )
		{
			insert_to_index_no_split(node, key);
		}
		else
		{
			insert_to_index_no_split(new_index, key);
		}

		reset_index_children_parent(new_index);

		insert_to_index(node->children_node[point], node->node_record[point]->key);
	}
	else
	{
		insert_to_index_no_split(node, key);
	}

	return 0;
}

int BP_Tree::insert_to_leaf_no_split(Node* leaf, int key, Value* value)
{
	std::vector<Record*>::iterator iter;
	iter=std::lower_bound(leaf->node_record.begin(), leaf->node_record.end(), key, record_cmp);
	
	Record* record=new Record;
	record->key=key;
	record->value=value;
	leaf->node_record.insert(iter, record);
	leaf->count++;
	return 0;
}

int BP_Tree::insert_to_index_no_split(Node* index, int key)
{
	std::vector<Record*>::iterator iter;
	iter=std::lower_bound(index->node_record.begin(), index->node_record.end(), key, record_cmp);

	Record* record=new Record;
	record->key=key;
	index->node_record.insert(iter, record);
	index->count++;

	return 0;
}

int BP_Tree::reset_index_children_parent(Node* node)
{
	for ( int i=0; i<node->children_node.size(); ++i )
	{
		node->children_node[i]->parent=node;
	}
	return 0;
}

Node* BP_Tree::search_node(int key)
{
	int height=m_meta->height;
	Node* node=m_meta->root;
	while ( height>1 )
	{
		std::vector<Record*>::iterator iter;
		if ( node->node_record.size()==0 )
		{
			return node;
		}
		else
		{
			iter=std::lower_bound(node->node_record.begin(), node->node_record.end(), key,record_cmp);
			node=node->children_node[iter-node->node_record.begin()];
		}
		--height;
	}
	return node;
}


Node* BP_Tree::create_node(Node* parent , bool is_leaf)
{
	Node* node=new Node;
	node->count=0;
	node->is_leaf=is_leaf;
	node->parent=parent;

	return node;
}
