#include "aes.h"
#include <string.h>

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

void AESencryptFaster(unsigned char* plainText, unsigned char* cipherText, unsigned char*  key, int rounds, unsigned char* expandedKey){

    //unsigned char expandedKey[176];
    //keyExpansion(key, expandedKey);
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
                ans[i][j] ^= fastMultiplyGF(matrixA2[k][j],matrixB[i][k]) ;  // multiplication in GF(2^8)
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


unsigned char fastMultiplyGF(unsigned char polynomial, int multiplier ){
    
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

void AESdecryptFaster(unsigned char* cipherText,unsigned char* resultText, unsigned char*  key, int rounds, unsigned char* expandedKey){
    int endofKey=176-16;
    //unsigned char expandedKey[176];
    //keyExpansion(key, expandedKey);
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
