#ifndef conversions_H
#define conversions_H

void hextoBytes(char* hex, unsigned char* bytes);
void hextoBits(char* hex,int* bits);
void bitstoHex(int* bits, char* hex, int bitNumbers);
void bytetoHex(unsigned char* bytes, char* hex, int byteNumber);

__uint128_t bytesTo128(unsigned char* bytes);
void uint128ToBytes(__uint128_t integer, unsigned char* bytes);

#endif
