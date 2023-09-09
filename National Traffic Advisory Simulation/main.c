#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <conio.h>
#include "structure.h"

int main()
{
	printf("---------------------------全国交通咨询系统----------------------------\n");
	printf("正在初始化，请稍后...\n");
	TrafficMap TM;
	if (InitTrafficMap(&TM)) {
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("初始化完成。准备读取交通图。\n");
	}
	else {
		printf("初始化失败，请重新进入系统！\n");
		return 0;
	}
	if (InsertTrafficMap(&TM)) {
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("交通图输入完成。准备进入系统。");
	}
	else {
		printf("请重新进入系统！\n");
		return 0;
	}
	main_menu(&TM);
	return 0;
}

Status InitTrafficMap(TrafficMap* TM) {
	if (!TM) return ERROR;
	TM->citynum = 0; TM->edgenum = 0;
	for (int i = 0; i < MAX_CITY_NUM; i++) {
		TM->cities[i].citynum = 0;
		TM->cities[i].firstedge = NULL;
	}
	return OK;
}// InitTrafficMap

Status InsertTrafficMap(TrafficMap* TM) {
	printf("请选择输入方式：\n(0 = \"从终端输入\"; 1 = \"从文件输入\")\n");
	if (!(getchar() - '0')) {
		if (!BuildFromUser(TM)) {
			printf("读取失败！");
			return ERROR;
		}
	}
	else if (!BuildFromFile(TM)) {
		printf("读取失败！");
		return ERROR;
	}
	return OK;
}// InsertTrafficMap

Status BuildFromUser(TrafficMap* TM) {
	printf("请按如下方式输入交通图：\n");
	printf("（例：BeiJing TianJin G110 7:30 8:30 20\n");
	printf("以上的输入代表：G110 次列车 7 时 30 分从北京出发，8 时 30 分到达天津，票价 20 元。\n");
	printf("输入结束后请输入终止符 EOF 以结束读取。）\n"); getchar();
	char c = getchar();
	do {
		char tmp[MAX_NAME_LEN];
		int i, j, q;
		int from = 0, to = 0;//出发城市与到达城市的编号
		*tmp = c;
		for (j = 1; (tmp[j] = getchar()) != ' '; j++);
		tmp[j] = '\0';//读取出发城市
		for (i = 0; i < TM->citynum; i++) {
			if (strcmp(CITY[i], tmp) == 0) {//出发城市在交通图的第 i 个
				from = i;
				break;
			}
		}
		if (i == TM->citynum) {//出发城市不在交通图中
			CITY[i] = (char*)malloc(MAX_NAME_LEN * sizeof(char));
			int k;
			for (k = 0; tmp[k] != '\0'; k++) CITY[i][k] = tmp[k];
			CITY[i][k] = '\0';
			TM->citynum++;
			from = i;
		}
		TM->cities[i].citynum = i;
		for (j = 0; (tmp[j] = getchar()) != ' '; j++);
		tmp[j] = '\0';//读取到达城市
		for (q = 0; q < TM->citynum; q++) {
			if (strcmp(CITY[q], tmp) == 0) {//到达城市在交通图的第 i 个
				to = q;
				break;
			}
		}
		if (q == TM->citynum) {//到达城市不在交通图中
			CITY[q] = (char*)malloc(MAX_NAME_LEN * sizeof(char));
			int k;
			for (k = 0; tmp[k] != '\0'; k++) CITY[q][k] = tmp[k];
			CITY[q][k] = '\0';
			TM->citynum++;
			to = q;
		}
		edgenode** p = &(TM->cities[i].firstedge);
		while (*p) p = &((*p)->nextedge);
		(*p) = (edgenode*)malloc(sizeof(edgenode));
		(*p)->destination = to;
		(*p)->nextedge = NULL;
		scanf("%s", (*p)->info.number);
		int hour = 0, minute = 0;
		scanf("%d:%d", &hour, &minute);
		(*p)->info.departure_time = 60 * hour + minute;
		scanf("%d:%d", &hour, &minute);
		(*p)->info.arrival_time = 60 * hour + minute;
		scanf("%d", &((*p)->info.cost)); getchar();
		TM->edgenum++;
	} while ((c = getchar()) != EOF);
	return OK;
}// BuildFromUser

