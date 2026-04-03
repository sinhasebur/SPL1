#include <stdio.h>
#include <string.h>
//#include "authenticationModes.h"
#include <stdlib.h>
#include <stdint.h>

#define endl printf("\n");

__uint128_t R= (__uint128_t)(1+(1LL<<5)+(1LL<<6)+(1LL<<7))<<120;
__uint128_t M[128]={0};

__uint128_t GFmultiply(__uint128_t x, __uint128_t y);
__uint128_t bytesTo128(unsigned char* bytes);
__uint128_t incr(__uint128_t Yi);

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
unsigned char multiplyGF(unsigned char polynomial, int multiplier );
void addRoundKey(unsigned char* plainText, unsigned char* key);
void inverseShiftRows(unsigned char* cipherText);
void AESdecrypt(unsigned char* cipherText,unsigned char* resultText, unsigned char*  key, int rounds);



// Algorithm 1 Multiplication in ). Computes the value of Z = X · Y , where X, Y and
// Z ∈ GF(2^128).
// Z ← 0, V ← X
// for i = 0 to 127 do
// if Yi = 1 then
// Z ← Z ⊕ V
// end if
// if V127 = 0 then
// V ← rightshift(V )
// else
// V ← rightshift(V ) ⊕ R
// end if
// end for
// return Z

// The multiplication operation uses the special element R = 11100001 || 0^120

__uint128_t GFmultiply(__uint128_t x, __uint128_t y){

    __uint128_t Z=0;
    __uint128_t v=x;
       

    for(int i=0;i<128;i++){
        if ((y>>(127-i)) & 1) {
            Z=Z^v;
        }
        if((v&1)==0){
            v>>=1;
        }
        else{
            v>>=1;
            v=v^R;
        }
    }
    return Z;
}


void uint128ToBytes(__uint128_t integer, unsigned char* bytes) {
    for (int i = 15; i >= 0; i--) {
        bytes[i] = (unsigned char)(integer & 0xFF);
        integer >>= 8;
    }
}

//PAGE 7
// H= E(K, 0^128 )
//  Y0 = IV 0^31 1 if len(IV ) = 96,  Y0 = GHASH(H, {}, IV ) otherwise.
// Yi = incr(Yi−1) for i = 1, . . . , n
//Ci = Pi ⊕ E(K, Yi) for i = 1, . . . , n − 1
//Cn* = Pn* ⊕ MSBu(E(K, Yn))
// T' = MSBt(GHASH(H, A, C) ⊕ E(K, Y0))

void gcm_encrypt(int* intKey, unsigned char* plaintext, int length, unsigned char* iv, unsigned char ivLength, unsigned char* outCipher, unsigned char* outTag){

    unsigned char key[16] ={0};

    for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++) {
            if(intKey[i*8+j]) key[i] |=(1<<(7-j));
        }
    }

    // H= E(K, 0^128 )
    unsigned char h[16], zero[16]={0};

    AESencrypt(zero, h, key, 10);
    __uint128_t H = bytesTo128(h);

    printf("H ");
    for(int i=0;i<16;i++){
        printf("%02x ", h[i]); 
    }
    endl
    
    //  Y0 = IV 0^31 1 if len(IV ) = 96,  Y0 = GHASH(H, {}, IV ) otherwise.

    unsigned char y0[16]={0};
    if(ivLength==12){
        for(int i=0;i<12;i++){
            y0[i]=iv[i];
        }
        y0[12]=0; y0[13]=0; y0[14]=0; y0[15]=1;
    }
    else{
        printf("unsupported iv length"); endl
    }
    
    __uint128_t Y0 = bytesTo128(y0);

    printf("Y ");
    for(int i=0;i<16;i++){
        printf("%02x ", y0[i]); 
    }
    endl

    // Yi = incr(Yi−1) for i = 1, . . . , n
    
    __uint128_t Yi=incr(Y0);
    
    unsigned char e[16];
    unsigned char y0_for_encrypt[16]; 
    memcpy(y0_for_encrypt, y0, 16); 
    AESencrypt(y0_for_encrypt, e, key, 10);
    __uint128_t E_Y0 = bytesTo128(e);  //store for E(K, Y0)

    __uint128_t ghash=0;
    
    int numBlocks=(length+15)/16;

    
    for (int i = 0; i<numBlocks; i++) {
        unsigned char K[16], yBytes[16], block[16]={0};
        int offset = i*16;
        int blockLength;
        if(length-offset>16){
            blockLength=16;
        }
        else{
            blockLength=length-offset;
        }
        
        uint128ToBytes(Yi, yBytes);
        AESencrypt(yBytes,K, (unsigned char*)key, 10);

        //Ci = Pi ⊕ E(K, Yi) for i = 1, . . . , n − 1
        //when length is not 16, Cn* = Pn* ⊕ MSBu(E(K, Yn))

        for (int j=0; j<blockLength; j++) {
            outCipher[offset+j]= plaintext[offset + j] ^ K[j];
        }

        //Xi=(Xi−1 ⊕ Ci) · H
        unsigned char Ci[16]={0};
        memcpy(Ci, outCipher+offset, blockLength);
        ghash =ghash^bytesTo128(Ci);
        ghash =GFmultiply(ghash, H);

        Yi =incr(Yi);
    }

    //Xi = (Xn ⊕(len(C))) · 
    __uint128_t lenC = (__uint128_t)(length*8); 
    ghash= ghash ^ lenC;
    ghash= GFmultiply(ghash, H);

    //T = GHASH ^ E(K, Y0)
    __uint128_t Tag = ghash ^ E_Y0;
    uint128ToBytes(Tag, outTag);

} 


