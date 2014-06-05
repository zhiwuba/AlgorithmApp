#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>

#include "bp_tree.h"

/*    Features
**  B+Tree 为前开后闭
**
**
**/

bool node_cmp(Node* node , int key )
{
	return node->key < key ;
}


BP_Tree::BP_Tree()
{

}

BP_Tree::~BP_Tree()
{

}

int BP_Tree::init_meta()
{
	m_meta=new Meta;
	m_meta->degree=6;
	m_meta->height=2;

	m_meta->inner_node_num=0;
	m_meta->leaf_node_num=0;
	m_meta->key_size=0;
	m_meta->value_size=0;

	m_meta->root=create_node(NULL,false);
	Node* leaf=create_node(m_meta->root, true);
	m_meta->root->children.push_back(leaf);

	return 0;
}

int BP_Tree::insert(int key, Value* value)
{
	Node* leaf=search_node(key);
	 
	if ( leaf->children.size()==m_meta->degree )
	{   //full
		Node* new_leaf=create_node(leaf->parent,true); /*新叶节点*/
		
		int point=leaf->children.size()/2;
		bool  place_left= leaf->children[point]->key >= key ;

		new_leaf->children.resize(leaf->children.size()-point);
		std::copy(leaf->children.begin()+point, leaf->children.end(), new_leaf->children.begin())	;
		leaf->children.erase(leaf->children.begin()+point, leaf->children.end());
		
		/* 更新key值 */
		leaf->key=leaf->children.back()->key;
		new_leaf->key=new_leaf->children.back()->key;

		if ( place_left )
		{
			insert_to_leaf_no_split(leaf,key, value);
		}
		else
		{
			insert_to_leaf_no_split(new_leaf,key,value);
		}

		insert_to_index(leaf->parent, new_leaf);
	}
	else
	{
		insert_to_leaf_no_split(leaf,key,value);
	}

	return 0;
}

int  BP_Tree::search(int key, Value* value)
{
	int ret=0;
	Node* leaf=search_node(key);
	
	std::vector<Node*>::iterator iter;
	iter=std::lower_bound(leaf->children.begin(), leaf->children.end(),  key, node_cmp);
	if (iter!=leaf->children.end() && (*iter)->key==key )
	{
		value=leaf->value;
	}
	else
	{
		ret=-1;
		value=NULL;
	}
	return ret;
}

int BP_Tree::update(int key, Value* value)
{
	int ret=0;
	Node* leaf=search_node(key);

	std::vector<Node*>::iterator iter;
	iter=std::lower_bound(leaf->children.begin(), leaf->children.end(),  key, node_cmp);
	if (iter!=leaf->children.end() && (*iter)->key==key )
	{
		delete leaf->value;  //释放空间
		leaf->value=value;
	}
	else
	{   //没有找到就插入
		insert(key, value);
	}
	return ret;	
}

int  BP_Tree::remove(int key)
{
	Node* leaf=search_node(key);

	std::vector<Node*>::iterator delete_iter;
	delete_iter=std::lower_bound(leaf->children.begin(), leaf->children.end(), key, node_cmp);
	if ( delete_iter==leaf->children.end() || (*delete_iter)->key!=key )
		return -1;

	leaf->children.erase(delete_iter);
	//delete (*delete_iter);

	int min_size=m_meta->degree/2;
	//borrow or  merge
	if ( leaf->children.size()< min_size && leaf!=m_meta->root )
	{
		// first borrow from left
		bool borrowed = false;
		Node* left_lender=get_sibling_node(leaf, true);
		if (  left_lender != NULL)
			borrowed = borrow_node(left_lender, leaf, true);

		// then borrow from right
		Node* right_lender=get_sibling_node(leaf, false);
		if ( !borrowed && right_lender!=NULL )
			borrowed = borrow_node(right_lender, leaf,false);
		
		Node* parent=leaf->parent;
	
		//合并
		if ( !borrowed )
		{
			if ( right_lender==NULL )
			{   //左侧合并
				merge_node(leaf , left_lender, true);
				parent=left_lender->parent;
			}
			else
			{   //和右侧合并
				merge_node(leaf, right_lender, false);
				parent=right_lender->parent;
			}
		}

		reset_parent_index(parent);
	}
	else
	{
		if ( leaf->children.size()>0 )
		{
			leaf->key=leaf->children.back()->key;
			if ( leaf->parent!=NULL )
			{
				reset_parent_index(leaf->parent);
			}
		}
	}

	return 0;
}


