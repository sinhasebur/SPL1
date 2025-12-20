#ifndef _3DES_H
#define _3DES_H

void _3DES_Encrypt(unsigned char* text, int* key1, int* key2, int* key3, unsigned char* cipherText);
void _3DES_Decrypt(unsigned char* cipherText, int* key1, int* key2,int* key3, unsigned char* decryptedText);

#endif


