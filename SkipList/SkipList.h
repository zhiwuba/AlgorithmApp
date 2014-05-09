#ifndef  __SKIP_LIST_H__
#define __SKIP_LIST_H__


struct ListNode
{
	int           id;
	void*       data;
	ListNode* next;
	ListNode* index;
};



class SkipList
{
public:
	SkipList(void);
	~SkipList(void);

	void  add_data(int id, void* data);
	void* search_data(int id);


private:

	ListNode*  m_top_node;
};




#endif // !__SKIP_LIST_H__
