#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <windows.h>
#include "TireTree.h"


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
	return 0;
}


