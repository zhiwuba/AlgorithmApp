#include "Commmon.h"

const int  kBufferSize=5*1024*1024;
const int  kWordHashMax=1000*1000;
const int  kHashFactor=31;

Commmon::Commmon(void)
{
}


Commmon::~Commmon(void)
{
}

int Commmon::get_word_hash_id( const char* word )
{
	unsigned int h=0;
	while(*word!='\0')
	{
		h=h*kHashFactor+(*word);
		++word;
	}

	return h/kWordHashMax;
}

int Commmon::get_doc_hash_id( std::string& filepath )
{
	return 0;
}

int Commmon::get_file_md5(const char* filepath, char* filemd5 )
{
	MD5_CTX md5;
	MD5Init(&md5);

	unsigned char* buffer=new unsigned char[kBufferSize];
	int file_pos=0;
	FILE* file=fopen(filepath,"rb");
	if ( file!=NULL )
	{
		int length=0;
		while ( (length=fread(buffer,1, kBufferSize, file))>0 )
		{
			MD5Update(&md5, buffer ,length );
		}

		unsigned char digest[16]={0};
		MD5Final(&md5, digest);

		char hex[3];
		for(int i=0;i<16;i++)  
		{
			sprintf(hex,"%02x",digest[i]); 
			strcat(filemd5,hex );
		}

		fclose(file);
	}

	delete[] buffer;
	return 0;
}

bool Commmon::is_alpha_char( char c )
{
	if ( (c>='a'&&c<='z')||(c>='A'&&c<='Z') )
	{
		return true;
	}
	return false;
}
