#include <stdio.h>

#define datatype int

typedef struct loopnode {
        datatype data;
        struct loopnode *next;
} loopnode;

typedef struct loopnode * looplist_t;

int is_empty_looplist(looplist_t h)
{
        return h->next == h ? 1:0;
}

int insert_head_looplist(looplist_t h, datatype x)
{
        looplist_t temp;
        temp = (looplist_t)malloc(sizeof(loopnode));
        temp->data = x;
        temp->next = h->next;

        h->next = temp;

        return 0;
}

datatype delete_head_looplist(looplist_t h)
{
        datatype x;
        looplist_t temp;
        if(is_empty_looplist(h)){
                printf("linklist is NULL.\n");
                return -1;
        }
        temp = h->next;
        h->next = temp->next;
        x = temp->data;
        free(temp);
        temp = NULL;

        return x;
}

int delete_all_looplist(looplist_t h)
{
        int i=0;
        looplist_t t = h;
        looplist_t temp;
        if(is_empty_looplist(h)){
                printf("linklist is NULL.\n");
                return -1;
        }
        //h = h->next;
        while(h->next != t){
                temp = h;
                h = h->next;
                temp->next = NULL;
                free(temp);
                i++;
        } 
        free(h);
        h = NULL;
        //printf("i = %d\n", i);
        return 0;
        
}

looplist_t cut_head_looplist(looplist_t h)
{
        looplist_t p = h;
        if(is_empty_looplist(h)){
                printf("linklist is NULL.\n");
                return -1;
        }
        while(h->next != p){
                h = h->next;
        }
        h->next = p->next;
        free(p);
        p = NULL;
        return h->next;
}

void print_looplist(looplist_t h, int new_head)
{
        int i = 0;
        looplist_t temp = h;
        if(is_empty_looplist(h) || h->next == NULL){
                printf("linklist is NULL or freed.\n");
                return -1;
        }

        if(new_head)
                printf("%d: h->data=%d\t", i, h->data);
                
        while(h->next != temp){
                printf("%d: h->next->data=%d\t", i, h->next->data);
                h = h->next;
                i++;
        }
        printf("\n");
}

int main(int argc, const char **argv)
{
        int i;
        looplist_t demo;
        looplist_t new_head;
        demo = (looplist_t)malloc(sizeof(loopnode));
        demo->next = demo;
        demo->data = 0;

        for(i=0; i<5; i++){
                insert_head_looplist(demo, i);
        }
        print_looplist(demo, 0);

        delete_head_looplist(demo);
        print_looplist(demo, 0);

        new_head = cut_head_looplist(demo);
        print_looplist(new_head, 1);

        delete_all_looplist(new_head);
        print_looplist(new_head, 1);

        return 0;
}
