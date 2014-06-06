/* 
**   B+Tree DB
**/
#ifndef  __SEARCH_DB_H__
#define __SEARCH_DB_H__

#include <vector>
#include <algorithm>

typedef  int      key_t;
typedef  void*  value_t;

const  int kOffSize=sizeof(off_t);
const  int kRootOffSet= 1000;

struct Record 
{
	int     key;
	off_t  offset;
};

struct Node
{
	bool       is_leaf;
	int          key;
	void*      value;
	off_t       offset;
	off_t       parent;
	std::vector<Record>   children;
};


class Search_DB
{
public:
	Search_DB(const char* db_path, int value_size);
	~Search_DB();

	int  insert(int key,   value_t* value);
	int  search(int key,  value_t* value);
	int  update(int key, value_t* value);
	int  remove(int key);

	void print();

private:
	/* search */
	off_t  search_node(int key);

	/* insert */
	int insert_to_leaf_no_split(Node* leaf, int key, value_t* value);
	int insert_to_index(Node* parent, Node* new_node);
	int insert_to_index_no_split(Node* parent, Node* new_node);

	/* remove */
	Node* get_sibling_node(Node* node, bool left);
	bool    borrow_node(Node* lender, Node* borrower, bool from_left);
	bool    merge_node(Node* from, Node* to, bool to_left);
	bool    reset_nodes_after_remove(Node* node);

	/* print */
	void    print_level(Node* node, int cur_level ,int level);

	/* node */
	Node* create_node(Node* parent, bool is_leaf);
	
	void    update_key(Node* node);  /* update self and parent's key */
	void    update_link(Node* node);  /* update self and children's link */
	
	int      map(Node* node, off_t offset);    
	int      unmap(Node* node, off_t offset);

	int      map_root();
	int      unmap_root();

	off_t   alloc_node(bool is_leaf);
	
private:
	Node*  m_root;
	int        m_degree;
	int        m_height;
	int        m_node_count;
	int        m_leaf_count;
	int        m_value_size;
	off_t     m_slot;

	std::string        m_db_path;
	FILE*               m_db_file;
};

#endif