Status BuildFromFile(TrafficMap* TM) {
	char file_pos[MAX_PATH_LEN];
	printf("请输入文件地址：\n");
	scanf("%s", file_pos, MAX_PATH_LEN);
	errno_t err; FILE* fp;
	err = fopen_s(&fp, file_pos, "r");
	//FILE* fp = fopen(file_pos, "r");
	while (fp == NULL) {
		printf("文件读取失败！请重新输入文件地址：\n");
		scanf("%s", file_pos, MAX_PATH_LEN);
		err = fopen_s(&fp, file_pos, "r");
		//fp = fopen(file_pos, "r");
	}
	char input[MAX_STRING_LEN];//采取按行读取的方式，先将文件中一行内容读取到 input
	int i, j, q;
	do {
		fgets(input, MAX_STRING_LEN, fp);
		char tmp[MAX_NAME_LEN];
		int from = 0, to = 0;//出发城市与到达城市的编号
		for (j = 0; input[j] != ' '; j++) tmp[j] = input[j];
		tmp[j++] = '\0';//读取出发城市
		for (i = 0; i < TM->citynum; i++) {
			if (strcmp(CITY[i], tmp) == 0) {//出发城市在交通图的第 i 个
				from = i;
				break;
			}
		}
		if (i == TM->citynum) {//出发城市不在交通图中
			CITY[i] = (char*)malloc(MAX_NAME_LEN * sizeof(char));
			int k;
			for (k = 0; tmp[k] != '\0'; k++) CITY[i][k] = tmp[k];
			CITY[i][k] = '\0';
			TM->citynum++;
			from = i;
		}
		TM->cities[i].citynum = i;
		for (q = 0; input[j] != ' '; j++, q++) tmp[q] = input[j];
		tmp[q] = '\0'; j++;//读取到达城市
		for (q = 0; q < TM->citynum; q++) {
			if (strcmp(CITY[q], tmp) == 0) {//到达城市在交通图的第 q 个
				to = q;
				break;
			}
		}
		if (q == TM->citynum) {//到达城市不在交通图中
			CITY[q] = (char*)malloc(MAX_NAME_LEN * sizeof(char));
			int k;
			for (k = 0; tmp[k] != '\0'; k++) CITY[q][k] = tmp[k];
			CITY[q][k] = '\0';
			TM->citynum++;
			to = q;
		}
		edgenode** p = &(TM->cities[i].firstedge);
		while (*p) p = &((*p)->nextedge);
		(*p) = (edgenode*)malloc(sizeof(edgenode));
		(*p)->destination = to;
		(*p)->nextedge = NULL;
		for (q = 0; input[j] != ' '; j++, q++) (*p)->info.number[q] = input[j];
		(*p)->info.number[q] = '\0'; j++;
		int hour = 0, minute = 0;
		for (; input[j] != ':'; j++) hour = hour * 10 + input[j] - '0';
		j++;
		for (; input[j] != ' '; j++) minute = minute * 10 + input[j] - '0';
		j++;
		(*p)->info.departure_time = 60 * hour + minute;
		hour = 0; minute = 0;
		for (; input[j] != ':'; j++) hour = hour * 10 + input[j] - '0';
		j++;
		for (; input[j] != ' '; j++) minute = minute * 10 + input[j] - '0';
		j++;
		(*p)->info.arrival_time = 60 * hour + minute;
		(*p)->info.cost = 0;
		for (; input[j] != '\n' && input[j] != '\0'; j++) (*p)->info.cost = (*p)->info.cost * 10 + input[j] - '0';
		TM->edgenum++;
	} while (input[j] != '\0');
	return OK;
}// BuildFromFile

Status main_menu(TrafficMap* TM) {
	while (1) {
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("--------------------------------主菜单---------------------------------\n");
		printf("=======================================================================\n");
		printf("请选择操作：\n");
		printf("1: 用户界面\n");
		printf("2: 管理面板\n");
		printf("0: 退出\n");
		printf("=======================================================================\n");
	RE:
		printf("请选择: ");
		int inquire = 0;
		scanf("%d", &inquire);
		switch (inquire)
		{
		case 1:
			inquire_menu(TM);
			break;
		case 2:
			manage_menu(TM);
			break;
		case 0:
			printf("已退出交通咨询系统。\n");
			return OK;
		default:
			printf("输入有误！请输入 1/2/0\n");
			goto RE;
		}
	}
	return OK;
}// main_menu

