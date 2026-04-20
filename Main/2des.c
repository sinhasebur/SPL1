#include "2des.h"
#include "des.h"


void _2DES_Encrypt(unsigned char* text, int* key1, int* key2, unsigned char* cipherText)
{
    static int keysM1[16][48];
    static int keysM2[16][48];
    static int des2StateEn = 0;
    unsigned char cipher1[8];

    if(des2StateEn==0) {
        getKeys(key1,keysM1[0]);
        getKeys(key2,keysM2[0]);
        des2StateEn=1;
    }
    
    DES_EncryptFaster(text, key1, cipher1, keysM1);
    DES_EncryptFaster(cipher1, key2, cipherText, keysM2);
}

void _2DES_Decrypt(unsigned char* text, int* key1, int* key2, unsigned char* decipheredText)
{
    static int keysM1[16][48];
    static int keysM2[16][48];
    static int des2StateDe = 0;
    unsigned char cipher1[8];

    if(des2StateDe == 0) {
        getKeys(key1, keysM1[0]);
        getKeys(key2, keysM2[0]);
        des2StateDe = 1;
    }
    
    DES_DecryptFaster(text, key2, cipher1, keysM2);
    DES_DecryptFaster(cipher1, key1, decipheredText, keysM1);
}

