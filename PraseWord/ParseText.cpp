#include "ParseText.h"
#include <assert.h>
#include "UTF8.h"

ParseText::ParseText():m_skip_list(m_changer)
{

}

ParseText::~ParseText()
{

}

int ParseText::load_file_and_stat(const char* file_path)
{
	assert(file_path!=NULL);
	FILE* file=fopen(file_path,"rb");
	if ( file!=NULL )
	{
		while ( !feof(file) )
		{
			int  pos=0, buffer_len=0;
			char buffer[1024]={0};
			if ( NULL!=fgets(buffer,1024,file))
			{
				buffer_len=strlen(buffer);
				while (pos<buffer_len)
				{
					int len=UTF8::get_utf8_length(*(buffer+pos));
					int code=UTF8::utf8_to_unicode(buffer+pos, len);
					if ( UTF8::filter_utf8_chinese_code(code) )
					{
						m_skip_list.add_node(code,1);
					}
					pos+=len;
				}
			}
		}
		fclose(file);
		return 0;
	}
	else
	{
		printf("failed to fopen file %s \n", file_path);
		return -1;
	}
}

int ParseText::save_result_to_file( const char* file_path )
{
	assert(file_path!=NULL);
	FILE* file=fopen(file_path,"wb");
	if ( file!=NULL )
	{
		char header[3] = {(char)0xEF, (char)0xBB, (char)0xBF};  
		fwrite(header, sizeof(char), 3, file); 

		char buffer[100];
		SkipList<int,SumChanger<int>>::iterator iter=m_skip_list.begin();
		for ( ; iter!=m_skip_list.end() ; ++iter )
		{
			char utf8[4]={0};
			UTF8::unicode_to_utf8(iter->key,  utf8);
			sprintf(buffer,"%s-------%d\r\n", utf8 , iter->value );
			fwrite(buffer, 1, strlen(buffer), file );
		}

		fclose(file);
	}

	return 0;
}