Status inquire_menu(TrafficMap* TM) {
BACKTOINQUIRE:
	system("cls");
	printf("---------------------------全国交通咨询系统----------------------------\n");
	printf("-------------------------------用户界面--------------------------------\n");
	printf("=======================================================================\n");
	printf("欢迎您使用全国交通咨询系统！\n请选择期望的出行策略：\n");
	printf("1: 最快到达\n");
	printf("2: 最省钱到达\n");
	printf("0: 返回主菜单\n");
	printf("=======================================================================\n");
	printf("请选择: ");
	int inquire = 0;
INQUIREERR:
	scanf("%d", &inquire);
	system("cls");
	printf("---------------------------全国交通咨询系统----------------------------\n");
	printf("-------------------------------用户界面--------------------------------\n");
	char from[MAX_NAME_LEN], to[MAX_NAME_LEN];
	treenode* root;
	pathnode* path[MAX_PATH_NUM];
	pathnode* wanted[MAX_PATH_NUM];
	pathnode* p;
	int mid_city;
	switch (inquire) {
	case 1:
	CITYERR1:
		printf("请输入起点城市：\n");
		int from_num = 0, to_num = 0;
		scanf("%s", from, MAX_NAME_LEN);
		printf("请输入终点城市：\n");
		scanf("%s", to, MAX_NAME_LEN);
		int i;
		for (i = 0; i < TM->citynum; i++) {//在已有的城市里查找输入城市的编号
			if (strcmp(from, CITY[i]) == 0) {
				from_num = i;
				break;
			}
		}
		if (i == TM->citynum) {
			printf("不支持该起点城市或起点城市输入有误！\n");
			goto CITYERR1;
		}
		for (i = 0; i < TM->citynum; i++) {//在已有的城市里查找输入城市的编号
			if (strcmp(to, CITY[i]) == 0) {
				to_num = i;
				break;
			}
		}
		if (i == TM->citynum) {
			printf("不支持该终点城市或终点城市输入有误！\n");
			goto CITYERR1;
		}
		//重置变量
		findflag = 0;
		pathcount = 0;
		root = buildtree(TM, from_num, NULL);
		int least_duration;
		for (i = 0; i < MAX_PATH_NUM; i++)
			path[i] = wanted[i] = NULL;
		Search(root, to_num, path, 0);
		least_duration = Sel_Path_time(path, wanted, to_num);
		//dijkstra_money(TM, from_num, to_num);
		if (wanted[0] == NULL)
			printf("抱歉, 未找到可以抵达%s的方式\n", CITY[to_num]);
		else {
			printf("以下是为您找到的最省时旅程安排:\n");
			for (i = 0; i < MAX_PATH_NUM && wanted[i] != NULL; i++) {
				printf("[方案%d]:\n", i + 1);
				printf("起始站         终点站         航班/列车号    出发时间   到达时间   费用\n");
				mid_city = from_num;
				for (p = wanted[i]; p != NULL; p = p->next) {
					printf("%-15s%-15s%-15s%.2d:%.2d\t%.2d:%.2d%10d\n",
						CITY[mid_city], CITY[p->edge.destination], p->edge.info.number,
						(p->edge.info.departure_time) / 60, (p->edge.info.departure_time) % 60,
						(p->edge.info.arrival_time) / 60, (p->edge.info.arrival_time) % 60,
						p->edge.info.cost);
					mid_city = p->edge.destination;
				}
				printf("-----------------------------------------------------------------------\n");
				printf("耗时:%dh%.2dmin\n", least_duration / 60, least_duration % 60);
			}
		}
		printf("按回车键返回咨询菜单\n");
		getchar(); getchar();
		goto BACKTOINQUIRE;
		break;
		//dijkstra_time(TM, from_num, to_num);
		break;
	case 2:
	CITYERR2:
		printf("请输入起点城市：\n");
		from_num = 0; to_num = 0;
		scanf("%s", from, MAX_NAME_LEN);
		printf("请输入终点城市：\n");
		scanf("%s", to, MAX_NAME_LEN);
		for (i = 0; i < TM->citynum; i++) {//在已有的城市里查找输入城市的编号
			if (strcmp(from, CITY[i]) == 0) {
				from_num = i;
				break;
			}
		}
		if (i == TM->citynum) {
			printf("不支持该起点城市或起点城市输入有误！\n");
			goto CITYERR2;
		}
		for (i = 0; i < TM->citynum; i++) {//在已有的城市里查找输入城市的编号
			if (strcmp(to, CITY[i]) == 0) {
				to_num = i;
				break;
			}
		}
		if (i == TM->citynum) {
			printf("不支持该终点城市或终点城市输入有误！\n");
			goto CITYERR2;
		}
		//重置变量
		findflag = 0;
		pathcount = 0;
		root = buildtree(TM, from_num, NULL);
		int least_cost;
		for (i = 0; i < MAX_PATH_NUM; i++)
			path[i] = wanted[i] = NULL;
		Search(root, to_num, path, 0);
		least_cost = Sel_Path_money(path, wanted, to_num);
		//dijkstra_money(TM, from_num, to_num);
		if (wanted[0] == NULL)
			printf("抱歉, 未找到可以抵达%s的方式\n", CITY[to_num]);
		else {
			printf("以下是为您找到的最省钱旅程安排:\n");
			for (i = 0; i < MAX_PATH_NUM && wanted[i] != NULL; i++) {
				printf("[方案%d]:\n", i + 1);
				printf("起始站         终点站         航班/列车号    出发时间   到达时间   费用\n");
				mid_city = from_num;
				for (p = wanted[i]; p != NULL; p = p->next) {
					printf("%-15s%-15s%-15s%.2d:%.2d\t%.2d:%.2d%10d\n",
						CITY[mid_city], CITY[p->edge.destination], p->edge.info.number,
						(p->edge.info.departure_time) / 60, (p->edge.info.departure_time) % 60,
						(p->edge.info.arrival_time) / 60, (p->edge.info.arrival_time) % 60,
						p->edge.info.cost);
					mid_city = p->edge.destination;
				}
				printf("-----------------------------------------------------------------------\n");
				printf("合计:%66d\n", least_cost);
			}
		}
		printf("按回车键返回咨询菜单\n");
		getchar(); getchar();
		goto BACKTOINQUIRE;
		break;
	case 0:
		printf("正在返回主菜单...\n");
		break;
	default:
		printf("输入有误！请输入 1/2/0\n");
		goto INQUIREERR;
	}
	return OK;
}// inquire_menu

Status manage_menu(TrafficMap* TM) {
	while (1) {
		int mode;
		int chance;
		char user_key[MAX_STRING_LEN];
		int i;
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("-------------------------------管理菜单-------------------------------\n");
		printf("1: 城市编辑\n");
		printf("2: 列车/航班信息编辑\n");
		printf("0: 返回主菜单\n");
		printf("=====================================================================\n");
		printf("请选择: ");
		scanf("%d", &mode);
		switch (mode) {
		case 1:
			for (chance = 5; chance > 0; chance--) {
				printf("请输入管理员密码:\n");
				i = 0;
				char c;
				while ((c = _getch()) != '\r' && i < 8)
				{
					user_key[i] = c;
					putchar('*');
					i++;
				}
				user_key[i] = '\0';
				//scanf("%s", user_key, 8);
				if (strcmp(user_key, ADMIN_KEY) == 0) {
					city_edit(TM); break;
				}
				else if (chance == 2) printf("密码错误！这是你的最后一次机会！用尽后将返回主菜单。\n");
				else if (chance != 1)
					printf("密码错误！剩余机会：%d\n", chance - 1);
				else printf("密码错误！机会已用尽！即将返回主菜单...\n");
			}
			if (!chance) return ERROR;
			else break;
		case 2:
			for (chance = 5; chance > 0; chance--) {
				printf("请输入管理员密码:\n");
				i = 0;
				char c;
				while ((c = _getch()) != '\r' && i < 8)
				{
					user_key[i] = c;
					putchar('*');
					i++;
				}
				user_key[i] = '\0';
				//scanf("%s", user_key, 8);
				if (strcmp(user_key, ADMIN_KEY) == 0) {
					edge_edit(TM); break;
				}
				else if (chance == 2) printf("密码错误！这是你的最后一次机会！用尽后将返回主菜单。\n");
				else if (chance != 1)
					printf("密码错误！剩余机会：%d\n", chance - 1);
				else printf("密码错误！机会已用尽！即将返回主菜单...\n");
			}
			if (!chance) return ERROR;
			else break;
		case 0:
			printf("正在返回主菜单......");
			return OK;
		default: printf("输入错误，请重试\n");
		}
	}
	return OK;
}// manage_menu

