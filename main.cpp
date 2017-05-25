#include "stdio.h"
#include "stdlib.h"

#define HASH_TABLE_MAX 5
#define REFRESH 10
#define PROCESS_SHCEDULING_MAX 12
#define TTL_INIT 3

typedef struct Page_Table{
    int ProcessNum;
    int TTL;
    int tempTTL;
    Page_Table *next;
}Page_Table;

void ListInit();
Page_Table *HashCalculate(int aProcessNum);
int PageTableIsHit(Page_Table *head, int aProcessNum);
void LishInsert(Page_Table *head, int aProcessNum);
void Update(Page_Table **Hash_Table);
void OutPutPageTable(Page_Table **Hash_Table);

Page_Table *Hash_Table[HASH_TABLE_MAX];
int T = 0;  //时间线


int main(){
    Page_Table *head = NULL;
    ListInit();
    //进程调度序列
    int process_scheduling[PROCESS_SHCEDULING_MAX] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    for(int i = 0; i < PROCESS_SHCEDULING_MAX; i++){
        if(0 == T % REFRESH)
            Update(Hash_Table);
            ;
        head = HashCalculate(process_scheduling[i]);
        if(0 <= PageTableIsHit(head, process_scheduling[i]));
        else{
            //未命中，调入所需虚页并调整页表
            LishInsert(head, process_scheduling[i]);
        }
        T++;
    }
    OutPutPageTable(Hash_Table);
    printf("T = %d\n", T);
    return 0;
}

void ListInit(){
    for(int i = 0; i < HASH_TABLE_MAX; i++){
        Hash_Table[i] = (Page_Table*)malloc(sizeof(Page_Table));
        Hash_Table[i]->next = NULL;
    }
}

Page_Table *HashCalculate(int aProcessNum){
    int HashValue = aProcessNum % HASH_TABLE_MAX;
    return Hash_Table[HashValue];
}

int PageTableIsHit(Page_Table *head, int aProcessNum){
    Page_Table *node = NULL;
    while(head->next){
        node = head->next;
        head = node;
        if(node->ProcessNum == aProcessNum && node->TTL > T)
            return 0;
        else if(node->ProcessNum == aProcessNum && node->TTL <= T){
            node->tempTTL++;
            return 1;
        }
        else ;
    }
    return -1;
}

void LishInsert(Page_Table *head, int aProcessNum){
    Page_Table *node = (Page_Table*)malloc(sizeof(Page_Table));
    node->TTL = TTL_INIT + T;
    node->tempTTL = 0;
    node->ProcessNum = aProcessNum;
    node->next = head->next;
    head->next = node;
}

void Update(Page_Table **Hash_Table){
    Page_Table *node = NULL;
    Page_Table *head = NULL;
    for(int i = 0; i < HASH_TABLE_MAX; i++){
        head = *(Hash_Table + i);
        node = head->next;
        while(node){
            if(node->TTL >= T){
                node->TTL -= T;
                node->tempTTL = 0;
                head = node;
            }
            else{
                if(node->tempTTL){
                    node->TTL = TTL_INIT + node->tempTTL;
                    node->tempTTL = 0;
                    head = node;
                }
                else{
                    head->next = node->next;
                    free(node);
                }
            }
            node = head->next;
        }
    }
    T = 0;
}

void OutPutPageTable(Page_Table **Hash_Table){
    Page_Table *node = NULL;
    Page_Table *head = NULL;
    printf("===========================\n");
    for(int i = 0; i < HASH_TABLE_MAX; i++){
        printf("%d  -->", i);
        head = *(Hash_Table + i);
        node = head->next;
        while(node){
            printf("  %d|%d|%d  -->", node->ProcessNum, node->TTL, node->tempTTL);
            node = node->next;
        }
        printf("  NULL\n");
    }
}
