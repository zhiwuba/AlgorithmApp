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
	virtual ~ParseText();

	virtual int load_file_and_stat(const char* file_path)=0;

	virtual int save_result_to_file(const char* file_path)=0;

protected:
	SkipList<int,SumChanger<int>> m_skip_list;
	SumChanger<int>  m_changer;
};


class ParseUTF8Text:public ParseText
{
public:
	ParseUTF8Text();
	virtual ~ParseUTF8Text();

	virtual int load_file_and_stat(const char* file_path);

	virtual int save_result_to_file(const char* file_path);

};


class ParseGBKText:public ParseText
{
public:
	ParseGBKText();
	virtual ~ParseGBKText();

	virtual int load_file_and_stat(const char* file_path);

	virtual int save_result_to_file(const char* file_path);

};


class ParseEnglishText:public ParseText
{
public:
	ParseEnglishText();
	virtual ~ParseEnglishText();

	virtual int load_file_and_stat(const char* file_path);

	virtual int save_result_to_file(const char* file_path);

};


#endif
