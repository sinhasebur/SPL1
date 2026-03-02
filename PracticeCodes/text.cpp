#include <stdio.h>
#include <string.h>


#define endl printf("\n");

void encrypt(unsigned char* plainText, unsigned char*  key, int rounds);
void substituteBytes(unsigned char* plainText);
void shiftRows(unsigned char* plainText);
void leftCircularShift(unsigned char* start, int shifts);
void mixColumns(unsigned char* plainText);
void matrixMultiply(unsigned char* matrixA, int matrixB[4][4]);
void addRoundKey(unsigned char* plainText, unsigned char* key);
void makeColumnWise(unsigned char* plaintext);
int s_box(unsigned char byte);
unsigned char multiplyGF(unsigned char polynomial, int integer );
void keyExpansion(unsigned char* key, unsigned char* expandedKey);



int main(){
    unsigned char plainText[16]={"123456789abcefg"};
   // makeColumnWise(plainText);
    unsigned char key[16]={'1','2','3','4','5','6','7','8','9','x','a','b','c','d','e','f'};
    encrypt(plainText, key, 10);
    for(int i = 0; i < 16; i++){
        printf("%02x ", plainText[i]);
    }
    printf("\n");

    // unsigned x=multiplyGF(1, 1);
    
    // printf("%x\n",x);

}



void encrypt(unsigned char* plainText, unsigned char*  key, int rounds){
    //intital permute();
    unsigned char expandedKey[176];
    keyExpansion(key, expandedKey);
    addRoundKey(plainText, expandedKey);

    for(int i=0;i<rounds-1;i++){
        // printf("1 %d",i); endl
        substituteBytes( plainText);

        // printf("2 %d",i); endl
        shiftRows(plainText);
        
        // printf("3 %d",i); endl
        mixColumns(plainText);
        
        // printf("4 %d",i); endl
        addRoundKey(plainText, expandedKey+(i+1)*16);
    }
        // printf("5"); endl
        substituteBytes( plainText);

        // printf("6"); endl
        shiftRows(plainText);
        
        // printf("7"); endl
        addRoundKey(plainText, expandedKey+(rounds)*16);

}

void RightCircularShiftWord(unsigned char word[4]){

    unsigned char temp;
    temp = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = temp;
}

void SubWord(unsigned char word[4]){

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
            RightCircularShiftWord(temp);
            SubWord(temp);
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



void substituteBytes(unsigned char* plainText){
    for(int i=0; i<16; i++){
        plainText[i] = s_box(plainText[i]);
    }
}


void shiftRows(unsigned char* plainText){
    
    // for(int i=0;i<4;i++){
    //     leftCircularShift(plainText+ (i)*4, i);
    // }
    unsigned char temp[16];
    memcpy(temp, plainText, 16);
    plainText[1] = temp[5];  plainText[5] = temp[9];  plainText[9] = temp[13]; plainText[13] = temp[1];
    plainText[2] = temp[10]; plainText[6] = temp[14]; plainText[10] = temp[2];  plainText[14] = temp[6];
    plainText[3] = temp[15]; plainText[7] = temp[3];  plainText[11] = temp[7];  plainText[15] = temp[11];
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


// void matrixMultiply(unsigned char* matrixA, int matrixB[4][4]){
    
//     int ans[4][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
//     unsigned char matrixA2[4][4];
//     int w=0;
    
//     for(int i=0;i<4 && w<16;i++,w++){
//         for(int j=0;j<4 && w<16;j++,w++){
//             matrixA2[i][j]=matrixA[w];
//         }
//     }
//     // printf("to 2d worked ig"); endl

//     for(int i=0;i<4;i++){
//         for(int j=0;j<4;j++){
//             for(int k=0;k<4;k++){
//                 // printf("Trying to multiply %c and %d ", matrixA2[i][j], matrixB[k][j]);
//                 ans[i][j] ^= multiplyGF(matrixA2[i][k],matrixB[k][j]) ;  // change to multiplication in GF(2^8)
//             }
            
//         }
//     }
//     int k=0;
//     for(int i=0;i<4 && k<16;i++,k++){
//         for(int j=0;j<4 && k<16;j++,k++){
//             matrixA[k]=ans[i][j];
//         }
//     }
// }
// void matrixMultiply(unsigned char* state, int matrixB[4][4]){
//     unsigned char temp[16];
//     memcpy(temp, state, 16);

//     for(int column=0; column<4; column++){ 
//         for(int row=0; row<4; row++){ 
//             state[column*4 + row] = multiplyGF(temp[column*4 + 0], matrixB[row][0]) ^
//                              multiplyGF(temp[column*4 + 1], matrixB[row][1]) ^
//                              multiplyGF(temp[column*4 + 2], matrixB[row][2]) ^
//                              multiplyGF(temp[column*4 + 3], matrixB[row][3]);
//         }
//     }
// }


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

