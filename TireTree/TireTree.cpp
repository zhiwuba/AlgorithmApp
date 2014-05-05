#include "TireTree.h"


TireTree::TireTree(void)
{
	m_root_node=new TireNode();
}


TireTree::~TireTree(void)
{
	delete m_root_node;
	m_root_node=NULL;
}


void TireTree::InsertNode(const char* word)
{
	if ( word!=NULL )
	{
		const char* p=word;
		TireNode* curNode=m_root_node;
		while(*p!='\0')
		{
			curNode=AddNode(curNode, *p);
			p++;
		}
		curNode->nodeCount++;
	}
}

std::vector<std::string> TireTree::GetSuggestion(const char* word)
{
	std::vector<std::string> result;
	if ( word!=NULL )
	{
		TireNode* curNode=m_root_node;
		const char* p=word;
		while (*p!='\0')
		{
			curNode=FindNode(curNode,*p);
			p++;
		}

		char suggest_word[50]={0};
		strcpy(suggest_word,word);
		GetWord(curNode, suggest_word, result);
	}
	return result;
}

int TireTree::GetWord(TireNode* node,const char* word, std::vector<std::string>& suggest_list )
{
	std::list<TireNode*>::iterator iter = node->childNodes.begin();
	for ( ; iter!=node->childNodes.end() ; ++iter )
	{
		int word_length=strlen(word);
		char new_word[100];
		strcpy(new_word, word);
		new_word[word_length]=(*iter)->nodeData;
		new_word[word_length+1]='\0';

		if ( (*iter)->nodeCount>0 )
		{
			suggest_list.push_back(new_word);	
		}

		GetWord(*iter, new_word, suggest_list);
	}
	return 0;
}

int TireTree::GetNodeCount(const char* word)
{
	int count=0;
	if ( word!=NULL )
	{
		TireNode* curNode=m_root_node;
		const char* p=word;
		while ( *p!='\0' )
		{
			curNode=FindNode(curNode, *p);
			if ( curNode==NULL )
			{
				break;
			}
			p++;
		}
		if ( curNode!=NULL )
		{
			count=curNode->nodeCount;
		}
	}
	return count;
}

TireNode* TireTree::AddNode(TireNode* node, char data)
{
	TireNode* resultNode=NULL;
	bool bfinded=false;
	std::list<TireNode*>::iterator iter = node->childNodes.begin();
	for ( ; iter!=node->childNodes.end() ; ++iter )
	{
		if ( (*iter)->nodeData==data )
		{
			bfinded=true;
			resultNode=(*iter);
			break;
		}
	}

	if ( !bfinded )
	{
		TireNode* new_node=new TireNode();
		new_node->nodeCount=0;
		new_node->nodeData=data;
		
		node->childNodes.push_back(new_node);
		resultNode=new_node;
	}

	return resultNode;
}

TireNode* TireTree::FindNode(TireNode* node, char data)
{
	std::list<TireNode*>::iterator iter= node->childNodes.begin();
	for ( ; iter!=node->childNodes.end(); ++iter )
	{
		if ( (*iter)->nodeData==data )
		{
			return *iter;
		}
	}
	return NULL;
}



