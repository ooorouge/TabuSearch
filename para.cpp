#include <stdio.h>
#include <stdlib.h>
#include "para.h"

//init
int iter = 0;
int Ttable[point_num][k_num] = { 0 };
void initM(int rmap[][point_num])
{
	rmap[0][1] = rmap[1][0] = 1;
	rmap[0][2] = rmap[2][0] = 1;
	rmap[0][3] = rmap[3][0] = 1;

	rmap[1][4] = rmap[4][1] = 1;
	rmap[1][8] = rmap[8][1] = 1;

	rmap[2][7] = rmap[7][2] = 1;

	rmap[3][6] = rmap[6][3] = 1;
	rmap[3][7] = rmap[7][3] = 1;

	rmap[4][5] = rmap[5][4] = 1;
	rmap[4][7] = rmap[7][4] = 1;

	rmap[6][8] = rmap[8][6] = 1;
	rmap[6][7] = rmap[7][6] = 1;
}

void init(int *pint, int (*rmap)[point_num])
{
	for (int i = 0; i < point_num; i++)
	{
		pint[i] = rand()%3;
	}
	initM(rmap);
}
