#include "stdio.h"
#include "stdlib.h"

#define HASH_TABLE_MAX 3	//Hash表长度
#define REFRESH 10			//大更新周期
#define PROCESS_SHCEDULING_MAX 12	//进程调度序列个数
#define TTL_INIT 3			//页面初始生存周期

typedef struct Page_Table{
    int ProcessNum;     //进程号
    int TTL;            //生存周期，其数值等于结束时刻
    int tempTTL;        //缓冲变量
    Page_Table *next;
}Page_Table;

void ListInit();    //所有单链表初始化
Page_Table *HashCalculate(int aProcessNum); //hash值计算
int PageTableIsHit(Page_Table *head, int aProcessNum);  //页面是否命中
void LishInsert(Page_Table *head, int aProcessNum);     //页面未命中时调入
void Update(Page_Table **Hash_Table);   //大更新
void OutPutPageTable(Page_Table **Hash_Table);  //输出

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

/**
 * 初始化Hash表，Hash表为指针数组，使所有指针指向NULL
 * @return
 *          
 */
void ListInit(){
    for(int i = 0; i < HASH_TABLE_MAX; i++){
        Hash_Table[i] = (Page_Table*)malloc(sizeof(Page_Table));
        Hash_Table[i]->next = NULL;
    }
}

/**
 * 计算Hash值
 * @param aProcessNum
 *          当前进程号
 * @return
 *          Hash值，即对应的单链表头指针
 */
Page_Table *HashCalculate(int aProcessNum){
    int HashValue = aProcessNum % HASH_TABLE_MAX;
    return Hash_Table[HashValue];   //返回对应的单链表头指针
}

/**
 * 计算Hash值
 * @param *head
 *          指针数组中的一个元素，某个单链表头指针
 * @param aProcessNum
 *          当前进程号
 * @return
 *          0：命中且页面未过期，1：页面命中但已过期，-1：出错
 */
int PageTableIsHit(Page_Table *head, int aProcessNum){
    Page_Table *node = NULL;
    while(head->next){//循环判断是否命中
        node = head->next;
        head = node;
        if(node->ProcessNum == aProcessNum && node->TTL > T)
            //若命中且页面未过期
            return 0;
        else if(node->ProcessNum == aProcessNum && node->TTL <= T){
            //若命中但页面已过期，缓冲变量值++
            node->tempTTL++;
            return 1;
        }
        else ;
    }
    return -1;
}

/**
 * 将当前进程插入到页表中
 * @param *head
 *          指针数组中的一个元素，某个单链表头指针
 * @param aProcessNum
 *          当前进程号
 * @return
 *          
 */
void LishInsert(Page_Table *head, int aProcessNum){
    //创建新结点
    Page_Table *node = (Page_Table*)malloc(sizeof(Page_Table));
    node->TTL = TTL_INIT + T;   //TTL_INIT为初始生存周期，T为当前时刻
    node->tempTTL = 0;          //缓冲变量值初始为0
    node->ProcessNum = aProcessNum;
    node->next = head->next;
    head->next = node;
}

/**
 * 更新页表
 * @param **Hash_Table
 *          页表
 * @return
 *          
 */
void Update(Page_Table **Hash_Table){
    Page_Table *node = NULL;
    Page_Table *head = NULL;
    for(int i = 0; i < HASH_TABLE_MAX; i++){
        head = *(Hash_Table + i);
        node = head->next;
        while(node){
            if(node->TTL >= T){
                //页面未过期，避免大数运算，将TTL和T减小
                node->TTL -= T;
                node->tempTTL = 0;
                head = node;
            }
            else{
                if(node->tempTTL){
                    //页面过期但缓冲变量值>0，对其TTL重新赋值
                    node->TTL = TTL_INIT + node->tempTTL;
                    node->tempTTL = 0;
                    head = node;
                }
                else{
                    //页面过期且缓冲变量值为0，删除结点
                    head->next = node->next;
                    free(node);
                }
            }
            node = head->next;
        }
    }
    T = 0;
}

/**
 * 打印页表
 * @param **Hash_Table
 *          页表
 * @return
 *          
 */
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
