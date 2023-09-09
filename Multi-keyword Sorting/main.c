#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include "structure.h"

int main()
{
    SqList List;
    InitList(&List);
    BuildList(&List);
    num = (int*)malloc((List.info.length + 1) * sizeof(int));
    order = (int*)malloc((List.info.length + 1) * sizeof(int));
    for (int i = 0; i < List.info.length; i++) { num[i] = i; order[i] = List.elem[i]; }

    printf("支持的关键词如下：\n");
    printf("1: chinese\n2: math\n3: english\n4: physics\n5: chem\n6: total\n");
CHOOSE:
    printf("请选择排序顺序：(如：1-2-3-4-5-6)\n");
    int inquire[6];
    for (int i = 0; i < 6; i++) {
        scanf("%d", &inquire[i]); getchar();
        if (inquire[i] < 1 || inquire[i] > 6) {
            printf("输入错误！请重新输入。\n");
            goto CHOOSE;
        }
    }
    RS_time.start = clock();
    for (int i = 5; i >= 0; i--) Radix_Sort_10(List, inquire[i] - 1);
    RS_time.end = clock();
    RS_time.duration = ((double)(RS_time.end - RS_time.start)) / CLOCKS_PER_SEC;
    PrintToFile_RS(List, "RS_sorted.txt");

    LSDM_time.start = clock();
    LSDMerge(&List);
    LSDM_time.end = clock();
    LSDM_time.duration = ((double)(LSDM_time.end - LSDM_time.start)) / CLOCKS_PER_SEC;
    PrintToFile_LSDM(List, "LSDM_sorted.txt");

    printf("LSD稳定的内部排序法所用时间为：%lf s\n", LSDM_time.duration);
    printf("LSD基数排序所用时间为：%lf s\n", RS_time.duration);
    return 0;
}

Status InitList(SqList* L) {
    L->elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));//存储分配失败
    if (!L->elem) exit(OVERFLOW);
    L->info.length = 0;
    L->info.listsize = LIST_INIT_SIZE;
    L->info.keynum = 0;
    return OK;
}//InitList

Status BuildList(SqList* L) {
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
    fgets(input, MAX_STRING_LEN, fp);
    int i = 0, j;
    for (j = 0; input[j] != '\t'; j++);
    while (input[j++] != '\n') {
        for (; input[j] != '\t' && input[j] != '\n'; j++) {
            L->keys[L->info.keynum].name[i++] = input[j];
        }
        L->keys[L->info.keynum].name[i] = '\0';
        i = 0; L->info.keynum++;
    }
    do {
        fscanf_s(fp, "%d", &L->elem[L->info.length]);
        for (int k = 0; k < L->info.keynum; k++) {
            fscanf_s(fp, "%d", &L->keys[k].key[L->info.length]);
        }
        L->info.length++;
    } while (fgetc(fp) == '\n');
}// BuildList


Status Radix_Sort_10(SqList L, int inquire) {
    int Base[10][MAXSIZE];
    int* sort, * sort_next, * tmp;
    int* num_next;
    sort_next = (int*)malloc((L.info.length + 1) * sizeof(int));
    for (int i = 0; i < L.info.length; i++)
        sort_next[i] = L.keys[inquire].key[num[i]];
    tmp = (int*)malloc((L.info.length + 1) * sizeof(int));
    for (int j = 1; j < 5; j++) {
        sort = (int*)malloc((L.info.length + 1) * sizeof(int));
        num_next = (int*)malloc((L.info.length + 1) * sizeof(int));
        for (int i = 0; i < 10; i++) Base[i][0] = 0;
        for (int i = 0; i < L.info.length; i++) {
            tmp[i] = (sort_next[i] / (int)pow(10, j - 1)) % 10;
            Base[tmp[i]][0]++;
            Base[tmp[i]][Base[tmp[i]][0]] = sort_next[i];
        }
        for (int i = 1; i < 10; i++) Base[i][0] += Base[i - 1][0];
        for (int i = L.info.length - 1; i >= 0; i--) {
            Base[tmp[i]][0]--;
            sort[Base[tmp[i]][0]] = sort_next[i];
            num_next[Base[tmp[i]][0]] = num[i];
        }
        sort_next = sort;
        num = num_next;
    }
    return OK;
}// Radix_Sort_10