Status city_edit(TrafficMap* TM) {
	while (1) {
		int mode;
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("-------------------------------管理菜单-------------------------------\n");
		printf("城市编辑\n");
		printf("选择编辑模式:\n");
		printf("1: 直接输入\n2: 文件输入\n0: 退出\n");
		printf("=====================================================================\n");
		printf("请选择: ");
		scanf("%d", &mode);
		switch (mode)
		{
		case 1:
			editCityFromUser(TM);
			break;
		case 2:
			editCityFromFile(TM);
			break;
		case 0:
			return OK;
		default:
			printf("输入错误，请重试\n");
		}
	}
	return OK;
}
Status edge_edit(TrafficMap* TM) {
	while (1) {
		int mode;
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("-------------------------------管理菜单-------------------------------\n");
		printf("列车/航班信息编辑\n");
		printf("选择编辑模式:\n");
		printf("1:直接输入\n2:文件输入\n0:退出\n");
		printf("=====================================================================\n");
		printf("请选择:");
		scanf("%d", &mode);
		switch (mode)
		{
		case 1:
			editEdgeFromUser(TM);
			break;
		case 2:
			editEdgeFromFile(TM);
			break;
		case 0:
			return OK;
		default:
			printf("输入错误，请重试\n");
		}
	}
	return OK;
}

Status editCityFromUser(TrafficMap* TM) {
	int mode;
	char temp[MAX_NAME_LEN];
	int i;
	while (1)
	{
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("-------------------------------管理菜单-------------------------------\n");
		printf("城市编辑\n");
		printf("请选择需要执行的操作:\n");
		printf("1: 添加\n2: 删除\n0: 退出编辑\n");
		printf("=====================================================================\n");
		printf("请选择: ");
		scanf("%d", &mode);
		switch (mode)
		{
		case 0:
			printf("正在退出...\n");
			return OK;
		case 1:
			printf("请输入需要添加的城市名: ");
			scanf("%s", temp, MAX_NAME_LEN);
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], temp) == 0)
					break;
			if (i != TM->citynum)
				printf("该城市已存在, 请重新输入！\n");
			else
			{
				TM->cities[i].citynum = i;
				TM->cities[i].firstedge = NULL;
				TM->citynum++;
				CITY[i] = (char*)malloc(MAX_NAME_LEN * sizeof(char));
				strcpy(CITY[i], temp);
			}
			printf("添加成功!\n");
			printf("按回车键返回上一级\n");
			getchar(); getchar();
			break;
		case 2:
			printf("请输入需要删除的城市名: ");
			scanf("%s", temp, MAX_NAME_LEN);
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], temp) == 0)
					break;
			if (i == TM->citynum)
				printf("该城市未存在, 请重新输入！\n");
			else
			{
				//修改CITY[]
				int j;
				for (j = i; j < TM->citynum - 1; j++) {
					CITY[j] = CITY[j + 1];
				}
				CITY[j] = NULL;
				//修改TM->cityies数组
				for (j = i; j < TM->citynum; j++)
					TM->cities[j] = TM->cities[j + 1];
				TM->cities[j].citynum = 0;
				TM->cities[j].firstedge = NULL;
				TM->citynum--;
				//修改所有影响到的边
				for (j = 0; j < TM->citynum; j++) {
					edgenode* p = TM->cities[j].firstedge;
					edgenode* q = NULL;
					while (p != NULL) {
						//去往删掉的城市的边，直接删除
						if (p->destination == i) {
							q = findpre(TM, i, p);
							if (p == TM->cities[j].firstedge)
								TM->cities[j].firstedge = p->nextedge;
							else
								q->nextedge = p->nextedge;
							edgenode* temp = p;
							p = p->nextedge;
							free(temp);
						}
						//去往邻接表下标变化的城市，将destination值减一
						else if (p->destination > i) {
							p->destination--;
							p = p->nextedge;
						}
						else
							p = p->nextedge;
					}
				}
			}
			printf("删除成功!\n");
			printf("按回车键返回上一级\n");
			getchar(); getchar();
			break;
		default:
			printf("输入错误，请重试\n");
			printf("按回车键返回上一级\n");
			getchar(); getchar();
			break;
		}
	}
	return	OK;
}// editCityFromUser

