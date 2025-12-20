#include "3des.h"
#include "des.h"

void _3DES_Encrypt(unsigned char* text, int* key1, int* key2,int* key3, unsigned char* cipherText)
{
    unsigned char cipher1[8], cipher[8];

    DES_Encrypt(text, key1, cipher);
    DES_Decrypt(cipher, key2,cipher1 );
    DES_Encrypt(cipher1, key3, cipherText);
}

void _3DES_Decrypt(unsigned char* text, int* key1, int* key2, int* key3, unsigned char* decipheredText)
{
    unsigned char cipher1[8], cipher[8];
    DES_Decrypt(text, key3, cipher1);
    DES_Encrypt(cipher1, key2, cipher);
    DES_Decrypt(cipher, key1, decipheredText);
}
