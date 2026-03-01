#include <stdio.h>
#include <string.h>

#define endl printf("\n");

void encrypt(char* plainText, char*  key, int rounds);
void substituteBytes(char* plainText);
void shiftRows(char* plainText);
void rightCircularShift(char* start, int shifts);
void mixColumns(char* plainText);
void matrixMultiply(char* matrixA, int matrixB[4][4]);
void addRoundKey(char* plainText, char* key, int round);
void makeColumnWise(char* plaintext);
int s_box(char byte);
unsigned char multiplyGF(unsigned char polynomial, int integer );
void keyExpansion(char* key, char* expandedKey);



int main(){
    char plainText[16]={"123456789abcefg"};
    makeColumnWise(plainText);
    char key[16]={'1','2','3','4','5','6','7','8','9','x','a','b','c','e','f','g'};
    encrypt(plainText, key, 16);
    printf("%x\n",plainText);


}



void encrypt(char* plainText, char*  key, int rounds){
    //intital permute();
    char expandedKey[1][16];
    keyExpansion(key, expandedKey);

    for(int i=0;i<rounds-1;i++){
        // printf("1 %d",i); endl
        substituteBytes( plainText);

        // printf("2 %d",i); endl
        shiftRows(plainText);
        
        // printf("3 %d",i); endl
        mixColumns(plainText);
        
        // printf("4 %d",i); endl
        addRoundKey(plainText, expandedKey[rounds], i);
    }
        // printf("5"); endl
        substituteBytes( plainText);

        // printf("6"); endl
        shiftRows(plainText);
        
        // printf("7"); endl
        addRoundKey(plainText, expandedKey[rounds-1], rounds-1);

}

void keyExpansion(char* key, char* expandedKey){
    int i=0, rounds=10;
    for(;i<rounds;i++){
        char* currentKey[4][4];
        for(int p=0;p<4;p++){
            memcpy(key+(i+p)*4, currentKey[0], sizeof(char)*4);
        }
        rightCircularShift(currentKey,3); //equivalent of 1 leftCircularShift
        
        for(int p=0;p<4;p++){
            memcpy(currentKey[p],s_box(currentKey[p]), sizeof(char)*4);
        }


    }
}

void makeColumnWise(char* plainText){ 
    // aes does a matrix where 
    int columnWise[16]={0,  4,  8,  12,
                        1,  5,  9,  13,
                        2,  6,  10, 14,
                        3,  7,  11, 15};
    char temporary[16];
    
    for(int i=0;i<16;i++){
        temporary[i]=plainText[columnWise[i]];
    }

    // for(int i=0;i<16;i++){
    //     plainText[i]=temporary[i];
    // }
    memcpy(plainText, temporary, sizeof(char)*16);

}

int s_box(char x){
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


void substituteBytes(char* plainText){
    

    char temp[16];
    memcpy(temp, plainText, sizeof(char)*16);
    for(int i=0;i<16;i++){
        unsigned char x=(unsigned char) temp[i];
        int col= x%16; 
        int row= x/16;
        plainText[i]=s_box(plainText[i]);
    }

}

void shiftRows(char* plainText){
    
    for(int i=0;i<4;i++){
        rightCircularShift(plainText+ (i)*4, i);
    }
}


void rightCircularShift(char* start, int number){
    
    char temp[4]; memcpy(temp, start, sizeof(char)*4);

    for(int i=0;i<4;i++){
        start[i]=temp[(i+number)%4];
    }
}


void mixColumns(char* plainText){
    int mix_columns_matrix[4][4] ={
        {2, 3, 1, 1},
        {1, 2, 3, 1},
        {1, 1, 2, 3},
        {3, 1, 1, 2}
    };
    
    // printf("function called"); endl;

    matrixMultiply(plainText, mix_columns_matrix);
}

void matrixMultiply(char* matrixA, int matrixB[4][4]){
    
    int ans[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    char matrixA2[4][4];
    int w=0;
    
    for(int i=0;i<4 && w<16;i++,w++){
        for(int j=0;j<4 && w<16;j++,w++){
            matrixA2[i][j]=matrixA[w];
        }
    }
    // printf("to 2d worked ig"); endl

    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                // printf("Trying to multiply %c and %d ", matrixA2[i][j], matrixB[k][j]);
                ans[i][j] ^= multiplyGF(matrixA2[i][k],matrixB[k][j]) ;  // change to multiplication in GF(2^8)
            }
            
        }
    }
    int k=0;
    for(int i=0;i<4 && k<16;i++,k++){
        for(int j=0;j<4 && k<16;j++,k++){
            matrixA[k]=ans[i][j];
        }
    }
}

unsigned char multiplyGF(unsigned char polynomial, int integer ){
    
    long long overflowed=polynomial;
    // printf("polynomial %x", overflowed); endl

    overflowed<<=integer;

    // printf("overflowed is %x", overflowed); endl

    //divide by x^8 + x^4 + x^3 + x + 1
    long long irreduciblePolynomial = (1LL<<8) + (1LL<<4) + (1LL<<3) + (1LL<<1) + 1;
    long long limit = (1LL << 8);
    // printf("irreducible Polynomial is %x", irreduciblePolynomial); endl

    while(overflowed>=limit){
        int leadingbit=-1;
        int checkLeading=overflowed;
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


void addRoundKey(char* plainText, char* key, int round){
    for(int i=0;i<16;i++){
        plainText[i]=plainText[i]^key[i];// addition in Finite field 
    }
}



      