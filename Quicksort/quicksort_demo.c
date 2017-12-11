#include <stdio.h>

void sort(int *a, int left, int right)
{
        int i = left;
        int j = right;
        int key = a[left];
        if(left >= right) return;

        while(i < j){
                while(i < j && key <= a[j]){
                        printf("a[%d] = %d\t", j, a[j]);
                        j--;
                }
                printf("\n");
                a[i] = a[j];
                while(i < j && key >= a[i]){
                        printf("a[%d] = %d\t", i, a[i]);
                        i++;
                }
                printf("\n");
                a[j] = a[i];
        }
        a[i] = key;
        sort(a, left, i-1);
        sort(a, i+1, right);
}

int main(int argc, char **argv)
{
        int i;
        int a[8]={20, 58, 43, 44, 57, 9, 34, 19};
        for(i=0; i<8; i++){
                printf("a[%d] = %d\t", i, a[i]);
        }
        printf("\n");

        sort(a, 0, 7);
        for(i=0; i<8; i++){
                printf("a[%d] = %d\t", i, a[i]);
        }
        printf("\n");
        
        return 0;
}
