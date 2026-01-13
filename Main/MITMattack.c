#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"
#include "2des.h"
#include "modes.h"


#define endl printf("\n");
void toCharArray(int charArraySize, int* array, unsigned char* charArray);

int main(){
    
    int key2[128]={1 ,1 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 0 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,1 ,1 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };
    unsigned char k1[8];
    unsigned char k2[8];
    toCharArray(8, key2, k1);
    toCharArray(8, key2+64, k2);

    //unsigned char iv[8]={'a','b','c','d','e','f','g','h'};

    FILE *f  = fopen("input", "rb");

    if(!f){
        printf("Input file not found"); endl
        exit(1);
    }
    unsigned char knownBlock[64];

    for(int i=0;i<8;i++){
        char x= fgetc(f);
        knownBlock[i]=x;
    }
    fseek(f, 0, SEEK_SET);
    fclose(f);

    unsigned char x[8];
    DES_Encrypt(knownBlock, key2, x);

    ebc_encrypt(64,key2, "input", "2des");


    // can use knownBlock and cipherText
    // dummy intermediary value x for now
    // attack start    

    unsigned char k2[8]= { 0, 0, 0, 0, 0, 0, 0, 0};

    // HOW DO I TRY 2^64 COMBINATIONS? my des too a int key[64] as key
    //while()...



}


void toCharArray(int size, int* x, unsigned  char* y){

    int k=0;
    for(int i=0;i<size;i++){
        int temp=0;
        for(int j=0;j<8;j++,k++){

            if(x[k]==1)
                temp+= (1)<<j;
        }
        y[i]=(unsigned char)temp;
    }
}
