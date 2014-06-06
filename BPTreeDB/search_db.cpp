#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <list>
#include <algorithm>

#include "search_db.h"

bool node_cmp(Node* node , int key )
{
	return node->key < key ;
}




/*    Features
**  B+Tree 为前开后闭
**
**/
Search_DB::Search_DB(const char* db_path, int value_size)
{
	m_root=create_node(NULL,false);
	Node* leaf=create_node(m_root, true);
	m_root->children.push_back(leaf);

	m_degree=6;
	m_height=2;
}

Search_DB::~Search_DB()
{
}

int Search_DB::insert(int key, value_t* value)
{
	off_t leaf_offset=search_node(key);
	Node leaf;
	
	map(leaf,)

	if ( leaf->children.size()==m_degree )
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

int  Search_DB::search(int key, value_t* value)
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

int Search_DB::update(int key, value_t* value)
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

int  Search_DB::remove(int key)
{
	Node* leaf=search_node(key);

	std::vector<Node*>::iterator delete_iter;
	delete_iter=std::lower_bound(leaf->children.begin(), leaf->children.end(), key, node_cmp);
	if ( delete_iter==leaf->children.end() || (*delete_iter)->key!=key )
		return -1;

	leaf->children.erase(delete_iter);
	//delete (*delete_iter);

	reset_nodes_after_remove(leaf);

	return 0;
}


int Search_DB::insert_to_index(Node* parent, Node* new_node)
{
	if ( parent==m_root->parent ) /* root node */
	{
		Node* new_root=create_node(NULL,false);
		new_root->children.push_back(m_root);
		new_root->children.push_back(new_node);

		m_root->parent=new_root;
		new_node->parent=new_root;

		m_root=new_root;
		m_height++;
		return 0;
	}

	if ( parent->children.size()==m_degree )
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

		update_link(new_sibling);
		insert_to_index(parent->parent,  new_sibling);
	}
	else
	{
		insert_to_index_no_split(parent, new_node);
	}

	return 0;
}

int Search_DB::insert_to_leaf_no_split(Node* leaf, int key, value_t* value)
{
	std::vector<Record>::iterator iter;
	iter=std::lower_bound(leaf->children.begin(), leaf->children.end() , key, node_cmp);

	Node* record=create_node(leaf,true); //记录数据
	record->key=key;
	record->value=value;

	leaf->children.insert(iter, record);
	update_key(leaf);

	return 0;
}

int Search_DB::insert_to_index_no_split(Node* parent, Node* new_node)
{
	std::vector<Record>::iterator iter;
	iter=std::lower_bound(parent->children.begin(), parent->children.end(), new_node->key, node_cmp);
	
	Record new_record;
	new_record.key=new_node->key;
	new_record.offset=new_node->offset;
	parent->children.insert(iter, new_record);
	parent->key=(parent->key > new_node->key)? parent->key : new_node->key;

	return 0;
}


off_t Search_DB::search_node(int key)
{
	int height=m_height;
	off_t   offset=kRootOffSet;
	Node  node;
	while ( height>1 )
	{
		map(&node, offset);
		std::vector<Record>::iterator iter;
		iter=std::lower_bound(node.children.begin(), node.children.end()-1, key,node_cmp);
		offset=iter->offset;
		--height;
	}

	return offset;
}



Node* Search_DB::get_sibling_node(Node* node, bool left)
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

bool Search_DB::borrow_node(Node* lender, Node* borrower, bool from_left)
{
	int min_size=m_degree/2;
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

bool Search_DB::merge_node(Node* from, Node* to, bool to_left)
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

	update_link(to);
	update_key(to);

	return true;
}


bool  Search_DB::reset_nodes_after_remove(Node* node)
{
	if ( node==m_root && node->children.size()==1&&m_height>2 )
	{   //parent节点
		Node* old_root=m_root;
		Node* new_root=node->children.front();
		new_root->parent=NULL;
		m_root=new_root;
		m_height--;
		delete old_root;
		return true;
	}

	/**/
	int min_size=m_degree/2;
	if ( node->children.size() < min_size && node!=m_root )
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
			if ( right_lender!=NULL )
			{ //和右侧合并
				merge_node(node, right_lender,false);
			}
			else if ( left_lender!=NULL )
			{	//左侧合并
				merge_node(node , left_lender,true);
			}
			else
			{
				update_key(node);
			}

			reset_nodes_after_remove(parent);
		}
	}
	else
	{
		node->key=node->children.back()->key;
		if ( node->parent!=NULL )
		{
			reset_nodes_after_remove(node->parent);
		}
	}

	return true;
}

