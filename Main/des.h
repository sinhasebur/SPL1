#ifndef DES_H 
#define DES_H

void swapBits(unsigned char* string);
void swap(unsigned  char* left, unsigned char* right);
void mangler( unsigned char* temp, int* key);
void fillExpansionMatrix( unsigned char* text, int* returnData );
void charToBinary( unsigned  char c, int*binary);
void sbox(int* data, int* returnData);
int binToInt(int data);
void pbox(int* data);
void int32bitstoChar(int* data,unsigned  char* returnchar);
void xOr(unsigned char * x, unsigned  char* y);
void DES_Encrypt(unsigned  char* plaintext ,int* key,unsigned  char* cipherText);
void DES_Decrypt(unsigned char* cipherText, int* key,unsigned  char* decryptedText);
void permutedChoice1(int *x, int *y);
void permutedChoice2(int *x, int *y);
void lcs(int *x);
void getKeys(int* x, int* y);


#endif
