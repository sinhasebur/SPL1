#include "3des.h"
#include "des.h"

void _3DES_Encrypt(unsigned char* text, int* key1, int* key2,int* key3, unsigned char* cipherText)
{
    static int keysM1[16][48];
    static int keysM2[16][48];
    static int keysM3[16][48];
    static int des3StateEn = 0;
    unsigned char cipher1[8], cipher[8];

    if(des3StateEn==0) {
        getKeys(key1, keysM1[0]);
        getKeys(key2, keysM2[0]);
        getKeys(key3, keysM3[0]);
        des3StateEn= 1;
    }
    
    DES_EncryptFaster(text, key1, cipher, keysM1);
    DES_DecryptFaster(cipher, key2, cipher1, keysM2);
    DES_EncryptFaster(cipher1, key3, cipherText, keysM3);
}

void _3DES_Decrypt(unsigned char* text, int* key1, int* key2, int* key3, unsigned char* decipheredText)
{
    static int keysM1[16][48];
    static int keysM2[16][48];
    static int keysM3[16][48];
    static int des3StateDe = 0;
    unsigned char cipher1[8], cipher[8];

    if(des3StateDe==0) {
        getKeys(key1,keysM1[0]);
        getKeys(key2,keysM2[0]);
        getKeys(key3,keysM3[0]);
        des3StateDe=1;
    }
    
    DES_DecryptFaster(text, key3, cipher1, keysM3);
    DES_EncryptFaster(cipher1, key2, cipher, keysM2);
    DES_DecryptFaster(cipher, key1, decipheredText, keysM1);
}