void  Search_DB::print()
{
	for ( int i=1 ;i<=m_height; i++ )
	{
		print_level(m_root, 1, i);
		printf(" \n\n");
	}
}

void Search_DB::print_level(Node* node, int cur_level ,int level)
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




void Search_DB::create_node(off_t parent , bool is_leaf, Node* node)
{
	node->offset=alloc_node(is_leaf);
	node->is_leaf=is_leaf;
	node->parent=parent;
	node->value=NULL;
}

void Search_DB::update_key(Node* node)
{
	if ( !node->children.empty() )
	{
		 node->key=node->children.back()->key;

		if ( node->parent!=NULL )
		{
			node->parent->key=node->parent->children.back()->key;
		}
	}
}

void Search_DB::update_link(Node* node)
{
	for ( int i=0; i<node->children.size(); ++i )
	{
		node->children[i]->parent=node;
	}
}


int Search_DB::unmap(Node* node, off_t offset )
{
	int  length=0;
	fseek(m_db_file,offset,SEEK_CUR);
	//fwrite((char*)&node->parent.key,1,4, m_db_file );
	fwrite((char*)&node->parent,1,4, m_db_file);

	int children_count=node->children.size();
	fwrite((char*)children_count, 1, 4, m_db_file);
	for ( int i=0; i< children_count; ++i )
	{
		fwrite((char*)&node->children[i].key ,1,4, m_db_file );
		fwrite((char*)&node->children[i].offset, 1,4, m_db_file);
	}

	return  length;
}

int Search_DB::map(Node* node, off_t offset )
{
	int length=0;
	fseek(m_db_file, offset, SEEK_CUR);
	fread((char*)&node->is_leaf,1,4,m_db_file);
	fread((char*)&node->key,1,4,m_db_file);
	//fread((char*)&node->parent.key, 1, 4, m_db_file);
	fread((char*)&node->parent,1,4, m_db_file);

	if ( node->is_leaf )
	{
		node->value=new char[m_value_size];
		fread(node->value, 1, m_value_size, m_db_file);
	}
	else
	{
		node->children.clear();
		int children_count;
		fread((char*)&children_count, 1, 4, m_db_file);
		for ( int i=0; i< children_count; ++i )
		{
			Record child;
			fread((char*)&child.key ,1,4, m_db_file );
			fread((char*)&child.offset, 1,4, m_db_file);
			node->children.push_back(child);
		}
	}

	return length;
}

off_t Search_DB::alloc_node(bool is_leaf)
{
	off_t offset=m_slot;
	if ( is_leaf )
	{
		int length=12+m_value_size;
		m_slot+=length;
	}
	else
	{
		int length=16+m_degree*(4+kOffSize);
		m_slot+=length;
	}
	return offset;
}


int  Search_DB::map_root()
{
	fseek(m_db_file, 0, SEEK_SET);
	fread((char*)&m_degree, 1 , 4, m_db_file );
	fread((char*)&m_height, 1 , 4, m_db_file );
	fread((char*)&m_node_count, 1 , 4, m_db_file );
	fread((char*)&m_leaf_count, 1 , 4, m_db_file );
	fread((char*)&m_value_size, 1 , 4, m_db_file );
	fread((char*)&m_slot, 1 , 4, m_db_file );

	return 0;
}

int  Search_DB::unmap_root()
{
	fseek(m_db_file, 0, SEEK_SET);
	fwrite((char*)&m_degree, 1 , 4, m_db_file );
	fwrite((char*)&m_height, 1 , 4, m_db_file );
	fwrite((char*)&m_node_count, 1 , 4, m_db_file );
	fwrite((char*)&m_leaf_count, 1 , 4, m_db_file );
	fwrite((char*)&m_value_size, 1 , 4, m_db_file );
	fwrite((char*)&m_slot, 1 , 4, m_db_file );

	return 0;
}



