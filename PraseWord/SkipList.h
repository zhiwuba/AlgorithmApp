#ifndef  __SKIP_LIST_H__
#define __SKIP_LIST_H__


struct ListNode
{
	int              key;
	int              value;
	ListNode** next;         //一个一维数组
};

class SkipList
{
public:
	SkipList(void);
	~SkipList(void);

	bool  add_node(int key, int value);
	bool  search_node(int key, int* value);
	bool  delete_node(int key);
	
private:
	ListNode*  create_node(int key, void* value, int level);
	void          create_list();
	int            rand_level();

	ListNode*      m_header;
	ListNode*      m_nil;
	unsigned int  m_level;
};

#endif // !__SKIP_LIST_H__