Status editCityFromFile(TrafficMap* TM) {
L1:
	system("cls");
	printf("---------------------------全国交通咨询系统----------------------------\n");
	printf("-------------------------------管理菜单-------------------------------\n");
	printf("城市编辑\n");
	printf("请选择:\n1:从文件中编辑 2:退出编辑\n");
	int contin;
	scanf("%d", &contin);
	if (contin == 2)
		return OK;
	else if (contin != 1 && contin != 2)
	{
		printf("输入错误，请重新选择");
		goto L1;
	}
	printf("请确保文件中的每一行有以下格式:\n");
	printf("操作数字 城市名\n");
	printf("其中操作数字: 1: 添加\n2: 删除\n");
	char file_pos[MAX_PATH_LEN];
	printf("请输入文件地址：\n");
	scanf("%s", file_pos, MAX_PATH_LEN);

	errno_t err; FILE* fp;
	err = fopen_s(&fp, file_pos, "r");
	//FILE* fp = fopen(file_pos, "r");
	while (fp == NULL) {
		printf("文件读取失败！请重新输入文件地址：\n");
		scanf("%s", file_pos, MAX_PATH_LEN);
		err = fopen_s(&fp, file_pos, "r");
		//fp = fopen(file_pos, "r");
	}
	char input[MAX_STRING_LEN];
	char temp[MAX_STRING_LEN];
	int i;
	int len;
	do {
		char* succ = fgets(input, MAX_STRING_LEN, fp);
		if (succ == NULL)
			break;
		int mode = input[0] - '0';
		switch (mode)
		{
		case 1:
			strcpy(temp, input + 2);
			len = strlen(temp);
			if (temp[len - 1] == '\n')
				temp[len - 1] = '\0';
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], temp) == 0)
					break;
			if (i != TM->citynum)
				printf("%s 城市已存在, 请检查输入文本后重新进行添加\n", temp);
			else
			{
				TM->cities[i].citynum = i;
				TM->cities[i].firstedge = NULL;
				TM->citynum++;
				CITY[i] = (char*)malloc(MAX_NAME_LEN * sizeof(char));
				strcpy(CITY[i], temp);
			}
			printf("添加成功!\n");
			break;
		case 2:
			strcpy(temp, input + 2);
			if (temp[len - 1] == '\n')
				temp[len - 1] = '\0';
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], temp) == 0)
					break;
			if (i == TM->citynum)
				printf("%s 城市未存在, 请检查输入文本后重新进行删除\n", temp);
			else
			{
				//修改CITY[]
				int j;
				for (j = i; j < TM->citynum - 1; j++) {
					CITY[j] = CITY[j + 1];
				}
				CITY[j] = NULL;
				//修改TM->cityies数组
				for (j = i; j < TM->citynum; j++)
					TM->cities[j] = TM->cities[j + 1];
				TM->cities[j].citynum = 0;
				TM->cities[j].firstedge = NULL;
				TM->citynum--;
				//修改所有影响到的边
				for (j = 0; j < TM->citynum; j++) {
					edgenode* p = TM->cities[j].firstedge;
					edgenode* q = NULL;
					while (p != NULL) {
						//去往删掉的城市的边，直接删除
						if (p->destination == i) {
							q = findpre(TM, i, p);
							if (p == TM->cities[j].firstedge)
								TM->cities[j].firstedge = p->nextedge;
							else
								q->nextedge = p->nextedge;
							edgenode* temp = p;
							p = p->nextedge;
							free(temp);
						}
						//去往邻接表下标变化的城市，将destination值减一
						else if (p->destination > i) {
							p->destination--;
							p = p->nextedge;
						}
						else
							p = p->nextedge;
					}
				}
			}
			printf("删除成功!\n");
			break;
		default:
			printf("文件输入错误!\n");
		}
	} while (1);
	return	OK;
}// editCityFromFile

