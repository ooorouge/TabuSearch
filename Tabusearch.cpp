#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "para.h"
#include <ctime>

using namespace std;

bool isF = 0;

int Iter = 0;
int f_number = point_num;
int TryStep = 0;

int movement[3] = { 0 };
int MinofAll = point_num;
int candidate[point_num][2] = {};
int candidate_tabu[point_num][2] = {};

//点颜色 0红1蓝2绿
int point_color[point_num] = { 0 };
//点与点之间是否在拓扑图上有联系
int point_re[point_num][point_num] = { 0 };
//关系表
/*int table[point_num][k_num] = { { 2, 0, 1 },{ 1, 2, 0 },{ 1, 0, 1 },
extern { 1, 0, 2 },{ 0, 1, 2 },{ 0, 1, 0 },{ 1, 1, 1 },{ 2, 1, 1 },{ 0, 0, 2 } };*/
int point_table[point_num][k_num] = { 0 };
//冲突点数及记录
int point_f_num = 0;
int Tabutable[point_num][k_num] = { 0 };

int calF(int *color, int(*rmap)[point_num]);
//defination
int whatcolor(int j, int *p)
{
	for (int i = 0; i < k_num; i++)
	{
		if (p[j] == i)
		{
			return i;
			break;
		}
	}
}
//color颜色， rmap拓扑图， list冲突点， Ttable记录禁忌动作过期步长
void InitPara(int *color, int(*rmap)[point_num], int(*Ttable)[k_num], int f_num)
{
	init(color, rmap);
	int f = calF(color, rmap);
	MinofAll = f;
	f_num = f;
	/*	for (int i = 0; i < point_num; i++)
	{
	for (int j = 0; j < k_num; j++)
	{
	Ttable[i][j] = (f + rand()%10);
	}
	}
	*/
}
int calF(int *color, int(*rmap)[point_num])
{
	//color为点颜色， rmap为关系对, point_num 记录冲突
	int flags = 0;
	int kflags = 0;

	for (int i = 0; i < point_num; i++)
	{
		kflags = 0;
		for (int j = 0; j < point_num; j++)
		{
			if (color[i] == color[j] && rmap[i][j] == 1)
			{
				flags += 2;
			}
		}
	}
	flags = flags / 2;

	return flags;
}
//形成关系表 这个表有问题 改了第一次
void EstTable(int(*table)[k_num], int *color, int(*rmap)[point_num])
{
	for (int j = 0; j < point_num; j++)
	{
		for (int ik = 0; ik < point_num; ik++)//匹配到有关系的点的位置
		{
			if (rmap[j][ik] == 1)
			{
				table[j][color[ik]] = table[j][color[ik]] + 1;
			}
		}
	}
}
//更新关系表 
void ReTable(int(*table)[k_num], int *color, int(*rmap)[point_num], int *mov)
{
	int sol = whatcolor(mov[0], color);
	for (int i = 0; i < point_num; i++)
	{
		if (rmap[mov[0]][i] == 1)
		{
			table[i][sol] = table[i][sol] - 1;
			table[i][mov[1]] = table[i][mov[1]] + 1;
		}
	}
}

void Findmove(int(*table)[k_num], int *color, int(*Ttable)[k_num], int *mov, int f_num)
{
	int delt = point_num;
	int delt_tabu = point_num;
	int temp;
	int n_color;//当前颜色
	int *adj; //邻域表行首指针
	int adj_value_n;//当前邻域表值
	int *tabu_p;
	int count = 0;
	int count_tabu = 0;


	for (int i = 0; i < point_num; i++)
	{
		n_color = whatcolor(i, color);//取出当前颜色
		adj = table[i];
		adj_value_n = adj[n_color];
		if (adj_value_n)  //检查是否是冲突点
		{
			tabu_p = Ttable[i];//取禁忌表首指针
			for (int j = 0; j < k_num; j++)//遍历颜色
			{
				if (n_color != j)//排除当前颜色
				{
					temp = adj[j] - adj_value_n;//计算增量 移动后减移动前                  
					if (temp <= delt)
					{
						if (tabu_p[j] <= Iter)//判断是否在禁忌
						{
							if (temp < delt)//非禁忌
							{
								delt = temp;
								count = 0;
							}
							candidate[count][0] = i;//候选结点
							candidate[count][1] = j;//对应候选颜色
							count++;
						}
					}
					else {
						if (temp < delt_tabu)//禁忌
						{
							delt_tabu = temp;
							count_tabu = 0;
						}
						candidate_tabu[count][0] = i;//候选结点
						candidate_tabu[count][1] = j;//对应候选颜色
						count_tabu++;
					}

				}
			}
		}
	}

	if (delt_tabu < delt && delt_tabu < MinofAll - f_number)
	{
		temp = rand() % count_tabu;
		mov[0] = candidate_tabu[temp][0];
		mov[1] = candidate_tabu[temp][1];
		delt = delt_tabu;
	}
	else {
		temp = rand() % count;
		mov[0] = candidate[temp][0];
		mov[1] = candidate[temp][1];//随机选择移动
	}
}

void MakeMove(int f_num, int(*table)[k_num], int(*Ttable)[k_num],
	int(*rmap)[point_num], int *color, int *mov)
{
	ReTable(table, color, rmap, mov);
	color[mov[0]] = mov[1];

	f_num = calF(color, rmap);
	f_number = f_num;

	Ttable[mov[0]][mov[1]] = f_num + rand() % 10 + Iter;

	if (f_num < MinofAll)
	{
		MinofAll = f_num;
	}
	Iter += 1;
	/*cout << f_num << "  " << MinofAll << "  " << mov[0] << "  " << mov[1] << "  " << Iter << "  " << Ttable[mov[0]][mov[1]]
	<< "  " << TryStep << endl;*/
}

void TabuSearch(int(*table)[k_num], int *color, int(*Ttable)[k_num], int(*rmap)[point_num],
	int *mov, int f_num)
{
	InitPara(color, rmap, Ttable, f_num);
	EstTable(table, color, rmap);

	while (f_number > 1)
	{
		Findmove(table, color, Ttable, mov, f_num);
		MakeMove(f_num, table, Ttable, rmap, color, mov);
		if (f_number == 0)
		{
			cout << f_number << endl << Iter;
			system("pause");
		}
	}
}
//***************************************
int main()
{
	clock_t start, finish;
	start = clock();
	cout << "f_num  MinofAll  mov[0]  mov[1]  Iter  Ttable" << endl;
	TabuSearch(point_table, point_color, Tabutable, point_re, movement, f_number);
	finish = clock();
	cout << finish - start << "/" << CLOCKS_PER_SEC << "="<<(finish - start)/ CLOCKS_PER_SEC <<" (s) " << endl;
	system("pause");
	return 0;
}
//***************************************