__uint128_t bytesTo128(unsigned char* bytes){
    __uint128_t integer=0;

    for(int i=0;i<16;i++){
        integer= (integer<<8)|(bytes[i]);
    }
    return integer;
}



void gcm_decrypt(int* intKey, unsigned char* ciphertext, int length, unsigned char* iv, unsigned char ivLength, unsigned char* outCipher, unsigned char* outTag){

    unsigned char key[16] ={0};

    for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++) {
            if(intKey[i*8+j]) key[i] |=(1<<(7-j));
        }
    }

    // H= E(K, 0^128 )
    unsigned char h[16], zero[16]={0};

    AESencrypt(zero, h, key, 10);
    __uint128_t H = bytesTo128(h);

    unsigned char y0[16]={0};
    if(ivLength==12){
        for(int i=0;i<12;i++){
            y0[i]=iv[i];
        }
        y0[12]=0; y0[13]=0; y0[14]=0; y0[15]=1;
    }
    else{
        printf("unsupported iv length"); endl
    }
    
    __uint128_t Y0 = bytesTo128(y0);

    
    __uint128_t Yi=incr(Y0);
    
    unsigned char e[16];
    unsigned char y0_for_encrypt[16]; 
    memcpy(y0_for_encrypt, y0, 16); 
    AESencrypt(y0_for_encrypt, e, key, 10);
    __uint128_t E_Y0 = bytesTo128(e);  //store for E(K, Y0)

    __uint128_t ghash=0;
    
    int numBlocks=(length+15)/16;

        

    for (int i = 0; i<numBlocks; i++) {
        unsigned char K[16], yBytes[16], block[16]={0};
        int offset = i*16;
        int blockLength;
        
        if(length-offset>16){
            blockLength=16;
        }
        else{
            blockLength=length-offset;
        }

        //Xi=(Xi−1 ⊕ Ci) · H
        unsigned char Ci[16]={0};
        memcpy(Ci, ciphertext+offset, blockLength);
        ghash =ghash^bytesTo128(Ci);
        ghash =GFmultiply(ghash, H);

        uint128ToBytes(Yi, yBytes);
        AESencrypt(yBytes,K, (unsigned char*)key, 10);

        //Ci = Pi ⊕ E(K, Yi) for i = 1, . . . , n − 1
        //when length is not 16, Cn* = Pn* ⊕ MSBu(E(K, Yn))

        for (int j=0; j<blockLength; j++) {
            outCipher[offset+j]= ciphertext[offset + j] ^ K[j];
        }

        Yi =incr(Yi);
    }

    //Xi = (Xn ⊕(len(C))) · 
    __uint128_t lenC = (__uint128_t)(length*8); 
    ghash= ghash ^ lenC;
    ghash= GFmultiply(ghash, H);

    //T = GHASH ^ E(K, Y0)
    __uint128_t Tag = ghash ^ E_Y0;
    uint128ToBytes(Tag, outTag);

} 





