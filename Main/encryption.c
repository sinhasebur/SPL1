#include "encryption.h"
#include "string.h"
#include "des.h"
#include "2des.h"
#include "3des.h"
#include <stdio.h>

void encrypt(unsigned char* text, int* key, unsigned char* cipherText, unsigned char* encryptiontype)
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
            memcpy(key3, key+64, sizeof(int)*64);
            _3DES_Encrypt(text, key1,key2,key3, cipherText);
            break;
        }

    }
}



void decrypt(unsigned char* cipherText, int* key, unsigned char* resultText, unsigned char* encryptiontype)
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
            memcpy(key3, key+64, sizeof(int)*64);
            _3DES_Decrypt(cipherText, key1,key2,key3, resultText);
            break;
        }
    }
}



int getType(unsigned char* x){
    if(x[0]=='D'||x[0]=='d'){
        return 1;
    }
    else if(x[0]=='2'||x[0]=='t') return 2;
    else if(x[0]=='3') return 3;
    else if (x[0]=='a'||x[0]=='A') return 4;
    else {
        printf("INVALID ENCRYPTION TYPE \nDefaulting to DES\n");
        return 1;
    }
}
