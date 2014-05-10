#ifndef  __SKIP_LIST_H__
#define __SKIP_LIST_H__


struct ListNode
{
	int            node_id;
	void*        node_data;
	ListNode* sibling_next;
	ListNode* level_next;
};



class SkipList
{
public:
	SkipList(void);
	~SkipList(void);

	void  add_node(int node_id, void* node_data);
	void* search_node(int node_id);
	void  delete_node(int node_id);
	
private:
	int  rand_level();
	ListNode*  create_node(int node_id, void* node_data);
	ListNode*  m_root_node;
};




#endif // !__SKIP_LIST_H__
