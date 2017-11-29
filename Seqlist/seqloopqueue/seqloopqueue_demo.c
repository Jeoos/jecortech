#include <stdio.h>
#include <string.h>

#define datatype int

#define MAX_QUEUE_NUM 10

typedef struct seqloopqueue {
        int front, rear;
        datatype data[MAX_QUEUE_NUM];
} seqloopqueue_t;

int is_empty_seqloopqueue(seqloopqueue_t *sq)
{
        return sq->front == sq->rear ? 1:0;
}

int is_full_seqloopqueue(seqloopqueue_t *sq)
{
        return (sq->rear + 1) % (MAX_QUEUE_NUM+1) == sq->front ? 1:0;
}



int entry_seqloopqueue(seqloopqueue_t *sq, datatype x)
{
        if(is_full_seqloopqueue(sq)){
                printf("the seqqueue is full.\n");
                return -1;
        }
        sq->data[sq->rear] = x; //init front = rear = 0
        sq->rear = (sq->rear + 1)%(MAX_QUEUE_NUM+1);
        return 0;
}

datatype out_seqloopqueue(seqloopqueue_t *sq)
{
        datatype x;
        if(is_empty_seqloopqueue(sq)){
                printf("the seqqueue is empty.\n");
                return -1;
        }
        x = sq->data[sq->front];
        sq->front = (sq->front + 1)%(MAX_QUEUE_NUM+1);
        return x;
}

datatype get_queue_data(seqloopqueue_t *sq)
{
        if(is_empty_seqloopqueue(sq)){
                printf("the seqqueue is empty.\n");
                return ;
        }
        return sq->data[sq->front];

}

void print_seqloopquque(seqloopqueue_t *sq)
{
        int i; 
        if(is_empty_seqloopqueue(sq)){
                printf("the seqqueue is empty.\n");
                return ;
        }
        for(i=0; i<sq->rear-sq->front; i++) {
                printf("%d sq->front=%d  ", sq->front+i, sq->data[sq->front+i]);
        }
        printf("\n");
}

int main(int argc, char **argv)
{
        int i;
        datatype temp;
        seqloopqueue_t demo;
        memset(&demo, 0, sizeof(seqloopqueue_t));
        demo.front = 0;
        demo.rear = 0;
        for(i=0; i<MAX_QUEUE_NUM; i++){
                //printf("i=%d.\n", i);
                entry_seqloopqueue(&demo, i);
        }
        print_seqloopquque(&demo);

        out_seqloopqueue(&demo);
        print_seqloopquque(&demo);

        temp = get_queue_data(&demo);
        printf("the seqqueue data = %d\n", temp);


        return 0;
}
