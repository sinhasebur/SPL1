#ifndef AES_H
#define AES_H

void AESencrypt(unsigned char* plainText,unsigned char* cipherText, unsigned char*  key, int rounds);
void leftCircularShiftWord(unsigned char word[4]);
void subWord(unsigned char word[4]);
void keyExpansion(unsigned char key[16], unsigned char expandedKey[176]);
int s_box(unsigned char x);
int inverse_s_box(unsigned char x);
void substituteBytes(unsigned char* plainText);
void inverseSubstituteBytes(unsigned char* cipherText);
void shiftRows(unsigned char* plainText);
void mixColumns(unsigned char* plainText);
void inverseMixColumns(unsigned char* cipherText);
void matrixMultiply(unsigned char* matrixA, int matrixB[4][4]);
unsigned char fastMultiplyGF(unsigned char polynomial, int multiplier );
void addRoundKey(unsigned char* plainText, unsigned char* key);
void inverseShiftRows(unsigned char* cipherText);
void AESdecrypt(unsigned char* cipherText,unsigned char* resultText, unsigned char*  key, int rounds);


#endif