#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  

#define NUM 4
#define T_NUM 3
int log[16]={0};  
int alog[16]={0};  
  
void generate_Galois_table()  
{  
    alog[0]=1;  
    int i;  
    int alog_data;  
    for(i=1; i<15; i++)  
    {  
           alog_data=alog[i-1]*2;  
           if(alog_data & 16)alog_data^=19;  //0x11D
           alog[i]=alog_data;  
           log[alog[i]]=i;  
        }  
}  
  
int Galois_mutipile(int a,int b)  
{  
    if(a==0||b==0)return 0;  
    return alog[(log[a]^log[b])%15];//need % 15?  
}  
  
int Galois_division(int a,int b)  
{  
    if(a==b)return 0;  
    if(b==0)return a;  
    return alog[((log[a]^log[b])+15)%15];  
}  

int main()  
{  
        generate_Galois_table();  
        int i;  
        int j=0;  
        for(i=0; i<16; i++)  
        {  
                printf("%02x ",alog[i]);  
                j++;  
                if(j==16)  
                {  
                        printf("\n");  
                        j=0;  
                }  
        }  
        printf("\n\n\n");  
        j=0;  
        for(i=0; i<16; i++)  
        {  
                printf("%02x ",log[i]);  
                j++;  
                if(j==16)  
                {  
                        printf("\n");  
                        j=0;  
                }  
        }  
        printf("\n\n");  
        j=0;  
        for(i=0; i<32; i++)  
        //for(i=0; i<16; i++)  
        {  
                printf("%02d ",alog[i]);  
                j++;  
                if(j==16)  
                {  
                        printf("\n");  
                        j=0;  
                }  
        }  
        printf("\n\n");  
        j=0;  
        for(i=0; i<32; i++)  
        //for(i=0; i<16; i++)  
        {  
                printf("%02d ",log[i]);  
                j++;  
                if(j==16)  
                {  
                        printf("\n");  
                        j=0;  
                }  
        }  
        printf("\n\n");  
        //printf("%d\n",Galois_mutipile(129,5));  
        //printf("%d\n",Galois_division(97,5));  
        
        int D[NUM];  
        int K[NUM];  
        int P=0,Q=0;  
        srand(time(NULL));  
        for(i=0;i<NUM;i++)  
        {  
                //D[i]=i+2;  
                D[i]=rand()%16+1;
                printf("D[%d]=%d\n",i,D[i]);  
                K[i]=rand()%16+1;//1=<K[i]<=15  
                printf("K[%d]=%d\n\n",i,K[i]);  
                P^=D[i];  
                Q^=Galois_mutipile(D[i],K[i]);  
        }  
        // D1 p losing to zero
        printf("\noriginal data: P=%d Q=%d D[%d]=%d\n",P,Q, T_NUM, D[T_NUM]);  
        D[T_NUM]=0;P=0;
        printf("losing:\n\t P=%d D[%d]=%d\n", P, T_NUM, D[T_NUM]);
        for(i=0;i<NUM;i++)  
        {  
                if(i == T_NUM) continue;
                D[T_NUM]^=Galois_mutipile(D[i],K[i]);  
        }  
        D[T_NUM]^=Q;  
        D[T_NUM]=Galois_division(D[T_NUM],K[T_NUM]);  
        for(i=0;i<NUM;i++)  
        {  
                P^=D[i];  
        }  
        //recovery to
        printf("recovery:\n\t P=%d D[%d]=%d\n", P, T_NUM, D[T_NUM]);
        return 0;
}
