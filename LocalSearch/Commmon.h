#pragma once

#include <string>
#include "FileMD5.h"


class Commmon
{
public:
	Commmon(void);
	~Commmon(void);

	static int  get_word_hash_id(const char* word);

	static int  get_doc_hash_id(std::string& filepath);

	static int get_file_md5(const char* filepath,  char* md5 );

	static bool is_alpha_char(char c);

};

