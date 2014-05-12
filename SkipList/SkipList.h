//SkipList的实现 参考如下网址
//http://www.cnblogs.com/xuqiang/archive/2011/05/22/2053516.html
#ifndef  __SKIP_LIST_H__
#define __SKIP_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int kMaxLevel=8;

template<class V>
struct ListNode
{
	int   key;
	V   value;
	ListNode** next;         //一个一维数组
};

template<class V>
class EqualChanger
{
public:
	bool operator()(V& dest,V& src)
	{
		dest=src;
		return true;
	}
};

template<class V, class Changer>
class SkipList
{
public:
	explicit SkipList(Changer changer);
	~SkipList(void);

	bool  add_node(int key, V value);
	bool  search_node(int key, V* value);
	bool  delete_node(int key);
	
private:
	ListNode<V>*  create_node(int key, V value, int level);
	void          create_list();
	int            rand_level();

	ListNode<V>*      m_header;
	ListNode<V>*      m_nil;
	Changer               m_changer_func;
	unsigned int  m_level;
};

template<class V, class Changer>
SkipList<V,Changer>::SkipList(Changer changer):m_changer_func(changer)
{
	srand(time(0));
	m_level=0;
	create_list();
}

template<class V,class Changer>
SkipList<V,Changer>::~SkipList(void)
{

}

template<class V,class Changer>
void SkipList<V,Changer>::create_list()
{
	m_nil=new ListNode<V>;
	m_nil->key=0x7fffffff; //int 最大值
	m_nil->value=0;
	m_nil->next=0;

	m_header=create_node(0,0, kMaxLevel);
	for ( int i=0; i<kMaxLevel; i++ )
	{
		m_header->next[i]=m_nil;
	}
}

template<class V,class Changer>
bool  SkipList<V,Changer>::add_node(int key, V data)
{
	//获取插入位置数组update
	ListNode<V>*  update[kMaxLevel];

	ListNode<V>*  p=m_header;
	ListNode<V>*  q;

	int cur_level=m_level;	
	for ( ;cur_level>=0; --cur_level )
	{
		while ( q=p->next[cur_level], q->key<key )
		{
			p=q;
		}
		update[cur_level]=p;
	}

	//已经存在
	if ( q->key==key )
	{
		m_changer_func(q->value, data);
		//q->value=data;
		return false;
	}

	//随机生成一个数
	cur_level=rand_level();
	if ( cur_level>m_level )
	{
		cur_level=++m_level;
		update[cur_level]=m_header; //在header的后面添加数据
	}

	//生成节点
	ListNode<V>* node=create_node(key,data,cur_level);
	for ( ; cur_level>=0 ; --cur_level )
	{
		p=update[cur_level];
		node->next[cur_level]=p->next[cur_level];
		p->next[cur_level]=node;
	}

	return true;
}

template<class V,class Changer>
bool SkipList<V,Changer>::search_node(int key, V* value)
{
	ListNode<V>* q;
	ListNode<V>* p=m_header;
	int  cur_level=m_level;

	for ( ; cur_level>=0 ; --cur_level  )
	{
		while ( q=p->next[cur_level], q->key<key )
		{
			p=q;
		}
	}
	if ( q->key==key )
	{
		*value=q->value;
		return true;
	}
	else
	{
		value=NULL;
		return false;
	}
}

template<class V,class Changer>
bool  SkipList<V,Changer>::delete_node(int key)
{
	ListNode<V>* aim;  // 要删除的目标node
	ListNode<V>* p=m_header;
	ListNode<V>* update[kMaxLevel];
	int cur_level=m_level;

	// 获取update数组
	for ( ; cur_level>=0 ; --cur_level )
	{
		while ( aim=p->next[cur_level] ,aim->key < key )
		{
			p=aim;
		}
		update[cur_level]=p;
	}

	if ( aim->key==key )
	{   //找到再删除
		for (cur_level=0; cur_level<m_level&&((p=update[cur_level])->next[cur_level]==aim); ++cur_level )
		{
			p->next[cur_level]=aim->next[cur_level];
		}
		delete p->value;
		delete p;

		//维护调表高度
		cur_level=m_level;
		while ( m_header->next[cur_level]==m_nil&&cur_level>0 )
		{
			cur_level--;
		}
		m_level=cur_level;
		return true;
	}
	else
	{
		return false;
	}
}

template<class V,class Changer>
ListNode<V>*  SkipList<V,Changer>::create_node(int key, V data, int level)
{
	ListNode<V>* node=new ListNode<V>();
	node->key=key;
	m_changer_func(node->value, data);
	node->next=new ListNode<V>*[level];
	return node;
}

template<class V,class Changer>
int  SkipList<V,Changer>::rand_level()
{
	int level=1;
	while( (rand()%2) )level++;
	level=level>kMaxLevel?kMaxLevel:level;
	return level;
}


#endif // !__SKIP_LIST_H__
