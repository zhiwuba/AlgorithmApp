#include "ParseText.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

ParseText::ParseText():m_skip_list(m_changer)
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
	int l1=0,l2=0;
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
					int code=get_utf8_code(buffer+pos, len);

					if ( filter_utf8_chinese_code(code) )
					{
						//printf("word len:%d code: %d \n", len, code);
						m_skip_list.add_node(code,1);
						l2++;
					}
					pos+=len;
					l1++;
				}
			}
		}

		fclose(file);
	}

	m_skip_list.print_node();
	return 0;
}

int ParseText::get_utf8_length(char c)
{
	int length=0;
	if ( ((int)c&0x80)!=((int)0x80)  ) return 1;

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
{   //获取字的code
	int code=0;
	char*  bits=(char*)&code;
	char b1,b2,b3,b4;

	switch (length)
	{
	case 1:
		*bits=*word;
		break;
	case 2:
		b1=*word;
		b2=*(word+1);
		*bits=(b1<<6)+(b2&0x3F);
		*(bits+1)=((b1>>2)&0x7);
		break;
	case 3:
		b1=*word ;
		b2=*(word+1);
		b3=*(word+2);
		*bits=(b2<<6)+(b3&0x3F);
		*(bits+1)=(b1<<4)+((b2>>2)&0xF);
		break;
	case 4:
		b1=*word ;
		b2=*(word+1);
		b3=*(word+2);
		b4=*(word+3);
		*bits=(b2<<6)+(b1&0x3F);
		*(bits+1)=(b3<<4)+((b2>>2)&0xF);
		*(bits+2)=((b4<<5)>>3)+((b3>>4)&0x3);
	default:
		break;
	}
	return code;
}


int ParseText::filter_utf8_chinese_code(int  u)
{
	int c= ( ( u >= 0x4E00 && u <= 0x9FBF )
		|| ( u >= 0x2E80 && u <= 0x2EFF ) || ( u >= 0x2F00 && u <= 0x2FDF )  
		|| ( u >= 0x31C0 && u <= 0x31EF ) //|| ( u >= 0x3200 && u <= 0x32FF )
		|| ( u >= 0x3300 && u <= 0x33FF ) //|| ( u >= 0x3400 && u <= 0x4DBF )
		|| ( u >= 0x4DC0 && u <= 0x4DFF ) || ( u >= 0xF900 && u <= 0xFAFF )
		|| ( u >= 0xFE30 && u <= 0xFE4F ) ); 
	return c;
}


