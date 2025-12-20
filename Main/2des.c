#include "2des.h"
#include "des.h"


void _2DES_Encrypt(unsigned char* text, int* key1, int* key2, unsigned char* cipherText)
{
    char cipher1[8];

    DES_Encrypt(text, key1, cipher1);

    DES_Encrypt(cipher1, key2, cipherText);
}

void _2DES_Decrypt(unsigned char* text, int* key1, int* key2, unsigned char* decipheredText)
{
    unsigned char cipher1[8];

    DES_Decrypt(text, key2, cipher1);

    DES_Decrypt(cipher1, key1, decipheredText);
}

