#include <stdio.h>
#include <string.h>

#define datatype int

#define MAX_QUEUE_NUM 20

typedef struct seqqueue {
        int front, rear;
        datatype data[MAX_QUEUE_NUM];
} seqqueue_t;

int is_empty_seqqueue(seqqueue_t *sq)
{
        return sq->front == sq->rear ? 1:0;
}

int is_full_seqqueue(seqqueue_t *sq)
{
        return (sq->rear - sq->front) == MAX_QUEUE_NUM ? 1:0;
}



int entry_seqqueue(seqqueue_t *sq, datatype x)
{
        if(is_full_seqqueue(sq)){
                printf("the seqqueue is full.\n");
                return -1;
        }
        sq->data[sq->rear + 1] = x; //init front = rear = -1
        sq->rear++;
}

datatype out_seqqueue(seqqueue_t *sq)
{
        datatype x;
        if(is_empty_seqqueue(sq)){
                printf("the seqqueue is empty.\n");
                return -1;
        }
        x = sq->data[++sq->front];
        return x;
}

datatype get_queue_data(seqqueue_t *sq)
{
        if(is_empty_seqqueue(sq)){
                printf("the seqqueue is empty.\n");
                return ;
        }
        return sq->data[sq->front+1];

}

void print_seqquque(seqqueue_t *sq)
{
        int i; 
        if(is_empty_seqqueue(sq)){
                printf("the seqqueue is empty.\n");
                return ;
        }
        for(i=1; i<=sq->rear-sq->front; i++) {
                printf("%d sq->front=%d  ", sq->front+i, sq->data[sq->front+i]);
        }
        printf("\n");
}

int main(int argc, char **argv)
{
        int i;
        datatype temp;
        seqqueue_t demo;
        memset(&demo, 0, sizeof(seqqueue_t));
        demo.front = -1;
        demo.rear = -1;
        for(i=0; i<MAX_QUEUE_NUM; i++){
                entry_seqqueue(&demo, i);
        }
        print_seqquque(&demo);

        out_seqqueue(&demo);
        print_seqquque(&demo);

        temp = get_queue_data(&demo);
        printf("the seqqueue data = %d\n", temp);


        return 0;
}
