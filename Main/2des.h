#ifndef _2DES_H
#define _2DES_H


void _2DES_Encrypt(unsigned char* text, int* key1, int* key2,unsigned char* cipherText);
void _2DES_Decrypt(unsigned char* cipherText, int* key1, int* key2,unsigned char* decryptedText);


#endif
