#ifndef   __TOP_HEAP_H__
#define  __TOP_HEAP_H__


struct HeapNode
{
	int  data;

	HeapNode* left_child;
	HeapNode* right_child;
};


class TopHeap
{
public:
	TopHeap(void);
	~TopHeap(void);

	int    GetTop(int count, int* data);
	void  Insert(int data);

private:

	HeapNode* m_root_node;
	int               m_node_count;
};


#endif  

