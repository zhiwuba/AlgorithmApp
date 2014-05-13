#ifndef  __PARSE_TEXT_H__
#define __PARSE_TEXT_H__

#include "SkipList.h"

template<class T>
class SumChanger
{
public:
	bool operator()(T& dest, T& src)
	{
		dest+=src;
		return true;
	}
};

class ParseText
{
public:
	ParseText();
	~ParseText();

	int load_file_and_stat(const char* file_path);

	int save_result_to_file(const char* file_path);

private:
	SkipList<int,SumChanger<int>> m_skip_list;
	SumChanger<int>  m_changer;
};

#endif
