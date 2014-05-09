#ifndef   __TOP_HEAP_H__
#define  __TOP_HEAP_H__

#include <vector>


//template<class T>
//struct Compare 
//{
//	bool operator()(const T& a , const T& b) const
//	{
//		return a>b ? true : false;
//	}
//};

//大堆
template<class T, class Comp>
class TopHeap
{
public:
	TopHeap(void);
	~TopHeap(void);

	int    GetTop(int count, T* data);
	void  Add(T data);
	void  PrintHeap();

private:

	void  SiftupNode(int index);
	void  SiftdownNode(int index);

	int   left_child(int index);
	int   right_child(int index);
	int   node_parent(int index);
	int   node_value(int index);
	void swap_node(int index1, int index2);

	int               m_node_count;
	std::vector<T> m_array;
};

template<class T, class Comp>
TopHeap<T,Comp>::TopHeap(void)
{

}

template<class T, class Comp>
TopHeap<T,Comp>::~TopHeap(void)
{

}

template<class T, class Comp>
int  TopHeap<T,Comp>::GetTop(int count, T* data)
{
	count=count>m_array.size()?m_array.size():count;
	for ( int i=0 ;i <count; i++ )
	{
		data[i]=m_array[0]; //最大值
		m_array[0]=m_array[m_array.size()-1];
		m_array.pop_back();
		SiftdownNode(0);
	}

	return 0;
}

template<class T, class Comp>
void  TopHeap<T,Comp>::Add(T data)
{
	m_array.push_back(data);
	SiftupNode(m_array.size()-1);
}

template<class T, class Comp>
void TopHeap<T,Comp>::PrintHeap()
{
	int  count=0;
	int  layer=0;
	for ( int i=0; i<m_array.size(); i++ )
	{ 
		count++;
		printf("%d ", m_array[i]);

		if ( count==pow(2,layer) )
		{
			count=0;
			layer++;
			printf("\n");
		}
	}
}


template<class T, class Comp>
void TopHeap<T,Comp>::SiftupNode(int index)
{
	int i=index;
	while ( i>0 )
	{
		if ( Comp(m_array[i], m_array[node_parent(i)]) )
		{
			swap_node(i, node_parent(i) );
		}
		else
		{
			break;
		}
		i=node_parent(i);
	}

}

template<class T, class Comp>
void TopHeap<T,Comp>::SiftdownNode(int index)
{
	int i=index;
	int array_count=m_array.size();

	while (i<m_array.size())
	{
		if ( left_child(i)==0 ) //左孩子存在
		{
			break;
		}

		if ( right_child(i)!=0 ) //右孩子存在
		{
			int node=Comp(m_array[left_child(i)],m_array[right_child(i)] )?left_child(i) : right_child(i);
			if ( Comp(m_array[node] , m_array[i]) )
			{
				swap_node(i, node);
				i=node;
			}
			else
			{
				break;
			}
		}
		else
		{
			if ( Comp(m_array[i] , m_array[left_child(i)]) )
			{
				swap_node( i, left_child(i) );
				i=left_child(i);
			}
			else
			{
				break;
			}
		}

	}
}

template<class T, class Comp>
int TopHeap<T,Comp>::left_child(int index)
{
	if ( index*2+1>m_array.size()-1 )
	{
		return 0;
	}
	return index*2+1;
}

template<class T, class Comp>
int TopHeap<T,Comp>::right_child(int index)
{
	if ( (index+1)*2>m_array.size()-1 )
	{
		return 0;
	}
	return (index+1)*2;
}

template<class T, class Comp>
int TopHeap<T,Comp>::node_parent(int index)
{
	return (index-1)/2;
}

template<class T, class Comp>
int  TopHeap<T,Comp>::node_value(int index)
{
	return m_array[index];
}

template<class T, class Comp>
void TopHeap<T,Comp>::swap_node(int index1, int index2)
{
	int temp=m_array[index1];
	m_array[index1]=m_array[index2];
	m_array[index2]=temp;
}




#endif  

