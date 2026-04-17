#ifndef MODES_H
#define MODES_H
#include <stdio.h>


void Xor(unsigned char *output, unsigned char* other, int n);

void ecb_encrypt( int blockSize, int* key, char* infilename, char* outfilename, char* encryption);
void ecb_decrypt( int blockSize, int* key, char* infilename, char* outfilename, char* encryption);

void cbc_encrypt(  int blockSize, int* key, char* infilename, char* outfilename, unsigned char* iv, char* encryption);
void cbc_decrypt( int blockSize, int* key, char* infilename, char* outfilename, unsigned char* iv, char* encryption);

void cfb_encrypt(  int blockSize, int* key, char* infilename, char* outfilename, unsigned char* iv, char* encryption);
void cfb_decrypt( int blockSize, int* key, char* infilename, char* outfilename, unsigned char* iv, char* encryption);

void ofb_encrypt(  int blockSize, int* key,char* infilename, char* outfilename, unsigned char* iv, char* encryption);
void ofb_decrypt( int blockSize, int* key, char* infilename, char* outfilename, unsigned char* iv, char* encryption);

void counter_decrypt( int blockSize, int* key, char* infilename, char* outfilename,  unsigned char* iv, char* encryptionType);
void counter_encrypt( int blockSize, int* key, char* infilename, char* outfilename, unsigned char* iv, char* encryptionType);

int pkcs7_Pad(int blockSize,FILE* filename, unsigned char ** textstream ); // returns blocks needed
void pkcs7_remove_Pad(int blockSize,FILE* decrypted,unsigned char** outstream , int filesize);

#endif
