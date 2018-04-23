#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <time.h>

using namespace std;

const int point_num = 250;
const int k_num = 28;

int Iter = 0;
int f_number = point_num + 1;
int delt = 0;
int seed;

int movement[2] = { 0 };
int MinofAll = point_num;
int candidate[point_num][2] = {};
int candidate_tabu[point_num][2] = {};

int point_color[point_num] = { 0 };
//点与点之间是否在拓扑图上有联系
int point_re[point_num][point_num] = { 0 };
//关系表
int point_table[point_num][k_num] = { 0 };
int Tabutable[point_num][k_num] = { 0 };

//defination
int calF(int *color, int(*rmap)[point_num])
{
	//color为点颜色， rmap为关系对, point_num 记录冲突
	int flags = 0;

	for (int i = 0; i < point_num; i++)
	{
		for (int j = 0; j < point_num; j++)
		{
			if (color[i] == color[j] && rmap[i][j] == 1)
			{
				flags += 1;
				cout << i << "  " << j << endl;
			}
		}
	}
	flags = flags / 2;

	return flags;
}
//形成关系表 这个表有问题 改了第一次
void InitAll()
{
	//读取文件。形成拓扑图
	char initial;
	int flags = 0;
	int v, w;
	ifstream infile;

	infile.open("./DSJC_INSTANCE.txt");

	if (infile.fail())
	{
		cout << "error open!" << endl;
	}
	else
	{
		while (!infile.eof())
		{
			infile >> initial >> v >> w;
			point_re[v - 1][w - 1] = 1;
			point_re[w - 1][v - 1] = 1;
		}
	}
	infile.close();
	//涂色
	seed = time(NULL);
	srand((unsigned)seed);
	for (int i = 0; i < point_num; i++)
	{
		//srand((unsigned)time(NULL));
		point_color[i] = rand() % k_num;
	}
	//计算F以及形成表格
	for (int j = 0; j < point_num; j++)
	{
		int *p = point_re[j];
		for (int ik = 0; ik < point_num; ik++)//匹配到有关系的点的位置
		{
			if (p[ik] == 1)
			{
				point_table[j][point_color[ik]]++;
				if (point_color[j] == point_color[ik]) { flags++; }
			}
		}
	}
	f_number = flags / 2;
	MinofAll = f_number;
}
// 
void Findmove() {
	delt = point_num;
	int temp;//临时变量
	int tabu_delt = 10000;
	int count = 0, tabu_count = 0;
	int bestMove = MinofAll - f_number;
	int c_color;//当前结点颜色
	int *h_color;//邻接颜色表行首指针
	int *h_tabu;//禁忌表行首指针
	int c_color_table;//当前结点颜色表的值
	for (int i = 0; i<point_num; i++) {
		c_color = point_color[i];
		h_color = point_table[i];
		c_color_table = h_color[c_color];
		if (h_color[c_color] > 0) {
			h_tabu = Tabutable[i];
			for (int j = 0; j < k_num; j++) {
				if (c_color != j) {
					temp = h_color[j] - c_color_table;
					if (h_tabu[j] <= Iter)    //非禁忌动作
					{
						if (temp <= delt)
						{
							if (temp < delt)
							{
								count = 0;
								delt = temp;
							}
							candidate[count][0] = i;
							candidate[count][1] = j;
							count++;
						}
					}
					else {
						if (temp <= tabu_delt)
						{
							if (temp < tabu_delt)
							{
								tabu_delt = temp;
								tabu_count = 0;
							}
							candidate_tabu[tabu_count][0] = i;
							candidate_tabu[tabu_count][1] = j;
							tabu_count++;
						}
					}
				}
			}
		}
	}
	temp = 0;
	if (tabu_delt<bestMove && tabu_delt < delt) {
		delt = tabu_delt;
		temp = rand() % tabu_count;//相等delt随机选择
		movement[0] = candidate_tabu[temp][0];
		movement[1] = candidate_tabu[temp][1];
	}
	else {
		temp = rand() % count;//相等delt随机选择
		movement[0] = candidate[temp][0];
		movement[1] = candidate[temp][1];
	}
}

void MakeMove()
{
	int next_node = movement[0];
	int new_color = movement[1];
	int old_color = point_color[next_node];
	int *p_node = point_re[next_node];

	point_color[next_node] = new_color;
	for (int i = 0; i < point_num; i++)
	{
		if (p_node[i] == 1)
		{
			point_table[i][old_color]--;
			point_table[i][new_color]++;
		}
	}
	f_number = f_number + delt;
	Tabutable[next_node][new_color] = f_number + rand() % 20 + Iter;
	if (f_number < MinofAll)
	{
		MinofAll = f_number;
	}
	Iter++;
}

void document(int final_f, int SEC)
{
	fstream fout("./log.txt", ios::binary | ios::app | ios::in | ios::out);

		fout << "250.5.col " << "seed=" << seed << " " << final_f << " " << "DURATION=" << SEC << " " << "Iter/s=" << Iter / (SEC>0?SEC:1);
		for (int i = 0; i < point_num; i++)
		{
			fout << " " << point_color[i];
	    }
		fout << '\r' << '\n';
	fout.close();
}

int main()
{
	time_t start_t = clock();
	InitAll();
	while (f_number > 0)
	{
		Findmove();
		MakeMove();
	}
	time_t end_t = clock();
	int sec = (end_t - start_t) / CLOCKS_PER_SEC;
	int final_f = calF(point_color, point_re);
	document(final_f, sec);
	system("pause");
	return 0;
}
