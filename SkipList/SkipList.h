//SkipList的实现 参考如下网址
//http://www.cnblogs.com/xuqiang/archive/2011/05/22/2053516.html


#ifndef  __SKIP_LIST_H__
#define __SKIP_LIST_H__


struct ListNode
{
	int              key;
	void*          value;
	ListNode** next;         //一个一维数组
};

class SkipList
{
public:
	SkipList(void);
	~SkipList(void);

	bool  add_node(int key, void* value);
	bool  search_node(int key, void** value);
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
