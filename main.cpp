#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "para.h"

using namespace std;

int whatcolor(int j,int *p)
{
	for(int i = 0; i < k_num; i++)
	{
		if (p[j] == i)
		{
			return i;
			break;
		}
	}
}
//color颜色， rmap拓扑图， list冲突点， Ttable记录禁忌动作过期步长
void InitPara(int *color, int (*rmap)[point_num], int *list, int (*Ttable)[k_num], int MinF, int f_num)
{
	init(color, rmap);
	int f = calF(color, rmap, list);
	MinF = f;
	f_num = f;
	for (int i = 0; i < point_num; ++i)
	{
		for (int j = 0; j < k_num; ++i)
		{
			Ttable[i][j] = f + rand()%10;
		}
	}
}
int calF(int *color, int (*rmap)[point_num], int *list)
{
	//color为点颜色， rmap为关系对, point_num 记录冲突
	int flags = 0;

	for(int i = 0; i < point_num; i++)
	{
		for(int j = 0; j < point_num; j++)
		{
			if(color[i] == color[j])
			{
				if (rmap[i][j] == 1) { list[i] = 1; list[j] = 1; }
			}
		}
	}
	for(int i = 0; i < point_num; i++)
	{
		if (list[i] == 1)
		{
			flags++;
		}
	}
	return flags;
}
//形成关系表
void EstTable(int (*table)[k_num], int *color, int (*rmap)[point_num])
{
	for (int j = 0; j < point_num; j++)
	{
		for (int ik = 0; ik < point_num; ik++)//匹配到有关系的点的位置
		{
			if (rmap[j][ik] == 1)
			{
				if (color[ik] == 0)//有关系点为红色
				{
					table[j][0] += 1;
				}
				else if (color[ik] == 1)
				{
					table[j][1] += 1;
				}
				else if (color[ik] == 2)
				{
					table[j][2] += 1;
				}
			}
		}
	}
}
//更新关系表
void ReTable(int (*table)[k_num], int *color, int (*rmap)[point_num], int *list)
{
	for (int j = 0; j < point_num; j++)
	{
		if(list[j] == 1){
		for (int ik = 0; ik < point_num; ik++)//匹配到有关系的点的位置
		{
			if (rmap[j][ik] == 1)
			{
				if (color[ik] == 0)//有关系点为红色
				{
					table[j][0] += 1;
				}
				else if (color[ik] == 1)
				{
					table[j][1] += 1;
				}
				else if (color[ik] == 2)
				{
					table[j][2] += 1;
				}
			}
		}}
	}
}
//搜索邻域
///color颜色， rmap拓扑图， list冲突点， table为关系表， Ttable记录禁忌动作过期步长 TNMOV是禁忌非禁忌，动作， MinF历史最小F
void FindMove(int (*table)[k_num], int *color, int (*Ttable)[k_num], int *list, int *Tmov, int *NTmov, int *mov, int MinF, int iter, int f_num)
{
	int lastDelta;
	int Delta;
	int sol;
	
	for(int i = 0; i < point_num; i++)//i点
	{
		if(list[i] == 1)
		{
		for(int j = 0; j < k_num; j++)//k色
		{
			sol = whatcolor(i,color);
			lastDelta = 0;
			//冲突数目>0且不为以前颜色
			if(table[i][j]>0 && j != sol)
			{
                Delta = table[i][j] - table[i][sol];
			    //Tmov 禁忌动作， T0动作点 T1动作
				if (iter<Ttable[i][j] && Delta < lastDelta)
				{
					Tmov[0] = i;
					Tmov[1] = j;
					Tmov[2] = Delta;
				}
				else if (Delta < lastDelta)
				{
					NTmov[0] = i;
					NTmov[1] = j;
					NTmov[2] = Delta;
				}
				lastDelta = Delta;			
			}
		}
	    }
	}
	if ((Tmov[2] < NTmov[2]) && (f_num+Tmov[2] < MinF))
	{
		mov[0] = Tmov[0];
        mov[1] = Tmov[1];
        mov[2] = Tmov[2];
	}
	else
	{
		mov[0] = NTmov[0];
        mov[1] = NTmov[1];
        mov[2] = NTmov[2];
	}	
}

void MakeMove(int f_num, int (*table)[k_num], int (*Ttable)[k_num], 
	int (*rmap)[point_num], int *list, int *color, int *mov, int MinF, int iter)
{
	color[mov[0]] = mov[1];
	ReTable(table, color, rmap, list);
    f_num = calF(color, rmap, list);
    for(int i = 0; i < point_num; i++)
    {
    	if (list[i] == 1)
    	{
    		for(int j = 0; j<k_num; j++)
    		{
    			Ttable[i][j] = f_num + rand()%10;
    		}
    	}
    }
    if (f_num < MinF)
    {
    	MinF = f_num;
    }
	iter += 1;
}

void TabuSearch(int (*table)[k_num], int *color, int (*Ttable)[k_num], int (*rmap)[point_num], 
	int *list, int *Tmov, int *NTmov, int *mov, int MinF, int f_num)
{
	InitPara(color, rmap, list, Ttable, MinF, f_num);
	EstTable(table, color, rmap);

	while(iter<10000)
	{
		FindMove(table, color, Ttable, list, Tmov, NTmov, mov, MinF, iter, f_num);
		MakeMove(f_num, table, Ttable, rmap, list, color, mov, MinF, iter);
	}
}

//
int main()
{
	TabuSearch(point.table, point.color, Ttable, point.re, point.f, Tmovement, NTmovement, movement, MinF, f_number);
	return 0;
}