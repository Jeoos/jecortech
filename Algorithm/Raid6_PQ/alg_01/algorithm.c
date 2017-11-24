#include <stdio.h>

#define MM 8
#define NN 255
#define alphaToMM 45 //α^8=α^5+α^3+α^2+1

int alphaTo[NN+1];
int expOf[NN+1];

void main()
{
        alphaTo[MM] = alphaToMM;
        expOf[alphaToMM] = MM;
        alphaTo[NN] = 0;
        expOf[0] = NN;

        int i, shift;
        shift = 1;
        for(i=0; i<MM; i++){
                alphaTo[i] = shift;//2^i
                expOf[alphaTo[i]] = i;
                shift <<= 1;
        }

        shift = 128;
        for(i=MM+1; i<NN; i++){
                if(alphaTo[i-1] >= shift){
                        alphaTo[i] = alphaTo[MM] ^ ((alphaTo[i-1]^shift)<<1);//alphaTo[i-1]*alpha+alpha^8

                }else{
                        alphaTo[i] = alphaTo[i-1]<<1;

                }
                expOf[alphaTo[i]] = i;

        }
        printf("\n");
        printf("alphaTo: \n");
        for(i=0; i<NN; i++){
                printf("[%d]", alphaTo[i]);
        }
        printf("\n");
        printf("\n");

        printf("expOf: \n");
        for(i=0; i<NN; i++){
                printf("[%d]", expOf[i]);
        }
        printf("\n");
        printf("\n");
}
