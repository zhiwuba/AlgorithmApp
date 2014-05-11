#ifndef  __SKIP_LIST_H__
#define __SKIP_LIST_H__


struct ListNode
{
	int            level;
	int            node_id;
	void*        node_data;
	ListNode* sibling_next;
	ListNode* level_next;
};

//http://www.cnblogs.com/xuqiang/archive/2011/05/22/2053516.html

class SkipList
{
public:
	SkipList(void);
	~SkipList(void);

	void  create_list();
	void  add_node(int node_id, void* node_data);
	void* search_node(int node_id);
	void  delete_node(int node_id);
	
private:
	int  rand_level();
	ListNode*  create_node(int node_id, void* node_data);

	ListNode*  m_root_node;

};




#endif // !__SKIP_LIST_H__
