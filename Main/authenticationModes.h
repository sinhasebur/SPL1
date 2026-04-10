#ifndef AUTHMODES_H
#define AUTHMODES_H

__uint128_t GFmultiply(__uint128_t x, __uint128_t y);
__uint128_t bytesTo128(unsigned char* bytes);
__uint128_t incr(__uint128_t Yi);

void gcm_encryptText(int* intKey, unsigned char* plaintext, int length, unsigned char* iv, unsigned char ivLength, unsigned char* outCipher, unsigned char* outTag);
void gcm_decryptText(int* intKey, unsigned char* ciphertext, int length, unsigned char* iv, unsigned char ivLength, unsigned char* outCipher, unsigned char* outTag);

void gcm_decrypt(int* intKey, char* infilename, unsigned char* iv, unsigned char* outFilename, unsigned char* outTag);
void gcm_encrypt(int* intKey, char* infilename, unsigned char* iv, unsigned char* outFilename, unsigned char* outTag);

#endif