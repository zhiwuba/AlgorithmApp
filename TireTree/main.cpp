#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include <windows.h>
#include "TireTree.h"
#include "TopHeap.h"

template<class T>
struct Compare 
{
	bool operator()(const T& a , const T& b) const
	{
		return a>b ? true : false;
	}
};

int init_tire_tree(TireTree*  tree)
{
	int ret=-1;
	FILE* file=fopen("D:\\Workspace\\App\\TireTree\\Debug\\word.txt", "rb");
	if ( file!=NULL )
	{
		char line[256];
		while(fgets(line,256,file))
		{
			char word[100]={0};
			const char* p=line;
			while (*p==' ')p++;
			const char* start=p;
			while(*p>='a'&&*p<='z')p++;
			strncpy(word, start, p-start );
			tree->InsertNode(word);
		}
		fclose(file);
		ret=0;
	}
	return ret;
}


int main()
{
#if 0
	long  timeStart=GetTickCount();
	TireTree* tree=new TireTree();
	
	if (0 != init_tire_tree(tree) )
	{
		printf("init_tire_tree error. \n");
		return -1;
	}
	long timeCost=GetTickCount()-timeStart;
	printf("init_tire_tree cost time: %ld ms .\n", timeCost);


	while(true)
	{
		char word[100]={0};
		printf(" ‰»Îπÿº¸¥ :  ");
		scanf("%s", &word);

		std::vector<std::string> suggest_list;
		suggest_list = tree->GetSuggestion(word);




		printf("\n∫Ú—°¥ : \n");
		for ( int i=0;i <suggest_list.size(); i++ )
		{
			std::cout<<suggest_list[i]<<std::endl;
		}
	}

	delete tree;
#endif

#if 1	
	srand(time(0));
	Compare<int> comp;
	TopHeap<int, Compare<int>>* topHeap=new TopHeap<int,Compare<int>>(comp);
	for ( int i=0 ;i< 20; i++ )
	{
		int num=rand()%200;
		topHeap->Add(num);
	}


	//bool ret=Compare<int>()(15,24);

	//topHeap->PrintHeap();

	for ( int i=0; i<20; i++ )
	{
		int num=0;
		topHeap->GetTop(1, &num);
		printf("%d  ", num);
	}
	printf("\n");

#endif

	return 0;
}