Status PrintToUser(SqList L) {
    printf("id\t");
    for (int i = 0; i < L.info.keynum - 1; i++) printf("%s\t", L.keys[i].name);
    printf("%s\n", L.keys[L.info.keynum - 1].name);
    int k = 0;
    for (int i = 0; i < L.info.length; i++) {
        printf("%d\t", L.elem[num[i]]);
        for (int j = 0; j < L.info.keynum - 1; j++)
            printf("%d\t", L.keys[j].key[num[i]]);
        printf("%d\n", L.keys[L.info.keynum - 1].key[num[i]]);
    }
    return OK;
}// PrintToUser

Status PrintToFile_RS(SqList L, char* filepos) {
    errno_t err; FILE* fp;
    err = fopen_s(&fp, filepos, "w");
    fprintf(fp, "id\t");
    for (int i = 0; i < L.info.keynum - 1; i++) fprintf(fp, "%s\t", L.keys[i].name);
    fprintf(fp, "%s\n", L.keys[L.info.keynum - 1].name);
    int k = 0;
    for (int i = 0; i < L.info.length; i++) {
        fprintf(fp, "%d\t", L.elem[num[i]]);//num[i]/order[i] - order[0]
        for (int j = 0; j < L.info.keynum - 1; j++)
            fprintf(fp, "%d\t", L.keys[j].key[num[i]]);
        fprintf(fp, "%d\n", L.keys[L.info.keynum - 1].key[num[i]]);
    }
    return OK;
}// PrintToFile_RS

Status PrintToFile_LSDM(SqList L, char* filepos) {
    errno_t err; FILE* fp;
    err = fopen_s(&fp, filepos, "w");
    fprintf(fp, "id\t");
    for (int i = 0; i < L.info.keynum - 1; i++) fprintf(fp, "%s\t", L.keys[i].name);
    fprintf(fp, "%s\n", L.keys[L.info.keynum - 1].name);
    int k = 0;
    for (int i = 0; i < L.info.length; i++) {
        fprintf(fp, "%d\t", L.elem[order[i] - order[0]]);
        for (int j = 0; j < L.info.keynum - 1; j++)
            fprintf(fp, "%d\t", L.keys[j].key[order[i] - order[0]]);
        fprintf(fp, "%d\n", L.keys[L.info.keynum - 1].key[order[i] - order[0]]);
    }
    return OK;
}// PrintToFile_LSDM

int LSDmerge(SqList* L, int left, int mid, int right, int number) {
    int* elem1, * elem2;
    elem1 = (int*)malloc((mid - left + 1) * sizeof(int));
    elem2 = (int*)malloc((right - mid) * sizeof(int));
    int i = 0, j = 0, k = 0;
    for (i = 0; i <= mid - left; i++) {
        elem1[i] = order[i + left];
    }
    for (j = 0; j <= right - mid - 1; j++) {
        elem2[j] = order[j + mid + 1];
    }
    for (i = j = 0, k = left; i <= mid - left && j <= right - mid - 1; k++) {
        int a, b;
        a = L->keys[number].key[i];
        b = L->keys[number].key[j];
        if (a <= b) {
            order[k] = elem1[i++];
        }
        else order[k] = elem2[j++];
    }
    if (i > mid - left) {
        for (j; j <= right - mid - 1; j++) {
            order[k] = elem2[j];
            k++;
        }
    }
    else if (j > right - mid - 1) {
        for (i; i <= mid - left; i++) {
            order[k] = elem1[i];
            k++;
        }
    }
    return OK;
}

int LSDMergeSort(SqList* L, int left, int right, int keynum) {
    int mid;
    if (left < right) {
        mid = (left + right) / 2;
        if (!LSDMergeSort(L, left, mid, keynum))
            return ERROR;
        if (!LSDMergeSort(L, mid + 1, right, keynum))
            return ERROR;
        if (!LSDmerge(L, left, mid, right, keynum))
            return ERROR;
    }
    return OK;
}

int LSDMerge(SqList* L) {
    int i = 0;

    for (int i = L->info.keynum - 2; i >= 1; i--) {
        if (!LSDMergeSort(L, 0, L->info.length - 1, i))
            return ERROR;
    }
    LSDMergeSort(L, 0, L->info.length - 1, L->info.keynum - 1);
    //printf("排序后的结果为：");
    //for (i = 0; i < L->info.length; i++) {
    //    printf("%d\n", order[i]);
    //}
    //t.end = clock();
    //t.duration = ((double)(t.end - t.start)) / CLOCKS_PER_SEC;
    //printf("LSD归并排序所用的时间为：%lfs\n", t.duration);
    return OK;
}