Status editEdgeFromUser(TrafficMap* TM) {
	while (1)
	{
		system("cls");
		printf("---------------------------全国交通咨询系统----------------------------\n");
		printf("-------------------------------管理菜单-------------------------------\n");
		printf("列车/航班信息编辑\n");
		printf("请选择需要执行的操作:\n");
		printf("1: 添加\n2: 删除\n0: 退出编辑\n");
		printf("=====================================================================\n");
		printf("请选择: ");
		int mode;
		char from[MAX_NAME_LEN];
		char to[MAX_NAME_LEN];
		char number[10];
		char departure_time[6];
		int depart = 0;
		char arrival_time[6];
		int arrival = 0;
		int hour = 0;
		int minute = 0;
		int price;
		int from_num;
		int to_num;
		int i;
		edgenode* p = NULL;
		scanf("%d", &mode);
		switch (mode)
		{
		case 0:
			printf("正在退出...\n");
			return OK;
		case 1:
			printf("请用一下格式输入需要添加的列车/航班信息\n");
			printf("出发地 目的地 列车/航班号 出发时间 到达时间 票价\n");
			printf("请输入: ");
			scanf("%s %s %s %s %s", from, MAX_NAME_LEN, to, MAX_NAME_LEN, number, 10, departure_time, 6, arrival_time, 6);
			scanf(" %d", &price);
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], from) == 0)
					break;
			if (i == TM->citynum)
			{
				printf("出发城市%s不存在, 请重新进行添加\n", from);
				break;
			}
			else
				from_num = i;
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], to) == 0)
					break;
			if (i == TM->citynum)
			{
				printf("到达城市%s不存在, 请重新进行添加\n", to);
				break;
			}
			else
				to_num = i;
			for (i = 0; i < 6 && departure_time[i] != ':'; i++) {
				hour = hour * 10 + departure_time[i] - '0';
			}
			for (i++; i < 6 && departure_time[i] != '\0'; i++) {
				minute = minute * 10 + departure_time[i] - '0';
			}
			depart = hour * 60 + minute;
			hour = minute = 0;
			for (i = 0; i < 6 && arrival_time[i] != ':'; i++) {
				hour = hour * 10 + arrival_time[i] - '0';
			}
			for (i++; i < 6 && arrival_time[i] != '\0'; i++) {
				minute = minute * 10 + arrival_time[i] - '0';
			}
			arrival = hour * 60 + minute;
			//写入map
			edgenode* new = (edgenode*)malloc(sizeof(edgenode));
			for (p = TM->cities[from_num].firstedge; p != NULL && p->nextedge != NULL; p = p->nextedge)
				;
			new->destination = to_num;
			new->info.arrival_time = arrival;
			new->info.departure_time = depart;
			new->info.cost = price;
			strcpy(new->info.number, number);
			new->nextedge = NULL;
			if (p == NULL)
				TM->cities[from_num].firstedge = new;
			else
				p->nextedge = new;
			break;
		case 2:
			printf("请输入需要删除的列车/航班号:\n");
			scanf("%s", number, 10);
			int findflag = 0;
			for (i = 0; i < TM->citynum; i++)
			{
				for (p = TM->cities[i].firstedge; p != NULL; p = p->nextedge) {
					if (strcmp(p->info.number, number) == 0)
					{
						findflag = 1;
						break;
					}
				}
				if (findflag == 1)
					break;
			}
			if (findflag == 1) {
				edgenode* pre = findpre(TM, i, p);
				if (p == TM->cities[i].firstedge)
					TM->cities[i].firstedge = p->nextedge;
				else
					pre->nextedge = p->nextedge;
				free(p);
				p = NULL;
			}
			else
				printf("未查到该航班, 请重新操作！");
			break;
		default:
			printf("输入错误，请重试\n");
		}
	}
	return OK;
}// editEdgeFromUser

Status editEdgeFromFile(TrafficMap* TM) {
L2:
	system("cls");
	printf("---------------------------全国交通咨询系统----------------------------\n");
	printf("-------------------------------管理菜单-------------------------------\n");
	printf("列车/航班信息编辑\n");
	printf("请选择:\n1:从文件中编辑 2:退出编辑\n");
	int contin;
	scanf("%d", &contin);
	if (contin == 2)
		return OK;
	else if (contin != 1 && contin != 2)
	{
		printf("输入错误，请重新选择");
		goto L2;
	}
	printf("若选择添加, 请确保文件中用一下格式存储需要添加的列车/航班信息\n");
	printf("出发地 目的地 列车/航班号 出发时间 到达时间 票价\n");
	printf("若选择删除, 请确保文件中每一行为一个列车/航班号\n");
	printf("请选择要执行的操作:\n");
	printf("1: 添加, 2: 删除, 0: 退出\n");
	printf("请选择:\n");
	int	 mode;
	scanf("%d", &mode);
	char file_pos[MAX_PATH_LEN];
	errno_t err; FILE* fp;
	char from[MAX_NAME_LEN];
	char to[MAX_NAME_LEN];
	char number[10];
	char departure_time[6];
	int  depart = 0;
	char arrival_time[6];
	int  arrival = 0;
	int  hour = 0;
	int  minute = 0;
	int  price;
	int  from_num;
	int  to_num;
	int  i = 0;
	int  c;
	edgenode* p = NULL;
	if (mode == 1)
	{
		printf("请输入文件地址:\n");
		scanf("%s", file_pos, MAX_PATH_LEN);
		err = fopen_s(&fp, file_pos, "r");
		//FILE* fp = fopen(file_pos, "r");
		while (fp == NULL) {
			printf("文件读取失败！请重新输入文件地址：\n");
			scanf("%s", file_pos, MAX_NAME_LEN);
			err = fopen_s(&fp, file_pos, "r");
			//fp = fopen(file_pos, "r");
			return ERROR;
		}
		do {
			if (fscanf(fp, "%s %s %s %s %s %d", from, MAX_NAME_LEN, to, MAX_NAME_LEN, number, 10, departure_time, 6, arrival_time, 6, &price) == EOF)
				break;
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], from) == 0)
					break;
			if (i == TM->citynum)
			{
				printf("出发城市%s不存在, 请重新进行添加\n", from);
				return ERROR;
			}
			else
				from_num = i;
			for (i = 0; i < TM->citynum; i++)
				if (strcmp(CITY[i], to) == 0)
					break;
			if (i == TM->citynum)
			{
				printf("到达城市%s不存在, 请重新进行添加\n", to);
				return ERROR;
			}
			else
				to_num = i;
			for (i = 0; i < 6 && departure_time[i] != ':'; i++) {
				hour = hour * 10 + departure_time[i] - '0';
			}
			for (i++; i < 6 && departure_time[i] != '\0'; i++) {
				minute = minute * 10 + departure_time[i] - '0';
			}
			depart = hour * 60 + minute;
			hour = minute = 0;
			for (i = 0; i < 6 && arrival_time[i] != ':'; i++) {
				hour = hour * 10 + arrival_time[i] - '0';
			}
			for (i++; i < 6 && arrival_time[i] != '\0'; i++) {
				minute = minute * 10 + arrival_time[i] - '0';
			}
			arrival = hour * 60 + minute;
			//写入map
			edgenode* new = (edgenode*)malloc(sizeof(edgenode));
			for (p = TM->cities[from_num].firstedge; p != NULL && p->nextedge != NULL; p = p->nextedge)
				;
			new->destination = to_num;
			new->info.arrival_time = arrival;
			new->info.departure_time = depart;
			new->info.cost = price;
			strcpy(new->info.number, number);
			new->nextedge = NULL;
			if (p == NULL)
				TM->cities[from_num].firstedge = new;
			else
				p->nextedge = new;
			break;
		} while ((c = getchar()) != EOF);
	}
	else if (mode == 2) {
		printf("请输入文件地址：\n");
		scanf("%s", file_pos, MAX_NAME_LEN);
		err = fopen_s(&fp, file_pos, "r");
		//FILE* fp = fopen(file_pos, "r");
		while (fp == NULL) {
			printf("文件读取失败！请重新输入文件地址：\n");
			scanf("%s", file_pos, MAX_NAME_LEN);
			err = fopen_s(&fp, file_pos, "r");
			//fp = fopen(file_pos, "r");
			return ERROR;
		}
		do {
			if (fscanf(fp, "%s", number) == EOF)
				break;
			int findflag = 0;
			p = TM->cities[i].firstedge;
			for (i = 0; i < TM->citynum; i++)
			{
				for (; p != NULL; p = p->nextedge) {
					if (strcmp(p->info.number, number) == 0)
					{
						findflag = 1;
						break;
					}
				}
				if (findflag == 1)
					break;
			}
			if (findflag == 1) {
				edgenode* pre = findpre(TM, i, p);
				if (p == TM->cities[i].firstedge)
					TM->cities[i].firstedge = p->nextedge;
				else
					pre->nextedge = p->nextedge;
				free(p);
				p = NULL;
			}
			else
				printf("未查到该航班, 请重新操作！");
		} while ((c = getchar()) != EOF);
	}
	else if (mode == 0)
	{
		printf("正在退出, 请稍后...\n");
		return OK;
	}
	else {
		printf("选择模式有误，请重新操作\n");
		return ERROR;
	}
	return OK;
}// editEdgeFromFile

