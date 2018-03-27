#define k_num 3
#define point_num 9

extern int iter = 0;
extern int f_number = 0;

extern int Tmovement[3] = {0};
extern int NTmovement[3] = {0};
extern int movement[3] = {0};
extern int MinF = 0;

extern int Ttable[point_num][k_num];

extern struct Point
{
	//点颜色 0红1蓝2绿
	int color[point_num] = {0};
	//点与点之间是否在拓扑图上有联系
	int re[point_num][point_num] = { 0 };
	//关系表
	/*int table[point_num][k_num] = { { 2, 0, 1 },{ 1, 2, 0 },{ 1, 0, 1 },
	{ 1, 0, 2 },{ 0, 1, 2 },{ 0, 1, 0 },{ 1, 1, 1 },{ 2, 1, 1 },{ 0, 0, 2 } };*/
	int table[point_num][k_num] = { 0 };
	//冲突点数及记录
	int f_num = 0;
	int f[point_num] = {0};
};
extern Point point;
//init
extern void initM(int rmap[][point_num]);

extern void init(int *pint, int (*rmap)[point_num]);
//