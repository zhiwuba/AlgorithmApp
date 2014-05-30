#ifndef  __BP_TREE_H__
#define __BP_TREE_H__


struct Value
{
	int     len;   
	void* value;
};

struct Record
{
	int       key;
	Value* value;
};

struct Node
{
	Node*    Parent;
	int          count;

	Node**    children_node;   //指向孩子节点
	Record**  node_record;     //本节点的数据  value可能为空
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

private:
	Node* create_node();
	Node* search_index(int key);
	Node* search_leaf(Node* parent, int key);

	Meta*  m_meta;

private:
	/* Tool Functions */
	template<class T>
	int binary_search(T* nodes,int count, int key);
	template<class T>
	int lower_bound(T* nodes, int count, int key);
	template<class T>
	int upper_bound(T* nodes, int count, int key);
};



#endif