#include "UTF8.h"

void UTF8::print_binary(char c)
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

int UTF8::get_utf8_length(char c)
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


int UTF8::utf8_to_unicode(const char* word , int length)
{   //获取字的code
	int code=0;
	char*  bits=(char*)&code;
	char b1,b2,b3,b4;

	switch (length)
	{
	case 1:
		*bits = (*word)&0x7F;
		break;
	case 2:
		b1 = *word;
		b2 = *(word+1);
		*bits = (b1<<6)+(b2&0x3F);
		*(bits+1) = ((b1>>2)&0x7);
		break;
	case 3:
		b1 = *word ;
		b2 = *(word+1);
		b3 = *(word+2);
		*bits = (b2<<6)+(b3&0x3F);
		*(bits+1) = (b1<<4)+((b2>>2)&0xF);
		break;
	case 4:
		b1 = *word ;
		b2 = *(word+1);
		b3 = *(word+2);
		b4 = *(word+3);
		*bits = (b3<<6)+(b4&0x3F);
		*(bits+1) = (b2<<4)+((b3>>2)&0xF);
		*(bits+2) = ((b1<<5)>>3)+((b2>>4)&0x3);
	default:
		break;
	}
	return code;
}

int UTF8::unicode_to_utf8(int code , char* word)
{
	if ( code <= 0x0000007F )
	{	//U-00000000 - U-0000007F
		//0xxxxxxx
		*word = ( code & 0x7F );
		return 1;
	}
	else if ( code >= 0x00000080 && code <= 0x000007FF )
	{	//U-00000080 - U-000007FF
		//110xxxxx 10xxxxxx
		*( word + 1 ) = ( code & 0x3F) | 0x80;
		*word     	= ((code >> 6) & 0x1F) | 0xC0;
		return 2;
	}
	else if ( code >= 0x00000800 && code <= 0x0000FFFF )
	{	//U-00000800 - U-0000FFFF
		//1110xxxx 10xxxxxx 10xxxxxx
		*( word + 2 ) = ( code & 0x3F) | 0x80;
		*( word + 1 ) = ((code >> 6) & 0x3F) | 0x80;
		*word     	= ((code >> 12) & 0x0F) | 0xE0;
		return 3;
	}
	else if ( code >= 0x00010000 && code <= 0x001FFFFF )
	{	//U-00010000 - U-001FFFFF
		//11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		*( word + 3 ) = ( code & 0x3F) | 0x80;
		*( word + 2 ) = ((code >>  6) & 0x3F) | 0x80;
		*( word + 1 ) = ((code >> 12) & 0x3F) | 0x80;
		*word     	= ((code >> 18) & 0x07) | 0xF0;
		return 4;
	}
	return 0;
}

int UTF8::filter_utf8_chinese_code(int  code)
{
	int c= ( ( code >= 0x4E00 && code <= 0x9FBF )
		|| ( code >= 0x2E80 && code <= 0x2EFF ) || ( code >= 0x2F00 && code <= 0x2FDF )  
		|| ( code >= 0x31C0 && code <= 0x31EF ) //|| ( code >= 0x3200 && code <= 0x32FF )
		|| ( code >= 0x3300 && code <= 0x33FF ) //|| ( code >= 0x3400 && code <= 0x4DBF )
		|| ( code >= 0x4DC0 && code <= 0x4DFF ) || ( code >= 0xF900 && code <= 0xFAFF )
		|| ( code >= 0xFE30 && code <= 0xFE4F ) ); 
	return c;
}