edgenode* findpre(TrafficMap* TM, int i, edgenode* p) {
	if (TM->cities[i].firstedge == p)
	{
		return TM->cities[i].firstedge;
	}
	edgenode* pre;
	for (pre = TM->cities[i].firstedge; pre != NULL; pre = pre->nextedge)
		if (pre->nextedge != NULL && pre->nextedge == p)
			return pre;
}// findpre

treenode* buildtree(TrafficMap* TM, int start, edgenode* pre) {
	edgenode* q = TM->cities[start].firstedge;
	treenode* root = (treenode*)malloc(sizeof(treenode));
	treenode* p;
	treenode* k = root;
	int first = 1;
	if (q == NULL)
		return NULL;
	else
	{
		if (pre == NULL) {
			root->edge = q;
			root->firstchild = buildtree(TM, q->destination, q);
			root->nextsib = NULL;
			for (q = TM->cities[start].firstedge->nextedge; q != NULL; q = q->nextedge) {
				p = (treenode*)malloc(sizeof(treenode));
				k->nextsib = p;
				k = p;
				p->edge = q;
				p->firstchild = buildtree(TM, q->destination, q);
				p->nextsib = NULL;
			}
		}
		else
		{
			for (q = TM->cities[start].firstedge; q != NULL; q = q->nextedge) {
				if (q->info.departure_time > pre->info.arrival_time && !(isdigit(q->info.number[1]) ^ isdigit(pre->info.number[1])))
				{
					if (first == 1) {
						root->edge = q;
						root->firstchild = buildtree(TM, q->destination, q);
						root->nextsib = NULL;
						first = 0;
					}
					else {
						p = (treenode*)malloc(sizeof(treenode));
						k->nextsib = p;
						k = p;
						p->edge = q;
						p->firstchild = buildtree(TM, q->destination, q);
						p->nextsib = NULL;
					}
				}
			}
		}
	}

	return root;
}//buildtree

int Search(treenode* root, int end, pathnode* path[], int depth) {
	treenode* p = NULL;
	for (p = root; p != NULL; p = p->nextsib) {
		//找到终点城市
		if (p->edge->destination == end) {
			findflag = 1;
			Plugin(path, pathcount, p->edge);//插入path
		}
		//未找到，递归搜索孩子
		else {
			findflag = 0;//递归前重置标记
			Search(p->firstchild, end, path, depth + 1);//深度加1
			//孩子中含终点城市
			if (findflag == 1) {
				Plugin(path, pathcount, p->edge);//插入path
				if (depth == 0)//追溯到深度为0，则写入路径已完成，准备写下一条路径，并重置标记
				{
					pathcount++;
					findflag = 0;
				}
			}
		}
	}
	return OK;
}//Search

Status Plugin(pathnode* path[], int pathnum, edgenode* p) {
	pathnode* tmp = (pathnode*)malloc(sizeof(pathnode));
	pathnode* ptr;
	//for (; q != NULL && q->next != NULL; q = q->next);
	//填入path[pathnum]的首位（考虑path[num]为空）
	if (path[pathnum] == NULL)
	{
		tmp->edge = *p;
		tmp->next = NULL;
		path[pathnum] = tmp;
	}
	else {
		ptr = path[pathnum];
		tmp->edge = *p;
		tmp->next = ptr;
		path[pathnum] = tmp;
	}
	return OK;
}//Plugin


