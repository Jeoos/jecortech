#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define datatype int
#define MAX_STACK_NUM 20

typedef struct seqstack {
        int top;
        datatype data[MAX_STACK_NUM];
} seqstack_t;

int is_full_seqstack(seqstack_t *s)
{
        return s->top == MAX_STACK_NUM ? 1:0;
}

int is_empty_seqstack(seqstack_t *s)
{
        return s->top == -1 ? 1:0;
}

int push_seqstack(seqstack_t *s, datatype x)
{
        if(is_full_seqstack(s)){
                printf("the seqstack is full.\n");
                return -1;
        }
        s->data[s->top+1] = x;
        s->top++;
        return 0;
}

datatype pop_seqstack(seqstack_t *s)
{
        if(is_empty_seqstack(s)){
                printf("the seqstack is empty.\n");
                return -1;
        }
        datatype x;
        x = s->data[s->top];
        s->top--;
        return 0;
}

datatype get_top_data(seqstack_t *s)
{
        if(is_empty_seqstack(s)){
                printf("the seqstack is empty.\n");
                return -1;
        }
        return s->data[s->top-1];
}

void print_seqstack(seqstack_t *s)
{
        int i;
        for(i=0; i<s->top; i++){
                printf("%d %d  \n", i, s->data[i]);
        }
        printf("\n");
}

int main(int argc, char **argv)
{
        int i, temp;
        seqstack_t demo;
        demo.top = -1;
        for(i=0; i<MAX_STACK_NUM; i++){
                push_seqstack(&demo, i);
        }
        print_seqstack(&demo);

        pop_seqstack(&demo);
        print_seqstack(&demo);

        temp = get_top_data(&demo);
        printf("%d\n", temp);
        return 0;
}
