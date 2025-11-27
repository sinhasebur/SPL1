#ifndef DES_H 
#define DES_H

void swapBits(char* string);
void swap(char* a, char* b);
void mangler(char* temp, int* key);
void fillExpansionMatrix(char* temp, int* x);
void charToBinary(char c, int*x);
void sbox(int* x, int* z);
int binToInt(int x);
void pbox(int* x);
void int32bitstoChar(int* x, char* y);
void xOr(char * x, char* y);
void encrypt(char* x ,int* z);
void decrypt(char* x, int* z);
void permutedChoice1(int *x, int *y);
void permutedChoice2(int *x, int *y);
void lcs(int *x);
void getKeys(int* x, int* y);

#endif
