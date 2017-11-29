#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HASH_NUM 10

typedef struct _NODE {
    int data;  
    struct _NODE *next;  
} NODE;

typedef struct _HASH_TABLE {  
        NODE *value[MAX_HASH_NUM];  
} HASH_TABLE;  

HASH_TABLE* create_hash_table()  
{  
        HASH_TABLE* pHashTbl = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));  
        memset(pHashTbl, 0, sizeof(HASH_TABLE));  
        return pHashTbl;  
}

NODE* find_data_in_hash(HASH_TABLE* pHashTbl, int data)  
{  
        NODE* pNode;  
        if(NULL ==  pHashTbl)  
                return NULL;  

        if(NULL == (pNode = pHashTbl->value[data % 10]))  
                return NULL;  

        while(pNode){  
                if(data == pNode->data)  
                return pNode;  
                pNode = pNode->next;  
        }  
        return NULL;  
}

int insert_data_into_hash(HASH_TABLE* pHashTbl, int data)  
{
        NODE* pNode;
        if(NULL == pHashTbl){
                printf("err pHashTbl.\n");
                return -1;
        }

        if(NULL == pHashTbl->value[data % 10]){
                pNode = (NODE*)malloc(sizeof(NODE));
                memset(pNode, 0, sizeof(NODE));
                pNode->data = data;
                pHashTbl->value[data % 10] = pNode;
                return 0;
        }

        if(NULL != find_data_in_hash(pHashTbl, data))
                return -1;

        pNode = pHashTbl->value[data % 10];
        while(NULL != pNode->next)  
                pNode = pNode->next;

        pNode->next = (NODE*)malloc(sizeof(NODE));  
        memset(pNode->next, 0, sizeof(NODE));  
        pNode->next->data = data;
        return 0;
}

int delete_data_from_hash(HASH_TABLE* pHashTbl, int data)  
{  
        NODE* pHead;  
        NODE* pNode;  
        if(NULL == pHashTbl || NULL == pHashTbl->value[data % 10])  
                return -1;  

        if(NULL == (pNode = find_data_in_hash(pHashTbl, data)))  
                return -1;  

        if(pNode == pHashTbl->value[data % 10]){  
                pHashTbl->value[data % 10] = pNode->next;  
                goto final;  
        }  

        pHead = pHashTbl->value[data % 10];  
        while(pNode != pHead ->next)  
        pHead = pHead->next;
        pHead->next = pNode->next;  

final:
        free(pNode);  
        return 0;  
}

void print_data_from_hash(HASH_TABLE *pHashTbl)
{
        int i;
        for(i=0; i<MAX_HASH_NUM; i++){
                if(!pHashTbl->value[i])
                        continue;
                printf("pHashTbl->value[%d]->data = %d  ", i, pHashTbl->value[i]->data);
        }
        printf("\n");
}

int main(int argc, char **argv)
{
        int i;
        HASH_TABLE *demo;
        demo = create_hash_table();
        for(i=0; i<MAX_HASH_NUM-2; i++){
                insert_data_into_hash(demo, i);
        }
        print_data_from_hash(demo);

        insert_data_into_hash(demo, 9);
        print_data_from_hash(demo);

        insert_data_into_hash(demo, 20);
        print_data_from_hash(demo);

        return 0;
}
