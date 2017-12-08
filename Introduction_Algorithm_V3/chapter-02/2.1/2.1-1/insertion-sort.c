#include <stdio.h>

int A[6]={31, 41, 59, 26, 41, 58};

int main(int argc, char **argv)
{
        int i, j, key;

        for(j=1; j<6; j++){
                key = A[j];
                i = j - 1;
                while(i>=0 && A[i] > key){
                        A[i+1] = A[i];
                        i = i - 1;
                }
                A[i+1] = key;
        }
        for(j=0; j<6; j++){
                printf("A[%d] = %d\t", j, A[j]);
        }
        printf("\n");

        return 0;
}