int Sel_Path_money(pathnode* path[], pathnode* wanted[], int dest) {
	int number = 0;//wanted数组下标
	int money = 0;//记录该路径的费用
	int min_money = 10000;
	int last_money = 10000;
	pathnode* ptr_w;//指向wanted中路径结点的指针
	pathnode* ptr_p;//指向path中路径结点的指针
	pathnode* tmp;
	for (int i = 0; i < MAX_PATH_NUM && path[i] != NULL; i++) {
		money = 0;
		if (path[i]->edge.destination != dest)//非一步到达的路径
		{
			tmp = (pathnode*)malloc(sizeof(pathnode));
			tmp->edge = path[i]->edge;
			tmp->next = NULL;
			wanted[number] = tmp;
			ptr_w = tmp;
			money += tmp->edge.info.cost;//更新money

			//写入所有的到达终点前一步的结点
			for (ptr_p = path[i]->next; ptr_p != NULL && ptr_p->edge.destination != dest; ptr_p = ptr_p->next) {
				tmp = (pathnode*)malloc(sizeof(pathnode));
				tmp->edge = ptr_p->edge;
				tmp->next = NULL;
				ptr_w->next = tmp;
				ptr_w = tmp;
				money += tmp->edge.info.cost;
			}
			tmp = (pathnode*)malloc(sizeof(pathnode));
			last_money = 10000;
			//处理到达目的地的最后一步
			for (; ptr_p != NULL; ptr_p = ptr_p->next) {
				if (last_money > ptr_p->edge.info.cost)
				{
					last_money = ptr_p->edge.info.cost;
					tmp = (pathnode*)malloc(sizeof(pathnode));
					tmp->edge = ptr_p->edge;
					tmp->next = NULL;
					ptr_w->next = tmp;
				}
			}
			money += last_money;
		}
		//一步到达的路径
		else {
			last_money = 10000;
			for (ptr_p = path[i]; ptr_p != NULL; ptr_p = ptr_p->next) {
				//找到最小直接更新
				if (last_money > ptr_p->edge.info.cost)
				{
					last_money = ptr_p->edge.info.cost;
					tmp = (pathnode*)malloc(sizeof(pathnode));
					tmp->edge = ptr_p->edge;
					tmp->next = NULL;
					wanted[number] = tmp;
				}
			}
			money = last_money;
		}
		//若费用小于最小费用则写入wanted[0]，其余删除
		if (money < min_money)
		{
			wanted[0] = wanted[number];
			for (int j = 1; j <= number; j++)
				wanted[j] = NULL;
			min_money = money;
			number = 1;
		}
		//若费用为目前最低费用，则保留，number计数加一
		else if (money == min_money) {
			number++;
		}
		//若费用大于目前最低费用，则清楚刚写入的wanted[number]路径
		else
			wanted[number] = NULL;
	}
	return min_money;
}//Sel_Path_money

int Sel_Path_time(pathnode* path[], pathnode* wanted[], int dest) {
	int number = 0;
	int min_duration = 10000;
	int start_time, end_time;
	pathnode* ptr_w;
	pathnode* ptr_p;
	pathnode* tmp;
	for (int i = 0; i < MAX_PATH_NUM && path[i] != NULL; i++) {
		if (path[i]->edge.destination != dest)
		{
			tmp = (pathnode*)malloc(sizeof(pathnode));
			tmp->edge = path[i]->edge;
			tmp->next = NULL;
			wanted[number] = tmp;
			start_time = tmp->edge.info.departure_time;
			ptr_w = tmp;
			for (ptr_p = path[i]->next; ptr_p != NULL && ptr_p->edge.destination != dest; ptr_p = ptr_p->next) {
				tmp = (pathnode*)malloc(sizeof(pathnode));
				tmp->edge = ptr_p->edge;
				tmp->next = NULL;
				ptr_w->next = tmp;
				ptr_w = tmp;
			}
			tmp = (pathnode*)malloc(sizeof(pathnode));
			end_time = 10000;
			for (; ptr_p != NULL; ptr_p = ptr_p->next) {
				if (end_time > ptr_p->edge.info.arrival_time)
				{
					end_time = ptr_p->edge.info.arrival_time;
					tmp = (pathnode*)malloc(sizeof(pathnode));
					tmp->edge = ptr_p->edge;
					tmp->next = NULL;
					ptr_w->next = tmp;
				}
			}
		}
		else
		{
			end_time = 10000;
			start_time = 0;
			for (ptr_p = path[i]; ptr_p != NULL; ptr_p = ptr_p->next) {
				if ((ptr_p->edge.info.arrival_time - ptr_p->edge.info.departure_time) < (end_time - start_time))
				{
					end_time = ptr_p->edge.info.arrival_time;
					start_time = ptr_p->edge.info.departure_time;
					tmp = (pathnode*)malloc(sizeof(pathnode));
					tmp->edge = ptr_p->edge;
					tmp->next = NULL;
					wanted[number] = tmp;

				}
			}
		}
		if ((end_time - start_time) < min_duration)
		{
			wanted[0] = wanted[number];
			for (int j = 1; j <= number; j++)
				wanted[j] = NULL;
			min_duration = end_time - start_time;
			number = 1;
		}
		else if ((end_time - start_time) == min_duration) {
			number++;
		}
		else
			wanted[number] = NULL;
	}
	return min_duration;
}//Sel_Path_time
