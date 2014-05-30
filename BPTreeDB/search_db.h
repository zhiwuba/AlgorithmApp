/* 
**   B+Tree DB
**/
#ifndef  __SEARCH_DB_H__
#define __SEARCH_DB_H__


struct Value
{
	int     len;   
	void* value;
};

struct Index_t
{
	int    key;
	off_t child;
};

struct Record_t
{
	int       key;
	Value* value;
};

struct InnerNode
{
	off_t parent;
	off_t prev;
	off_t next;

	int              count;
	Index_t*      children;
};

struct LeafNode
{
	off_t  prev;
	off_t  next;
	off_t  parent;

	int              count;
	Record_t*    children;
};

struct Meta
{
	int  degree; 
	int  height;
	int  inner_node_num;
	int  leaf_node_num;

	int key_size;
	int value_size;
	
	long slot;
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

	
	Meta*             m_bp_meta;
	std::string        m_db_path;
	FILE*               m_db_file;

private:
	off_t alloc(InnerNode* node);
	off_t alloc(LeafNode* node);


	template<class T>
	int binary_search(T* nodes,int count, int key);
	template<class T>
	int lower_bound(T* nodes, int count, int key);
	template<class T>
	int upper_bound(T* nodes, int count, int key);
};

#endif
