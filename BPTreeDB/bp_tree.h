#ifndef  __BP_TREE_H__
#define __BP_TREE_H__

#include <vector>
#include <algorithm>

struct Value
{
	int     len;
	void* value;
};

class Node
{
public:
	Node();
	~Node();
	static Node* create_node(Node* parent, bool is_leaf);
	
	void  update_key();  /* update self and parent's key */
	void  update_link();  /* update selef and childrens's link */

	int    serialize(FILE* file, long offset);
	int    deserialize(FILE* file, long offset);

	Node*    parent;
	bool       is_leaf;
	int          key;
	Value*    value;
	std::vector<Node*>   children;

private:
	long     offset_;
};

class BP_Tree
{
public:
	BP_Tree();
	~BP_Tree();

	int  insert(int key, Value* value);
	int  search(int key, Value* value);
	int  update(int key, Value* value);
	int  remove(int key);

	void print();

private:
	/* search */
	Node* search_node(int key);

	/* insert */
	int insert_to_leaf_no_split(Node* leaf, int key, Value* value);
	int insert_to_index(Node* parent, Node* new_node);
	int insert_to_index_no_split(Node* parent, Node* new_node);

	/* remove */
	Node* get_sibling_node(Node* node, bool left);
	bool    borrow_node(Node* lender, Node* borrower, bool from_left);
	bool    merge_node(Node* from, Node* to, bool to_left);
	bool    reset_nodes_after_remove(Node* node);

	/* print */
	void    print_level(Node* node, int cur_level ,int level);

	Node*  m_root;
	int        m_degree;
	int        m_height;
	int        m_node_count;
	int        m_leaf_count;
};

#endif