//Page 7
// The function incr(), which treats the rightmost 32
// bits of its argument as a nonnegative integer with the least significant bit on the right, and increments this value modulo 232. 
// More formally, the value of incr(FiI) is Fi(I + 1 mod 2^32).


__uint128_t incr(__uint128_t Yi){
    
    __uint128_t increment=0xFFFFFFFF, fixed= ~increment;

    __uint128_t Yf=Yi&fixed;
    __uint128_t YI=Yi&increment;

    YI++;
    YI=YI&increment;
    return Yf | YI;
}




/////------------------------------------------------remove after header added-------------------------------------------------




void AESencrypt(unsigned char* plainText, unsigned char* cipherText, unsigned char*  key, int rounds){

    unsigned char expandedKey[176];
    keyExpansion(key, expandedKey);
    addRoundKey(plainText, expandedKey);

    for(int i=0;i<rounds-1;i++){
       
        substituteBytes( plainText);

        shiftRows(plainText);
        
        mixColumns(plainText);
        
        addRoundKey(plainText, expandedKey+(i+1)*16);

        
    }
        substituteBytes( plainText);

        shiftRows(plainText);
        
        addRoundKey(plainText, expandedKey+(rounds)*16);

    memcpy(cipherText, plainText, sizeof(char)*16);
}

void leftCircularShiftWord(unsigned char word[4]){

    unsigned char temp;
    temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

void subWord(unsigned char word[4]){

    for(int i= 0; i<4 ;i++){
        word[i]=s_box(word[i]);

    }
}

void keyExpansion(unsigned char key[16], unsigned char expandedKey[176]){

    int word;
    int j;
    unsigned char temp[4];  
    unsigned char Rcon[11] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36};    
    memcpy(expandedKey,key, sizeof(char)*16);

    for(word=4;word<44;word++){

        //memcpy(temp, expandedKey+(word-1)*4, sizeof(char)*16);
        for(j = 0 ; j < 4 ; j++){
            temp[j] = expandedKey[ (word-1)*4+j];
        }

        if(word%4==0){
            leftCircularShiftWord(temp);
            subWord(temp);
            temp[0] = temp[0] ^ Rcon[word/4];
        }

        for(int i=0 ; i<4 ; i++){
            expandedKey[word*4 + i] = expandedKey[(word-4)*4 + i] ^ temp[i];

        }
    }
}




