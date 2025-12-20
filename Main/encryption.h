#ifndef encryption
#define encryption
void encrypt(unsigned char* text, int* key, unsigned char* cipherText, unsigned char* encryption);
void decrypt(unsigned char* cipherText, int* key, unsigned char* resultText, unsigned char* encryption);

int getType(unsigned char* x); // 1 is des, 2 is 2des , 3 is 3des , 4 is aes

#endif
