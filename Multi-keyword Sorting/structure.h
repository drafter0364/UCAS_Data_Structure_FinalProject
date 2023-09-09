#pragma once
#define MAXSIZE 10000
#define MAX_KEY_NUM 6
#define MAX_KEY_SIZE 100
#define MAX_PATH_LEN 64
#define MAX_STRING_LEN 64
#define OK    			1
#define ERROR 			0
#define INFEASIBLE 		-1
#define OVERFLOW   		-2
#define LIST_INIT_SIZE 	10000
#define LISTINCREMENT  	100
#define scanf scanf_s

typedef enum { false = 0, true } bool;
typedef bool Status;
typedef int ElemType;
typedef int KeyType;
typedef struct {
	int length;//当前长度
	int listsize;//当前分配的存储容量
	int keynum;//关键字的数目
} InfoType;
typedef struct {
	KeyType key[MAXSIZE];//关键词矩阵
	char name[MAX_NAME_LEN];//当前关键词名称
} keyword;
typedef struct {
	ElemType* elem;//存储空间基址
	keyword keys[MAX_KEY_NUM];//关键词
	InfoType info;
} SqList;//采用顺序表存储结构

int* num, *order;//用于存放排序后的序号

//用于排序所用时长
typedef struct RunTime
{
	clock_t start;
	clock_t end;
	double duration;
}RunTime;

RunTime RS_time, LSDM_time;

//构造一个空的线性表 L 
extern Status InitList(SqList* L);
//建立顺序表的存储结构
extern Status BuildList(SqList* L);
//基数为 10 的 LSD 基数排序，采取“分配”和“收集”的策略
extern Status Radix_Sort_10(SqList L, int inquire);
//输出到终端中
extern Status PrintToUser(SqList L);
//输出到文件中
extern Status PrintToFile_RS(SqList L);
//输出到文件中
extern Status PrintToFile_RS(SqList L);
//LSDmerge排序的启动程序
extern int LSDMerge(SqList* L);
//LSDmerge排序的递归调用部分
extern int LSDMergeSort(SqList* L, int left, int right, int keynum);
//LSDmemrge合并部分
extern int LSDmerge(SqList* L, int left, int mid, int right, int keynum);
