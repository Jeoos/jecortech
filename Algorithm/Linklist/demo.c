#include <stdio.h>

#define datatype int

typedef struct linknode {
        datatype data;
        struct linknode *next;
} linknode;

typedef struct linknode * linklist_t;

int is_empty_linklist(linklist_t h)
{
        return h->next == NULL ? 1:0;
}

int insert_head_linklist(linklist_t h, datatype x)
{
        linklist_t temp;
        temp = (linklist_t)malloc(sizeof(linknode));
        temp->data = x;
        temp->next = h->next;

        h->next = temp;

        return 0;
}

int insert_tail_linklist(linklist_t h, datatype x)
{
        linklist_t temp;
        temp = (linklist_t)malloc(sizeof(linknode));
        temp->data = x;
        temp->next = NULL;

        while(h->next != NULL){
                h = h->next;
        }
        h->next = temp;

        return 0;
}

int insert_sort_linklist(linklist_t h, datatype x)
{
        linklist_t temp;
        temp = (linklist_t)malloc(sizeof(linknode));
        temp->data = x;
        temp->next = NULL;
        
        while(h->next != NULL && temp->data >= h->next->data){
                h = h->next;
        }
        temp->next = h->next;
        h->next = temp;

        return 0;
}

datatype delete_head_linklist(linklist_t h)
{
        datatype x;
        linklist_t temp;
        if(is_empty_linklist(h)){
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

datatype delete_tail_linklist(linklist_t h)
{
        datatype x;
        linklist_t temp;
        if(is_empty_linklist(h)){
                printf("linklist is NULL.\n");
                return -1;
        }
        while(h->next != NULL)
        {
                temp = h;
                h = h->next;
        }
        x = h->data;
        free(h);
        temp->next = NULL;
        h = NULL;

        return x;
}

int delete_data_linklist(linklist_t h, datatype x)
{
        linklist_t temp;
        if(is_empty_linklist(h)){
                printf("linklist is NULL.\n");
                return -1;
        }
        while(h->next != NULL && h->next->data != x){
                h = h->next;
        }
        if(h->next == NULL){
                printf("the linklist is no data: %d\n", x);
                return -1;
        }else{
                temp = h->next;
                h->next = temp->next;
                free(temp);
                temp = NULL;
        }

        return 0;
}

int delete_all_linklist(linklist_t *h)
{
        int i=0;
        linklist_t temp1 = *h;
        linklist_t temp2;
        if(is_empty_linklist(temp1)){
                printf("linklist is NULL.\n");
                return -1;
        }
        //temp1 = temp1->next;
        while(temp1->next != NULL){
                temp2 = temp1;
                temp1  = temp1->next;
                temp2->next = NULL;
                free(temp2);
                //temp2 = NULL;
                i++;
        } 
        free(temp1);
        temp1 = NULL;
        //printf("i = %d\n", i);
        return 0;
        
}

int __delete_all_linklist(linklist_t h)
{
        int i=0;
        linklist_t temp;
        if(is_empty_linklist(h)){
                printf("linklist is NULL.\n");
                return -1;
        }
        //h = h->next;
        while(h->next != NULL){
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

void print_linklist(linklist_t h)
{
        int i = 0;
        if(is_empty_linklist(h)){
                printf("linklist is NULL.\n");
                return -1;
        }
        while(h->next != NULL){
                printf("%d: h->next->data=%d\t", i, h->next->data);
                h = h->next;
                i++;
        }
        printf("\n");
}

int main(int argc, const char **argv)
{
        int i = 0;
        linklist_t demo;
        demo = (linklist_t)malloc(sizeof(linknode));
        demo->next = NULL;
        demo->data = 0;

        for(i=0; i<5; i++){
                insert_head_linklist(demo, i);
        }
        print_linklist(demo);

        for(i=0; i<5; i++){
                insert_tail_linklist(demo, i);
        }
        print_linklist(demo);

        insert_sort_linklist(demo, 3);
        print_linklist(demo);

        delete_head_linklist(demo);
        print_linklist(demo);
        
        delete_tail_linklist(demo);
        print_linklist(demo);

        delete_data_linklist(demo, 2);
        print_linklist(demo);

        //delete_all_linklist(&demo);
        __delete_all_linklist(demo);
        print_linklist(demo);

        return 0;
}