int BP_Tree::insert_to_index(Node* parent, Node* new_node)
{
	if ( parent==m_meta->root->parent ) /* root node */
	{
		Node* new_root=create_node(NULL,false);
		new_root->children.push_back(m_meta->root);
		new_root->children.push_back(new_node);

		m_meta->root->parent=new_root;
		new_node->parent=new_root;

		m_meta->root=new_root;
		m_meta->height++;
		return 0;
	}

	if ( parent->children.size()==m_meta->degree )
	{
		Node* new_sibling=create_node(parent->parent, false); /*parent节点已满 新建兄弟节点*/

		int point=parent->children.size()/2;
		bool  place_left= parent->children[point]->key >= new_node->key ;

		new_sibling->children.resize(parent->children.size()-point);
		std::copy(parent->children.begin()+point, parent->children.end(), new_sibling->children.begin())	;
		parent->children.erase(parent->children.begin()+point, parent->children.end());

		/*更新key值*/
		parent->key=parent->children.back()->key;
		new_sibling->key=new_sibling->children.back()->key;

		if ( place_left )
		{
			insert_to_index_no_split(parent, new_node);
		}
		else
		{
			insert_to_index_no_split(new_sibling, new_node);
		}

		reset_index_children_parent(new_sibling);
		insert_to_index(parent->parent,  new_sibling);
	}
	else
	{
		insert_to_index_no_split(parent, new_node);
	}

	return 0;
}

int BP_Tree::insert_to_leaf_no_split(Node* leaf, int key, Value* value)
{
	std::vector<Node*>::iterator iter;
	iter=std::lower_bound(leaf->children.begin(), leaf->children.end() , key, node_cmp);

	Node* record=create_node(leaf,true); //记录数据
	record->key=key;
	record->value=value;

	leaf->children.insert(iter, record);
	leaf->key=(leaf->key > key)? leaf->key : key;
	leaf->parent->key=leaf->parent->children.back()->key;
	return 0;
}

int BP_Tree::insert_to_index_no_split(Node* parent, Node* new_node)
{
	std::vector<Node*>::iterator iter;
	iter=std::lower_bound(parent->children.begin(), parent->children.end(), new_node->key, node_cmp);

	parent->children.insert(iter, new_node);
	parent->key=(parent->key > new_node->key)? parent->key : new_node->key;
	
	return 0;
}

int BP_Tree::reset_index_children_parent(Node* node)
{
	for ( int i=0; i<node->children.size(); ++i )
	{
		node->children[i]->parent=node;
	}
	return 0;
}

Node* BP_Tree::search_node(int key)
{
	int height=m_meta->height;
	Node* node=m_meta->root;
	while ( height>1 )
	{
		if ( node->children.empty() )
		{
			assert(false);
			Node* leaf=create_node(node, true);
			node->children.push_back(leaf);
			return leaf;
		}
		else
		{
			std::vector<Node*>::iterator iter;
			iter=std::lower_bound(node->children.begin(), node->children.end()-1, key,node_cmp);
			node=node->children[iter-node->children.begin()];
		}

		--height;
	}
	return node;
}


Node* BP_Tree::create_node(Node* parent , bool is_leaf)
{
	Node* node=new Node;
	node->is_leaf=is_leaf;
	node->parent=parent;
	node->value=NULL;

	return node;
}

