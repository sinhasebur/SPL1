#include <stdio.h>
#include <string.h>
#include "des.h"

#define endl printf("\n");
void ecb_encrypy_noPadding(char* text, int blockSize, int blockNum, int* key);
void ecb_decrypy_noPadding(char* text, int blockSize, int blockNum, int* key);


int main(){
    char a[16]={'a','b','c','d','e','f','g','h','1','2','3','4','5','6','7','8'};
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};

    for(int i=0;i<16;i++) printf("%02X ",a[i]);
    endl

    ecb_encrypy_noPadding(a,64,2,key);

    for(int i=0;i<16;i++) printf("%02X ",a[i]);
    endl

    ecb_decrypy_noPadding(a,64,2,key);

    for(int i=0;i<16;i++) printf("%02X",a[i]);
    endl

    for(int i=0;i<16;i++) printf("%c",a[i]);

    endl
}


void ecb_encrypy_noPadding(char* text, int blockSize, int blockNum, int* key){
    int bytes=blockSize/8;
    for(int i=0;i<blockNum;i++){
        encrypt(text,key);
        text=text+bytes;
    }
}


void ecb_decrypy_noPadding(char* text, int blockSize, int blockNum, int* key){
    int bytes=blockSize/8;
    for(int i=0;i<blockNum;i++){
        decrypt(text,key);
        text=text+bytes;
    }
}

