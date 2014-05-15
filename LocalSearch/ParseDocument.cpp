#include "ParseDocument.h"
#include <assert.h>
#include "Commmon.h"

int ParseDocument::m_last_doc_id=0;
int ParseDocument::m_last_word_id=0;

ParseDocument::ParseDocument(void)
{
}

ParseDocument::~ParseDocument(void)
{
}

int ParseDocument::Parse( const char* filepath )
{
	char md5[33]={0};
	Commmon::get_file_md5(filepath,md5);
	if ( m_doc_hashmap.find(std::string(md5))!=m_doc_hashmap.end() )
	{
		printf("document - %s - has parsed. \n", filepath);
		return -1;
	}

	m_doc_hashmap[std::string(md5)]=(++m_last_doc_id);

	Document* document=new Document();
	document->doc_file_path=filepath;
	document->doc_id=m_last_doc_id;

	FILE* file=fopen(filepath,"r");
	if ( file!=NULL )
	{
		int  file_read_pos=0;
		while ( !feof(file) )
		{
			int  line_pos=0, buffer_len=0;
			char cword[50]={0};
			char buffer[1024]={0};
			if ( NULL!=fgets(buffer,1024,file))
			{
				buffer_len=strlen(buffer);
				while (line_pos<buffer_len)
				{
					_strset(cword,0);
					int word_len=0;
					int ptr_move_len=get_next_word(buffer+line_pos, cword ,&word_len);
					line_pos+=ptr_move_len;
					file_read_pos+=ptr_move_len;
					if ( word_len >1 )
					{
						int word_id=0;
						StringIntHashMap::iterator iter=m_word_hashmap.find(cword);
						if ( iter==m_word_hashmap.end() )
						{
							word_id=++m_last_word_id;
							m_word_hashmap[cword]=word_id;
						}
						else
						{
							word_id=iter->second;
						}

						std::map<int,Word*>::iterator iter2=document->words.find(word_id);
						if ( iter2!=document->words.end() )
						{
							if ( iter2->second->word.compare(cword)!=0 )
							{
								printf("hash fighting. \n");
							}
							iter2->second->positions.push_back(file_read_pos);
						}
						else
						{
							Word* word=new Word;
							word->word_id=word_id;
							word->word=cword;
							word->positions.push_back(file_read_pos);
							document->words[word_id]=word;
						}
					}
				}
			}
		}
		fclose(file);
	}

	m_forward_index.add_doc(document);

	return 0;
}

int ParseDocument::get_next_word( const char* line, char* word,  int* length )
{
	const char* p=line;
	while (!Commmon::is_alpha_char(*p)&&*p!='\n')++p;
	const char* begin=p;

	while (Commmon::is_alpha_char(*p)&&*p!='\n')++p;
	const char* end=p;

	*length=end-begin;
	if ( *length>0 )
	{
		strncpy(word, begin, *length);
	}

	return end-line+1;
}

