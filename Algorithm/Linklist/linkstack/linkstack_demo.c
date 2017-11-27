#include <stdio.h>
#include <stdlib.h>

#define datatype int

typedef struct stackNode {
        datatype data; 
        struct stackNode *next;
} stackNode;

typedef stackNode *linkstack_t;

int is_empty_linkstack(linkstack_t s)
{
        return s->next == NULL?1:0;
}

int push_linkstack(linkstack_t s, datatype x)
{
        linkstack_t temp = (linkstack_t)malloc(sizeof(stackNode));
        temp->data = x;
        temp->next = s->next;
        s->next = temp;

        return 0;
}

datatype pop_linkstack(linkstack_t s)
{
        datatype x;
        linkstack_t temp;
        if(is_empty_linkstack(s)){
                printf("the linkstack is empty.\n");
                return -1;
        }

        temp = s->next;
        s->next = temp->next;

        x = temp->data;
        free(temp);
        temp = NULL;
        return x;
}

void free_all_stacknode(linkstack_t s)
{
        linkstack_t temp = s;
        if(is_empty_linkstack(s)){
                printf("the linkstack is empty.\n");
                return ;
        }

        while(s->next != NULL){
                temp = s;
                s = s->next;
                temp->next = NULL;
                free(temp);
        }
        free(s);
}

void print_linkstack(linkstack_t s)
{
        int i=0;
        while(s->next != NULL){
                s = s->next;
                printf("%d [data = %d]  ", i, s->data);
                i++;
        }
        printf("\n");
}

int main(int argc, char **argv)
{
        int i;
        linkstack_t demo;
        demo = (linkstack_t)malloc(sizeof(stackNode));
        for(i=0; i<20; i++){
                push_linkstack(demo, i);
        }
        print_linkstack(demo);
        pop_linkstack(demo);
        print_linkstack(demo);
        free_all_stacknode(demo);
        print_linkstack(demo);
        return 0;
}
