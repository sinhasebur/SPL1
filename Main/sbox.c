#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "sbox.h"

unsigned char multiplyGF(unsigned char a, unsigned char b);
int getInverseGF(unsigned char byte);
unsigned char MultiplywithA(unsigned char b);

void GenerateSBOX(){
    
    unsigned char sBox[256]={0};
    
    unsigned char affineTransformation=0x63;
    for(int byte=0;byte<256;byte++){
       unsigned char modularInverse=getInverseGF(byte);
       sBox[byte]=MultiplywithA(modularInverse) ^affineTransformation;
    }

    int k=0;
    printf("S box\n");
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            printf("%02x ",sBox[k++]);
        }
        printf("\n");
    }

    unsigned char inverseSBox[256];
    for (int i=0;i<256;i++) {
        inverseSBox[sBox[i]] = i;
    }

    printf("\nInverse S box\n");
    
    k=0;
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            printf("%02x ",inverseSBox[k++]);
        }
        printf("\n");
    }


}

int getInverseGF(unsigned char byte){
    if(byte==0){
        return 0;
    }

    unsigned char ans=1;
    for(int i=0;i<254;i++){//b^(-1)=b^(254)
        ans=multiplyGF(ans,byte);
    }
    return ans;
}

unsigned char multiplyGF(unsigned char a, unsigned char b){
    
    unsigned char ans=0;
    for (int i=0;i<8;i++) {
        
        if(b & 1) ans ^= a;  
        long long limit= (1<<7);
        bool overflows=a&limit;
        a <<= 1;
        long long irreduciblePolynomial= (1<<8)+(1<<4)+(1<<3)+(1<<1)+1; //x^8 + x^4 + x^3 + x + 1
        if (overflows) a= a^irreduciblePolynomial; 
        b >>= 1; 
    }
    return ans;
}



unsigned char MultiplywithA(unsigned char b){
    int matrix1[8][8]= {
                        {1,0,0,0,1,1,1,1},
                        {1,1,0,0,0,1,1,1},
                        {1,1,1,0,0,0,1,1},
                        {1,1,1,1,0,0,0,1},
                        {1,1,1,1,1,0,0,0},
                        {0,1,1,1,1,1,0,0},
                        {0,0,1,1,1,1,1,0},                                
                        {0,0,0,1,1,1,1,1}
                    };

    int matrix2[8][1]={0};

    for(int i=0;i<8;i++){
        matrix2[i][0]=(b%2);
        b=b/2;
    }

    int ans[8][1]={0};
    

    for(int i=0;i<8;i++){
        for(int j=0;j<1;j++){
            for(int k=0;k<8;k++){
               ans[i][j] ^= multiplyGF(matrix1[i][k],matrix2[k][j]) ; 
            }
            
        }
    }

    unsigned char answer=0;
    for(int i=0;i<8;i++){
       // printf("%d",ans[0][7-i]);
        if(ans[i][0]){
            answer |= (1<<i); 
        }

    }
    // printf("\n");

    return answer;
}

