#ifndef  __PARSE_TEXT_H__
#define __PARSE_TEXT_H__


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
};

#endif
