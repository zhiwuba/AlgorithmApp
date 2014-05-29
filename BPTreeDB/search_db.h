/* 
**   B+Tree DB
**/
#ifndef  __SEARCH_DB_H__
#define __SEARCH_DB_H__

#include "search_util.h"

struct Value
{
	int     len;   
	void* value;
};

struct Index_t
{
	int    key;
	long child;
};

struct Record_t
{
	int       key;
	Value* value;
};

struct InnerNode
{
	InnerNode* parent;
	Index_t*      children;
	int              count;
};

struct LeafNode
{
	InnerNode* parent;
	Record_t*    children;
	int              count;
};

struct Meta
{
	int  degree; 
	int  height;
	int  inner_node_num;
	int  leaf_node_num;

	int key_size;
	int value_size;

	InnerNode* root;

	long root_offset;
	long leaf_offset;
};

class Search_DB
{
public:
	Search_DB(const char* db_path);
	~Search_DB();


protected:
	int bp_tree_search(int key, Value* value);
	int bp_tree_insert(int key, Value* value);
	
	InnerNode* search_index(int key);



private:
	template<class T>
	void binary_search(T* nodes,int count, int key);
	void lower_bound();
	void upper_bound();
	
	Meta*             m_bp_tree;
	std::string        m_db_path;

};

#endif
