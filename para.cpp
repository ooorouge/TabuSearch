#include <stdio.h>
#include <stdlib.h>
#include "para.h"
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;
//init

void initM(int rmap[][point_num])
{
	char initial;
	int v, w;
	ifstream infile;   

	infile.open("./DSJC250_5.txt");

	if (infile.fail())
	{
		cout << "error open!" << endl;
	}
	else 
	{
		while (!infile.eof())
		{
			infile >> initial >> v >> w;
			rmap[v-1][w-1] = 1;
			rmap[w-1][v-1] = 1;
		}
	}
	infile.close();
}

void init(int *pint, int (*rmap)[point_num])
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < point_num; i++)
	{		
		//srand((unsigned)time(NULL));
		pint[i] = rand()%k_num;
	}
	initM(rmap);
}
