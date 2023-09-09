#pragma once
#define MAX_CITY_NUM	20//支持的城市数目
#define MAX_NAME_LEN	12//城市名称最大长度
#define MAX_PATH_LEN	64//读取文件路径的最大长度
#define MAX_STRING_LEN	64//读取输入的最大长度
#define MAX_PATH_NUM	100//存储路径的最大数
#define ERROR			0
#define OK				1
#define ADMIN_KEY		"123456"
#define INF 999
#define scanf			scanf_s
#define strcpy(s_to, s)	strcpy_s(s_to, strlen(s) + 1, s)
#define fscanf			fscanf_s

typedef enum { false = 0, true } bool;
typedef bool Status;
typedef int VertexType;

//从出发城市到到达城市的列车的相关信息
typedef struct edgeinfo {
	char number[10];		//航班号或车次号，可根据第二位是否是字母判断是飞机还是火车
	int departure_time;		//出发时间（用一天中的分钟表示，e.g. 7：30 表示为 450）
	int arrival_time;		//到达时间（同上）
	int cost;				//费用
} edgeinfo;

//----------交通图的邻接表存储表示----------
//（请参考《数据结构(C语言版)》P163，严蔚敏，吴伟民，清华大学出版社）
typedef struct edgenode {
	int destination;			//当前路径指向的城市的编号
	struct edgenode* nextedge;	//指向下一条路径的指针
	edgeinfo info;				//当前路径的相关信息
} edgenode;
typedef struct CityNode {
	VertexType citynum;		//城市编号
	edgenode* firstedge;	//指向从该城市出发的第一条路径
} CityNode, CityList[MAX_CITY_NUM];
typedef struct {
	CityList cities;		//以邻接表形式存储的交通图
	int citynum, edgenum;	//交通图当前的城市数与路径数 
} TrafficMap;

char* CITY[MAX_NAME_LEN];//全局变量，用于存放城市名称与编号

typedef struct treenode {
	edgenode* edge;//边
	struct treenode* firstchild;//第一个孩子结点
	struct treenode* nextsib;//下一个兄弟
} treenode;

typedef struct pathnode {
	edgenode edge;//边
	struct pathnode* next;//下一个路径结点
} pathnode;


int findflag = 0;	//标记是否搜索到终点城市
int pathcount = 0;	//记录搜索到的路径个数

//初始化交通图 TM
extern Status InitTrafficMap(TrafficMap* TM);
//读取并建立交通图 TM
extern Status InsertTrafficMap(TrafficMap* TM);
//从终端输入交通图 TM
extern Status BuildFromUser(TrafficMap* TM);
//从文件输入交通图 TM
extern Status BuildFromFile(TrafficMap* TM);
//系统主菜单
extern Status main_menu(TrafficMap* TM);
//需求选择菜单
extern Status inquire_menu(TrafficMap* TM);
//管理面板
extern Status manage_menu(TrafficMap* TM);
//编辑城市
extern Status city_edit(TrafficMap* TM);
//从输入中读取编辑城市的指令
extern Status editCityFromUser(TrafficMap* TM);
//从文件中读取编辑城市的指令
extern Status editCityFromFile(TrafficMap* TM);
//编辑城市之间的路径
extern Status edge_edit(TrafficMap* TM);
//从输入中读取编辑路径的指令
extern Status editEdgeFromUser(TrafficMap* TM);
//从文件中读取编辑路径的指令
extern Status editEdgeFromFile(TrafficMap* TM);
//查找同一城市出发的上一条路径
extern edgenode* findpre(TrafficMap* TM, int i, edgenode* p);
//建立交通路径树
treenode* buildtree(TrafficMap* TM, int start);
//搜索end并将可行的路径存于path[]中
int Search(treenode* root, int end, pathnode* path[], int depth);
//将p插入到path[pathnum]中
Status Plugin(pathnode* path[], int pathnum, edgenode* p);
//挑选到达dest的最省钱的路径存于wanted[]中
int Sel_Path_money(pathnode* path[], pathnode* wanted[], int dest);
//挑选到达dest的最省时的路径存于wanted[]中
int Sel_Path_time(pathnode* path[], pathnode* wanted[], int dest);
