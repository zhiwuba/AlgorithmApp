#pragma once
#include "Commmon.h"
#include "ForwardIndex.h"



class ParseDocument
{
public:
	ParseDocument(void);
	~ParseDocument(void);

	int Parse(const char* filepath);

private:
	int  get_next_word( const char* line, char* word,  int* length );

	typedef std::hash_map<std::string, int>  StringIntHashMap;
	StringIntHashMap  m_doc_hashmap;
	StringIntHashMap  m_word_hashmap;
	static int   m_last_doc_id;
	static int   m_last_word_id;

	ForwardIndex m_forward_index;
};

