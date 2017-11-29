#include <stdio.h>
#include <stdlib.h>

#define datatype int

struct queueNode {
        datatype data;
        struct queueNode *next;
};

typedef struct {
        struct queueNode *front;
        struct queueNode *rear;
} linkqueue_t;

int is_empty_linkqueue(linkqueue_t *lq)
{
        return lq->front == lq->rear ? 1:0;
}

int entry_linkqueue(linkqueue_t *lq, datatype x)
{
        struct queueNode *temp;
        temp = (struct queueNode *)malloc(sizeof(struct queueNode));
        temp->data = x;
        temp->next = NULL;

        lq->rear->next = temp;
        lq->rear = temp;
        return 0;
}

datatype out_linkqueue(linkqueue_t *lq)
{
        datatype x;
        struct queueNode *temp;
        if(is_empty_linkqueue(lq)){
                printf("the linkqueue is empty.\n");
                return -1;
        }
        x = lq->front->data;
        temp = lq->front;
        lq->front = temp->next;
        free(temp);
        temp = NULL;

        return x;
}

datatype __out_linkqueue(linkqueue_t *lq)
{
        struct queueNode *temp;
        datatype x;
        if(is_empty_linkqueue(lq)){
                printf("the linkqueue is empty.\n");
                return -1;
        }
        temp = lq->front->next;
        lq->front->next = temp->next;
        if(lq->front->next == NULL){
                lq->rear = lq->front;
        }
        x = temp->data;
        free(temp);
        temp = NULL;
        return x;
}

void free_all_queuenode(linkqueue_t *lq)
{
        struct queueNode *tmp = lq->front;
        struct queueNode *tmp1;

        while(tmp->next != NULL){
                tmp1 = tmp;
                tmp = tmp->next;
                tmp1->next = NULL;
                free(tmp1);
        }
        free(tmp);
        tmp = NULL;
}

void print_linkqueue(linkqueue_t *lq)
{
        int i;
        struct queueNode *tmp = lq->front;
        if(is_empty_linkqueue(lq)){
                printf("the linkqueue is empty.\n");
                return;
        }
        while(tmp->next != NULL){
                printf("%d [data=%d]  ", i, tmp->next->data);
                tmp = tmp->next;
                i++;
        }
        printf("\n");
}


int main(int agrc, char **argv)
{
        int i;
        datatype tmp;
        linkqueue_t demo;
        demo.front = (struct queueNode *)malloc(sizeof(linkqueue_t));
        demo.rear = demo.front;
        demo.rear->next = NULL;
        for(i=0; i<20; i++){
                entry_linkqueue(&demo, i);
        }
        print_linkqueue(&demo);
        printf("\n");

        __out_linkqueue(&demo);
        __out_linkqueue(&demo);
        tmp = __out_linkqueue(&demo);
        print_linkqueue(&demo);
        printf("tmp = %d\n\n", tmp);

        out_linkqueue(&demo);
        out_linkqueue(&demo);
        tmp = out_linkqueue(&demo);
        print_linkqueue(&demo);
        printf("tmp = %d\n\n", tmp);

        free_all_queuenode(&demo);

        //free(demo.front);
        demo.front = NULL;
        demo.rear = NULL;
        print_linkqueue(&demo);

        return 0;
}
