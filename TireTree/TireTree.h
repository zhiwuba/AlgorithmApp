#ifndef  __TIRE_TREE_H__
#define __TIRE_TREE_H__

#include <string>
#include <list>
#include <vector>

struct TireNode 
{
	char nodeData;
	int   nodeCount;

	std::list<TireNode*>  childNodes;
};

class TireTree
{
public:
	TireTree(void);
	~TireTree(void);

	void InsertNode(const char* word);

	std::vector<std::string> GetSuggestion(const char* word);

	int   GetNodeCount(const char* word);


private:
	int GetWord(TireNode* node, const char* word, std::vector<std::string>& suggest_list );

	TireNode* AddNode(TireNode* node, char data);
	TireNode* FindNode(TireNode* node, char data);

	TireNode* m_root_node;
};


#endif // !__TIRE_TREE_H__
