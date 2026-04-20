#ifndef encryption
#define encryption

#include <stdio.h>

//master
void encrypt(unsigned char* text, int* key, unsigned char* cipherText, char* encryption);
void decrypt(unsigned char* cipherText, int* key, unsigned char* resultText, char* encryption);


//useful gets
int getType(char* x); // 1 is des, 2 is 2des , 3 is 3des , 4 is aes
char * encNumber(int i);
int getBlockSize(int i);
long long countBytes(char* inFilename);
long long countBytesFile(FILE * x);
void benchmark(int* key, unsigned char* iv, char* inFilename, char* outFilename);
#endif
