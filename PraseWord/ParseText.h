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

	int load_file(const char* file_path);

private:
	int get_utf8_length(char c);
	int get_utf8_code(const char* word , int length);
	int get_word_value(const char* word , int length);
	int filter_utf8_chinese_code(int code);

	SkipList<int,SumChanger<int>> m_skip_list;
	SumChanger<int>  m_changer;
};

#endif
