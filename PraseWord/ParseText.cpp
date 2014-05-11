#include "ParseText.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

ParseText::ParseText()
{

}

ParseText::~ParseText()
{

}

void print_binary(char c)
{
	for (int i=0 ;i<8; i++ )
	{
		if ( ((int)c&0x80)==((int)0x80) )
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
		c<<=1;
	}
}

int ParseText::load_file(const char* file_path)
{
	assert(file_path!=NULL);
	FILE* file=fopen(file_path,"rb");
	if ( file!=NULL )
	{
		while ( !feof(file) )
		{
			int  pos=0;
			int  buffer_len=0;
			char buffer[1024]={0};
			if ( NULL!=fgets(buffer,1024,file))
			{
				buffer_len=strlen(buffer);
				while (pos<buffer_len)
				{
					int len=get_utf8_length(*(buffer+pos));
					int code=get_word_value(buffer+pos, len);
					printf("word len:%d code: %d \n", len, code);
					pos+=len;
				}
			}
		}

		fclose(file);
	}
	return 0;
}

int ParseText::get_utf8_length(char c)
{
	int length=0;
	if (((int)c&0x80)!=((int)0x80)  ) return 1;

	while( ((int)c&0x80)==((int)0x80) ) //此处需要强转
	{
		length++;
		c<<=1;
	}
	return length;
}

int ParseText::get_word_value(const char* word , int length)
{   //从文本中读取的汉字的value
	int ret=0;
	for ( int i=0;i <length; i++ )
	{
		char* c=(char*)&ret;
		c[i]=word[length-1-i];
	}

	return ret;
}

int ParseText::get_utf8_code(const char* word , int length)
{   // 获取汉字的code
	switch (length)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
	return 0;
}
