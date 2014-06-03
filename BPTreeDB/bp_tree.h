#ifndef  __BP_TREE_H__
#define __BP_TREE_H__

#include <vector>
#include <algorithm>

struct Value
{
	int     len;   
	void* value;
};


struct Node
{
	Node*    parent;

	bool       is_leaf;
	int          key;
	Value*    value;     //节点的数据 非叶子节点为空

	int          count;
	std::vector<Node*>   children;
};



struct Meta
{
	int  degree; 
	int  height;
	int  inner_node_num;
	int  leaf_node_num;

	int key_size;
	int value_size;

	Node*  root;  //root节点
};



class BP_Tree
{
public:
	BP_Tree();
	~BP_Tree();

	int  init_meta();
	int  insert(int key, Value* value);
	int  search(int key, Value* value);
	int  update(int key, Value* value);
	int  remove(int key);

	void print_tree(Node* root);
	
	Meta*  m_meta;

private:
	Node* create_node(Node* parent, bool is_leaf);
	Node* search_node(int key);
	Node* search_leaf(Node* parent, int key);



private:

	int insert_to_leaf_no_split(Node* leaf, int key, Value* value);
	int insert_to_index(Node* parent, Node* new_node);
	int insert_to_index_no_split(Node* parent, Node* new_node);
	int reset_index_children_parent(Node* node);

	Node* get_sibling_node(Node* node, bool left);
	bool borrow_node(Node* lender, Node* borrower);
	bool merge_node(Node* from, Node* to);
};



#endif