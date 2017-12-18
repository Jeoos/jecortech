#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct result {
        int A[16];
        int *ret;
};

int *find_max_crossing_subarray(struct result *A, int low, int mid, int high)
{
        int i, j;
        int sum=0, left_sum=0, right_sum=0;
        int max_left, max_right;

        for(i=mid; i>=low; i--){
                sum = sum + A->A[i];
                if(sum > left_sum){
                        left_sum = sum;
                        max_left = i;
                }
        }
        sum = 0;
        for(j=mid+1; j<=high; j++){
                sum = sum + A->A[j];
                if(sum > right_sum){
                        right_sum = sum;
                        max_right = j;
                }
        }
        A->ret[0] = max_left;
        A->ret[1] = max_right;
        A->ret[2] = left_sum + right_sum;
        return A->ret;
}

void fill_in(struct result *A, int *b, int *c, int *d, int flag)
{
        if (0 == flag){
                *b = A->ret[0];
                *c = A->ret[1];
                *d = A->ret[2];
                return ;
        }
        A->ret[0] = *b;
        A->ret[1] = *c;
        A->ret[2] = *d;
        return ;
}

int *find_maximun_subarray(struct result *A, int low, int high)
{
        int mid;
        int left_low, left_high, left_sum;
        int right_low, right_high, right_sum;
        int cross_low, cross_high, cross_sum;

        if(high == low){
                A->ret[0] = low;
                A->ret[1] = high;
                A->ret[2] = A->A[low];
                return A->ret;
        }
        mid = (low + high) / 2;
        find_maximun_subarray(A, low, mid);
        fill_in(A, &left_low, &left_high, &left_sum, 0);

        find_maximun_subarray(A, mid+1, high);
        fill_in(A, &right_low, &right_high, &right_sum, 0);

        find_max_crossing_subarray(A, low, mid, high);
        fill_in(A, &cross_low, &cross_high, &cross_sum, 0);

        if(left_sum >= right_sum && left_sum >= cross_sum){
                fill_in(A, &left_low, &left_high, &left_sum, 1);
        }else if(right_sum >= left_sum && right_sum >= cross_sum){
                fill_in(A, &right_low, &right_high, &right_sum, 1);
        }else{
                fill_in(A, &cross_low, &cross_high, &cross_sum, 1);
        }
        return A->ret;
}

int main(int argc, char **argv)
{
        int i, *sub;
        int a[16] = {-2, 20, -7, 3, 18, 24, -22, -10, -2, 20, -7, 3, -18, 24, -22, 21};
        struct result *demo;
        demo = (struct result *)malloc(sizeof(struct result));
        demo->ret = (int *)malloc(3*sizeof(int));

        memcpy(demo->A, a, 16*sizeof(int));
        for(i=0; i<16; i++){
                printf("A[%d]=%d\t", i, demo->A[i]);
        }
        printf("\n");

        sub = find_maximun_subarray(demo, 0, 7);
        printf("sub[0] = %d\n", sub[0]);
        printf("sub[1] = %d\n", sub[1]);
        printf("sub[2] = %d\n", sub[2]);

        free(demo->ret);
        free(demo);

        return 0;
}
