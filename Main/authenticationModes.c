#include <stdio.h>
#include <string.h>
#include "authenticationModes.h"
#include "conversions.h"
#include "aes.h"
#include <stdlib.h>
#include <stdint.h>

__uint128_t R= (__uint128_t)(1+(1LL<<5)+(1LL<<6)+(1LL<<7))<<120;
__uint128_t M[128]={0};



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




//PAGE 7
// H= E(K, 0^128 )
//  Y0 = IV 0^31 1 if len(IV ) = 96,  Y0 = GHASH(H, {}, IV ) otherwise.
// Yi = incr(Yi−1) for i = 1, . . . , n
//Ci = Pi ⊕ E(K, Yi) for i = 1, . . . , n − 1
//Cn* = Pn* ⊕ MSBu(E(K, Yn))
// T' = MSBt(GHASH(H, A, C) ⊕ E(K, Y0))

void gcm_encryptText(int* intKey, unsigned char* plaintext, int length, unsigned char* iv, unsigned char ivLength, unsigned char* outCipher, unsigned char* outTag){

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

    // printf("H ");
    // for(int i=0;i<16;i++){
    //     printf("%02x ", h[i]); 
    // }
     
    //  Y0 = IV 0^31 1 if len(IV ) = 96,  Y0 = GHASH(H, {}, IV ) otherwise.

    unsigned char y0[16]={0};
    if(ivLength==12){
        for(int i=0;i<12;i++){
            y0[i]=iv[i];
        }
        y0[12]=0; y0[13]=0; y0[14]=0; y0[15]=1;
    }
    else{
        printf("unsupported iv length");  
    }
    
    __uint128_t Y0 = bytesTo128(y0);

    // printf("Y ");
    // for(int i=0;i<16;i++){
    //     printf("%02x ", y0[i]); 
    // }
     

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





void gcm_decryptText(int* intKey, unsigned char* ciphertext, int length, unsigned char* iv, unsigned char ivLength, unsigned char* outCipher, unsigned char* outTag){

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
        printf("unsupported iv length");  
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

    // char x[33];
    // bytetoHex(outTag, x,16);
    // printf("%s",x);

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




void gcm_encrypt(int* intKey, char* infilename, unsigned char* iv, unsigned char* outFilename, unsigned char* outTag){

    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("Input file \"%s\" not found", infilename);
        exit(1);
    }

    int i=0,length=0;
    while((i=fgetc(text))!=EOF){
        length++;
    }

    fseek(text, 0, SEEK_SET);
    
    unsigned char* plaintext=malloc(length);
    
    int x,j=0;
    while((x=fgetc(text))!=EOF){
        plaintext[j]=x;
        j++;
    }

    unsigned char* ciphertext=malloc(length);
    gcm_encryptText(intKey,plaintext, length, iv, 12,ciphertext,outTag );

    FILE *output= fopen(outFilename,"wb");

    if (output) {
        fwrite(ciphertext,1,length,output);
        
    }else{
        printf("Could not get output file %s",outFilename);
    }

    free(plaintext);
    free(ciphertext);
    fclose(output);
    fclose(text);

}




void gcm_decrypt(int* intKey, char* infilename, unsigned char* iv, unsigned char* outFilename, unsigned char* outTag){

    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("Input file %s not found", infilename);
        return;
    }

    int i=0,length=0;
    while((i=fgetc(text))!=EOF){
        length++;
    }

    fseek(text, 0, SEEK_SET);
    
    unsigned char* ciphertext=malloc(length);
    
    int x,j=0;
    while((x=fgetc(text))!=EOF){
        ciphertext[j]=x;
        j++;
    }

    unsigned char* plaintext=malloc(length);
    gcm_decryptText(intKey,ciphertext, length, iv, 12,plaintext,outTag );

    FILE *output= fopen(outFilename,"wb");

    if (output) {
        fwrite(plaintext,1,length,output);
        
    }else{
        printf("Could not get output file %s",outFilename);
    }

    free(plaintext);
    free(ciphertext);
    fclose(output);
    fclose(text);
    
}