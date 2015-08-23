#include <string>
#include <map>
#include <list>


/*  限制访问次数的代码  */
long  get_time()
{
	return GetTickCount()/100;
}

long  start_time=get_time();

const int kPeriod=60;

std::map<int, std::pair<int,int*>> m_record_map;

bool visit(int ip)
{
	long cur_time=get_time();

	int* buffer=NULL;
	int  last_visit=0;
	std::map<int, std::pair<int,int*>>::iterator iter=m_record_map.find(ip);
	if( iter==m_record_map.end() )
	{
		buffer=new int[kPeriod];
		memset(buffer, 0, kPeriod*4);
		m_record_map.insert(std::make_pair(ip, std::make_pair(cur_time,buffer)));
		last_visit=cur_time;
	}
	else
	{
		last_visit=iter->second.first;
		iter->second.first=cur_time;
		buffer=iter->second.second;
	}
	
	if( (cur_time-last_visit)/kPeriod>1 )
	{
		memset(buffer, 0, kPeriod*4);
	}
	else if( cur_time!=last_visit )
	{
		for ( int j=last_visit+1; j<=cur_time ; j++ )
		{
			buffer[(j-start_time)%kPeriod]=0;
		}
	}
	buffer[(cur_time-start_time)%kPeriod]++;

	int  total_count=0;
	for(int i=0; i<kPeriod; i++)
	{
		total_count+=buffer[i];
	}

	if( total_count>10 )
	{
		for ( int i=0; i<kPeriod; i++ )
		{
			printf("%d ", buffer[i]);
		}
		return false;
	}
	else
	{
		return true;
	}
}

int main()
{
	srand( (unsigned)time( NULL ) );

	int ip=201;
	for(int i=0; i<100; i++)
	{
		bool ret=visit(ip);
		if( !ret )
		{
			printf("refused! i=%d", i);
			break;
		}

		Sleep(rand()%200);
	}
	return 0;
}
