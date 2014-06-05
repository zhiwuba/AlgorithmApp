/* 
**   B+Tree DB
**/
#ifndef  __SEARCH_DB_H__
#define __SEARCH_DB_H__


class Search_DB
{
public:
	Search_DB(const char* db_path);
	~Search_DB();


protected:




private:
	std::string        m_db_path;
	FILE*               m_db_file;
};

#endif