int s_box(unsigned char x){
    int s_box[16][16] = {
        {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
        {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
        {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
        {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
        {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
        {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
        {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
        {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
        {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
        {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
        {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
        {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
        {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
        {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
        {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
        {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
    };
    
    int row = x / 16;
    int col = x % 16;
    return s_box[row][col];
}

int inverse_s_box(unsigned char x){
    int inv_s_box[16][16] = {
        {0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB},
        {0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB},
        {0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E},
        {0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25},
        {0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92},
        {0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84},
        {0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06},
        {0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B},
        {0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73},
        {0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E},
        {0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B},
        {0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4},
        {0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F},
        {0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xe5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF},
        {0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61},
        {0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D}
    };

    int row=x/16;
    int col=x%16;
    return inv_s_box[row][col];
}


void substituteBytes(unsigned char* plainText){
    for(int i=0; i<16; i++){
        plainText[i] = s_box(plainText[i]);
    }
}

void inverseSubstituteBytes(unsigned char* cipherText){
    for(int i=0; i<16; i++){
        cipherText[i] = inverse_s_box(cipherText[i]);
    }
}


void shiftRows(unsigned char* plainText){
    
    // for(int i=0;i<4;i++){
    //     leftCircularShift(plainText+ (i)*4, i);
    // }
    unsigned char temp[16];
    memcpy(temp, plainText, 16);
    int shiftMap[16] = {
        0,  5, 10, 15,  
        4,  9, 14,  3,  
        8, 13,  2,  7,
        12,  1,  6, 11 
    };
    for(int i=0;i<16;i++){
        plainText[i]=temp[shiftMap[i]];
    }
}


// void leftCircularShift(unsigned char* start, int number){
    
//     unsigned char temp[4]; memcpy(temp, start, sizeof(unsigned char)*4);

//     for(int i=0;i<4;i++){
//         start[i] = temp[(i + number) % 4];
//     }
// }


void mixColumns(unsigned char* plainText){
    int mix_columns_matrix[4][4] ={
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2}
    };
    
    // printf("function called"); endl;

    matrixMultiply(plainText, mix_columns_matrix);
}

void inverseMixColumns(unsigned char* cipherText){
    
    int inv_mix_columns_matrix[4][4] = {
        {14, 11, 13,  9},
        { 9, 14, 11, 13},
        {13,  9, 14, 11},
        {11, 13,  9, 14}
    };
    
    
    matrixMultiply(cipherText, inv_mix_columns_matrix);
}


void matrixMultiply(unsigned char* matrixA, int matrixB[4][4]){
    
    int ans[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    unsigned char matrixA2[4][4];
    int w=0;
    
    for(int i=0;i<4 && w<16;i++){
        for(int j=0;j<4 && w<16;j++){
            matrixA2[j][i]=matrixA[w++];
        }
    }

    //makeColumnWise(matrixA2);
    // printf("to 2d worked ig"); endl

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                // printf("Trying to multiply %c and %d ", matrixA2[i][j], matrixB[k][j]);
                ans[i][j] ^= multiplyGF(matrixA2[k][j],matrixB[i][k]) ;  // change to multiplication in GF(2^8)
            }
            
        }
    }
    int k=0;
    for(int i=0;i<4 && k<16;i++){
        for(int j=0;j<4 && k<16;j++,k++){
            matrixA[k]=ans[j][i];
        }
    }
}




unsigned char multiplyGF(unsigned char polynomial, int multiplier ){
    
    long long overflowed=polynomial;
    // printf("polynomial %x", overflowed); endl

    if(multiplier==1){
        overflowed=overflowed;
    }
    else if(multiplier==2){
        overflowed<<=1;
    }
    else if(multiplier==3){
        long long temp=overflowed;
        overflowed<<=1;
        overflowed=overflowed^temp;
    
    }
    else if(multiplier==9){ //1001
        long long temp=overflowed;
        overflowed<<=3;
        overflowed=overflowed^temp;
    }
    else if(multiplier==11){ //1011
        long long temp=overflowed;
        overflowed<<=3;
        long long temp2=temp;
        temp2<<=1;
        overflowed=overflowed^temp2;
        overflowed=overflowed^temp;
    } 
    else if(multiplier==13){ //1101
        long long temp=overflowed;
        overflowed<<=3;
        long long temp2=temp;
        temp2<<=2;
        overflowed=overflowed^temp2;
        overflowed=overflowed^temp;
    }
    else if(multiplier==14){ //1110
        long long temp=overflowed;
        overflowed<<=3;
        long long temp2=temp;
        temp2<<=2;
        overflowed=overflowed^temp2;
        temp<<=1;
        overflowed=overflowed^temp;
    }
        
    // printf("overflowed is %x", overflowed); endl

    //divide by x^8 + x^4 + x^3 + x + 1
    long long irreduciblePolynomial = (1LL<<8) + (1LL<<4) + (1LL<<3) + (1LL<<1) + 1;
    long long limit = (1LL << 8);
    // printf("irreducible Polynomial is %x", irreduciblePolynomial); endl

    while(overflowed>=limit){
        int leadingbit=-1;
        long long checkLeading=overflowed;
        long long divisor=irreduciblePolynomial;
        
        while(checkLeading>0){leadingbit++; checkLeading>>=1;}

        int shifting=leadingbit-8;
        divisor<<=shifting;
        // printf("new divisor is %x" , divisor); endl
        
        overflowed ^=divisor;
        // printf("%d\n",overflowed); endl
    }

    return overflowed;

} 


void addRoundKey(unsigned char* plainText, unsigned char* key){
    for(int i=0;i<16;i++){
        plainText[i]=plainText[i]^key[i];// addition in Finite field 
    }
}


void inverseShiftRows(unsigned char* cipherText){
    unsigned char temp[16];
    memcpy(temp, cipherText, 16);

    int modifiedISRMatrix[16]={
        0, 13, 10,  7, 
        4,  1, 14, 11,  
        8,  5,  2, 15,
        12,  9,  6,  3
    };

    for (int i = 0; i < 16; i++) {
        cipherText[i] = temp[modifiedISRMatrix[i]];
    }

}

void AESdecrypt(unsigned char* cipherText,unsigned char* resultText, unsigned char*  key, int rounds){
    int endofKey=176-16;
    unsigned char expandedKey[176];
    keyExpansion(key, expandedKey);
    addRoundKey(cipherText, expandedKey+endofKey);

    for(int i=0;i<rounds-1;i++){
        
        inverseShiftRows(cipherText);
        inverseSubstituteBytes( cipherText);
        addRoundKey(cipherText, expandedKey + endofKey-(i+1)*16);
        inverseMixColumns(cipherText);
        
    }
        inverseShiftRows(cipherText);
        inverseSubstituteBytes( cipherText);
        
        
        addRoundKey(cipherText, expandedKey);

        memcpy(resultText, cipherText, sizeof(char)*16);
}




//------- covered by headers-----



// only for testcases

void hextoBytes(const char* hex, unsigned char* bytes) {
    for (int i=0, k=0; i<strlen(hex); i+=2,k++) { 
        unsigned char char1=0,char2=0;
        char1= hex[i];
        if(i+1<strlen(hex))
            char2= hex[i+1];
        
        int value1=0,value2=0;
        if(char1>='0' && char1<='9'){
            value1=char1-'0';
        }
        else{
            value1=10+(char1-'a');
        }

        if(char2>='0' && char2<='9'){
            value2=char2-'0';
        }
        else{
            value2=10+(char2-'a');
        }

        bytes[k]=value1*16+value2;
    }
}

void hextoBits(char* hex,int* bits) {

    for (int i=0; i<32; i++) { 
        unsigned char byte;
        byte= hex[i];
        int value;

        if(byte>='0' && byte<='9'){
            value=byte-'0';
        }
        else{
            value=10+(byte-'a');
        }

        for(int j=0;j<4;j++){
            bits[4*i+j]= (value >> (3-j))&1;
        }
    }
}


int main() {
    //testcase 2
    
    //printf("Testcase 2"); endl
    // int key[128] ={0}; 
    // unsigned char plaintext[16] = {0};
    // unsigned char iv[12] = {0}; 
    
    // unsigned char cipherText[16];
    // unsigned char tag[16];

    // gcm_encrypt(key, plaintext, 16, iv,12, cipherText, tag);

    // printf("Ciphertext is ");
    
    // for(int i=0; i<16; i++) {
    //     printf("%02x", cipherText[i]);
    // }
    // printf("\nExpected is 0388dace60b6a392f328c2b971b2fe78\n\n");

    // printf("Tag is   ");
    // for(int i=0; i<16; i++){
    //     printf("%02x", tag[i]);
    // } 
    // printf("\nExpected tag is ab6e47d42cec13bdf53a67b21257bddf\n");


    

// page 28 test 3


    printf("                          TESTCASE #3         "     ); endl

    char* hexKey= "feffe9928665731c6d6a8f9467308308";
    char* hexIV ="cafebabefacedbaddecaf888";
    char* hexPT ="d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255";

    int key[128];
    unsigned char iv[12];
    unsigned char plaintext[64]; 
    
    hextoBits(hexKey, key);
    hextoBytes(hexIV, iv);
    hextoBytes(hexPT, plaintext);

    unsigned char ciphertext[64];
    unsigned char tag[16];

    endl 
    gcm_encrypt(key, plaintext, 64, iv, 12, ciphertext, tag); 
    endl

    printf("Ciphertext: ");
    for(int i=0; i<64; i++)
        printf("%02x", ciphertext[i]);
    endl

    printf("Tag: ");
    for(int i=0; i<16; i++) printf("%02x", tag[i]);
    printf("\n");

    unsigned char out[64];
    unsigned char tag2[16];

    gcm_decrypt(key, ciphertext, 64, iv, 12, out, tag2);

    printf("plaintext decrypted: " ); 
    for(int i=0; i<64; i++) printf("%02x", out[i]);
    endl

    printf("decryption Tag: ");
    for(int i=0; i<16; i++) printf("%02x", tag2[i]);
    endl
}