Node* BP_Tree::get_sibling_node(Node* node, bool left)
{
	Node* parent=node->parent;
	if ( parent==NULL )
	{
		return NULL;
	}
	std::vector<Node*>::iterator iter;
	iter = lower_bound( parent->children.begin() , parent->children.end(), node->key, node_cmp );

	if ( left )
	{
		if ( iter==parent->children.begin() )
		{
			return NULL;
		}
		else
		{
			return *(--iter);
		}
	}
	else
	{
		if ( iter==parent->children.end()-1 )
		{
			return NULL;
		}
		else
		{
			return *(++iter);
		}
	}
}

bool BP_Tree::borrow_node(Node* lender, Node* borrower, bool from_left)
{
	int min_size=m_meta->degree/2;
	if ( lender->children.size()==min_size )
	{
		return false;
	}
	else
	{
		if ( from_left )
		{
			Node* exchange=lender->children.back();
			borrower->children.insert(borrower->children.begin(), exchange);
			lender->children.pop_back();
			lender->key=lender->children.back()->key;
			exchange->parent=borrower;
		}
		else
		{
			Node* exchange=lender->children.front();
			borrower->children.push_back( exchange );
			lender->children.erase( lender->children.begin());
			borrower->key=borrower->children.back()->key;
			exchange->parent=borrower;
		}
	}

	return  true;
}

bool BP_Tree::merge_node(Node* from, Node* to, bool to_left)
{
	Node* parent=to->parent;
	
	if ( to_left )
	{
		to->children.insert(to->children.end(), from->children.begin(), from->children.end() );
	}
	else
	{
		to->children.insert(to->children.begin(), from->children.begin(), from->children.end() );
	}

	std::vector<Node*>::iterator iter;
	iter=std::lower_bound(parent->children.begin(), parent->children.end(), from->key , node_cmp );
	parent->children.erase(iter);
	//delete *iter;

	reset_index_children_parent(to);

	to->key= to->children.back()->key;
	parent->key=parent->children.back()->key;

	return true;
}


bool  BP_Tree::reset_parent_index(Node* node)
{
	if ( node==m_meta->root && node->children.size()==1 )
	{   //parent节点
		Node* old_root=m_meta->root;
		Node* new_root=node->children.front();
		new_root->parent=NULL;
		m_meta->root=new_root;
		m_meta->height--;
		delete old_root;
		return true;
	}

	/**/
	int min_size=m_meta->degree/2;
	if ( node->children.size() < min_size && node!=m_meta->root )
	{
		bool borrowed = false;
		Node* left_lender=get_sibling_node(node, true);
		if (  left_lender != NULL)
			borrowed = borrow_node(left_lender, node, true);

		// then borrow from right
		Node* right_lender=get_sibling_node(node, false);
		if ( !borrowed && right_lender!=NULL )
			borrowed = borrow_node(right_lender, node,false);

		//else merge
		if ( !borrowed )
		{
			Node* parent=node->parent;
			if ( right_lender==NULL )
			{   //左侧合并
				merge_node(node , left_lender,true);
			}
			else
			{   //和右侧合并
				merge_node(node, right_lender,false);
			}
			reset_parent_index(parent);
		}
	}
	else
	{
		node->key=node->children.back()->key;
		if ( node->parent!=NULL )
		{
			reset_parent_index(node->parent);
		}
	}

	return true;
}


void  BP_Tree::print_tree()
{
	for ( int i=1 ;i<=m_meta->height; i++ )
	{
		print_level(m_meta->root, 1, i);
		printf(" \n\n");
	}
}

void BP_Tree::print_level(Node* node, int cur_level ,int level)
{
	if ( level==cur_level )
	{
		for ( int i=0;i <node->children.size(); ++i )
		{
			printf(" %d ", node->children[i]->key);
		}
		printf("##");
	}
	else
	{
		cur_level++;
		for ( int i=0; i<node->children.size(); ++i )
		{
			print_level(node->children[i], cur_level, level);
		}
	}
}





