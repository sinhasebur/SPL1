#ifndef conversions_H
#define conversions_H

void hextoBytes(char* hex, unsigned char* bytes);
void hextoBits(char* hex,int* bits);
void bitstoHex(int* bits, char* hex, int bitNumbers);
void bytetoHex(unsigned char* bytes, char* hex, int byteNumber);

#endif
