#include "encryption.h"
#include "string.h"
#include "des.h"
#include "2des.h"
#include "3des.h"
#include "aes.h"
#include <stdio.h>
#include <stdlib.h>


void encrypt(unsigned char* text, int* key, unsigned char* cipherText, char* encryptiontype)
{
    int type= getType(encryptiontype);

    switch(type){
        case 1:// des
        {
            DES_Encrypt(text, key, cipherText);
            break;
        }
        case 2:
        {
            int key1[64],key2[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            _2DES_Encrypt(text, key1,key2, cipherText);
            break;
        }
        case 3:
        {
            int key1[64],key2[64],key3[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            memcpy(key3, key+128, sizeof(int)*64);
            _3DES_Encrypt(text, key1,key2,key3, cipherText);
            break;
        }
        case 4:{
        
            unsigned char AESkey[16];
            int k=0;
            for(int i=0;i<16;i++){
                unsigned char u=0;
                for(int j=0;j<8;j++){
                    if(key[k]==1){
                        u|=(1LL<<(7-j));
                    }
                    k++;
                }
                AESkey[i]=u;
            }
            
            AESencrypt(text,cipherText, AESkey,10);
            break;
        }

    }
}



void decrypt(unsigned char* cipherText, int* key, unsigned char* resultText,  char* encryptiontype)
{
    int type=getType(encryptiontype);

    switch(type){
        case 1:
            DES_Decrypt(cipherText, key, resultText);
            break;
        case 2:
        {
            int key1[64], key2[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            _2DES_Decrypt(cipherText, key1, key2, resultText);
            break;
        }
        case 3:
        {
            int key1[64],key2[64],key3[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            memcpy(key3, key+128, sizeof(int)*64);
            _3DES_Decrypt(cipherText, key1,key2,key3, resultText);
            break;
        }
        case 4:{
            unsigned char AESkey[16];
            int k=0;
            for(int i=0;i<16;i++){
                unsigned char u=0;
                for(int j=0;j<8;j++){
                    if(key[k]==1){
                        u|=(1LL<<(7-j));
                    }
                    k++;
                }
                AESkey[i]=u;
            }
            
            AESdecrypt(cipherText,resultText,AESkey,10);
            break;
        }

    }
}



int getType( char* x){
    if(x[0]=='D'||x[0]=='d'){
        return 1;
    }
    else if(x[0]=='2'||x[0]=='t') return 2;
    else if(x[0]=='3') return 3;
    else if (x[0]=='a'||x[0]=='A') return 4;
    else {
        printf("INVALID ENCRYPTION TYPE \n\n");
        exit(1);
        //return 0;
    }
}


char * encNumber(int i){
    if(i==1) return "des";
    if(i==2) return "2des";
    if(i==3) return "3des";
    if(i==4) return "aes";

}

int getBlockSize(int i){
    if(i==4) return 128;
    else return 64;
}