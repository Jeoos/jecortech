#include <stdio.h>

#define datatype int
#define MAX_SEQ_NUM 20

typedef struct seqlist {
        int last;
        datatype data[MAX_SEQ_NUM];
} seqlist_t;

int is_full_seqlist(seqlist_t *l)
{
        return l->last == MAX_SEQ_NUM ? 1:0;
}

int is_empty_seqlist(seqlist_t *l)
{
        return l->last == 0 ? 1:0;
}

int insert_pos_seqlist(seqlist_t *l, int pos, datatype x)
{
        int i=0;
        if(is_full_seqlist(l)){
                printf("seqlist if full.\n");
                return -1;
        }
        if(pos < 0 || pos > l->last+1){
                printf("err insert pos.\n");
                return -1;
        }
        for(i=l->last; i>=pos; i--){
                l->data[i+1] = l->data[i];
        }
        l->data[pos] = x;

        return 0;
}

datatype delet_pos_seqlist(seqlist_t *l, int pos)
{
        int i=0;
        datatype x;
        if(is_empty_seqlist(l)){
                printf("seqlist if empty.\n");
                return -1;
        }
        if(pos < 0 || pos > l->last){
                printf("err delete pos.\n");
                return -1;
        }
        x = l->data[pos];
        for(i=pos; i<l->last; i++){
                l->data[i] = l->data[i+1];
        }
        l->last--;

        return x;
}

int print_seqlist(seqlist_t *l)
{
        int i;
        if(is_empty_seqlist(l)){
                printf("seqlist if empty.\n");
                return -1;
        }
        for(i=0; i<l->last; i++){
                printf("l->data[%d]=%d\t", i, l->data[i]);
        }
        printf("\n");

        return 0;
}

int main(int argc, const char *argv[])
{
        int i;
        seqlist_t demo;
        for(i=0; i<15; i++){
                demo.last = i;
                demo.data[i] = i;
        }
        print_seqlist(&demo);

        insert_pos_seqlist(&demo, 5, 100);
        print_seqlist(&demo);

        delet_pos_seqlist(&demo, 2);
        print_seqlist(&demo);

        return 0; 
}
