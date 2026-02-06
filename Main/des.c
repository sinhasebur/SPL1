#include "des.h"
#include <string.h>

void DES_Decrypt(unsigned char* x, int * key,unsigned  char* y)
{

    unsigned char l[4],r[4];
    unsigned char cipherText[8];

    for(int i=0;i<8;i++) cipherText[i]=x[i];
    for(int i=0;i<4; i++) l[i]=cipherText[i];
    for(int i=0;i<4; i++) r[i]=cipherText[4+i];

    int keys[16][48];
    getKeys(key, keys[0]);

    for(int i=15;i>=0;i--){
        unsigned char rtemp[4],ltemp[4];
        memcpy(rtemp,r, 4*sizeof(unsigned char));
        memcpy(ltemp,l, 4*sizeof(unsigned char));


        mangler(l,keys[i]);
        xOr(rtemp,l);

        memcpy(r,ltemp,4*sizeof(unsigned char));

    }

    unsigned char decipherText[8];
    for(int i=0;i<4; i++) decipherText[i]=r[i];
    for(int i=0;i<4; i++) decipherText[4+i]=l[i];

    for(int i=0; i<8; i++) {
        y[i]=decipherText[i];
    }
}






void DES_Encrypt(unsigned char *x, int *y, unsigned char *z)
{

    unsigned char plainText[9];
    for(int i=0;i<8;i++){plainText[i]=x[i];}

    int key[64];
    for(int i=0;i<64;i++){key[i]=y[i];}

    swapBits(plainText);

    unsigned char l[4] = {plainText[0], plainText[1], plainText[2], plainText[3]};
    unsigned char r[4] = {plainText[4], plainText[5], plainText[6], plainText[7]};

    int keys[16][48];
    getKeys(key, keys[0]);

    for (int i = 0; i < 16; i++) {

        unsigned char rtemp[4], ltemp[4];
        memcpy(rtemp, r, 4 * sizeof(unsigned char));
        memcpy(ltemp, l, 4 * sizeof(unsigned char));


        mangler(r, keys[i]); // changes r


        xOr(ltemp, r); // this func puts xOred value in r
        memcpy(l, rtemp, 4 * sizeof(unsigned char));
    }


    unsigned char cipherText[8];
    for(int i=0;i<4; i++) cipherText[i]=l[i];
    for(int i=0;i<4; i++) cipherText[4+i]=r[i];

    for(int i=0; i<8; i++) {
        z[i]=cipherText[i];
    }

}






void swapBits(unsigned char* temp)
{
    for(int i = 0; i < 4; i++){
        swap(&temp[i], &temp[4+i]);
    }
}


void swap(unsigned char *a, unsigned char *b)
{
    unsigned char temp;
    temp = *a; 
    *a = *b; 
    *b = temp;
}






void mangler (unsigned char* text, int* key)
{

    int expandedMatrix[48];
    fillExpansionMatrix(text ,expandedMatrix);

    for(int i=0;i<48;i++){
        expandedMatrix[i]=expandedMatrix[i] ^ key[i];
    }

    int matrix[32];

    sbox(expandedMatrix,matrix);


    pbox(matrix);

    int32bitstoChar(matrix, text );

}






void fillExpansionMatrix(unsigned char* text,int *returnData)
{
    int matrix[4][8];
    for(int i=0;i<4;i++){
        charToBinary((unsigned char)text[i],matrix[i]);
    }
    int linearForm[32];
    for(int i=0;i<4;i++){
        for(int j=0;j<8;j++){
            linearForm[i*8 + j]=matrix[i][j];
        }

    }

    int expansionMatrix[48]=
    {32, 1, 2, 3, 4, 5, 4, 5,
        6, 7, 8, 9, 8, 9, 10, 11,
        12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1};


    for(int i=0;i<48;i++){
        returnData[i] =linearForm[expansionMatrix[i]-1];
    }

}



void charToBinary(unsigned char c, int* x)
{

    for (int i = 0; i < 8; i++) {
        x[i] =( c>>(7-i) ) & 1;
    }

}


int binToInt(int x)
{
    int integer=0;
    int k=0;
    while(x>0){
        integer=(x&1)*k;
        k*=10;
        x=x>>1;
    }
    return integer;

}


void sbox(int* x, int* z)
{

    int s[8][4][16] =
    {
        {
            {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
            {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
            {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
            {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
        },
        {
            {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
            {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
            {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
            {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
        },
        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
        },
        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
        },
        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
        },
        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
        },
        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
        },
        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
        }
    };



    int a[8];
    for(int i=0;i<8;i++){
        int base=i*6;
        int b[6];
        for(int j=0;j<6;j++){
            b[j]=x[base+j];
        }

        int row=(b[0]<<1) | (b[5]);
        int col= ((b[1])<<3) | ((b[2])<<2) | ((b[3])<<1) | b[4];

        a[i]=s[i][row][col];
    }

    int k=0;
    for(int i=0;i<8;i++){
        for(int j=3;j>=0;j--){
            z[k]=a[i]&1;
            a[i]=a[i]>>1;
            k++;
        }
    }

}

void pbox(int *x)
{
    int p_box[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};

    int temp[32];
    for(int i=0;i<32;i++){
        temp[i]=x[p_box[i]-1];
    }
    for(int i=0;i<32;i++){
        x[i]=temp[i];
    }
}


void int32bitstoChar(int *x, unsigned  char *y)
{
    int k = 0;
    for(int i=0;i<4;i++){
        int temp=0;
        for (int j = 0;j<8;j++,k++) {

            if(x[k]==1)
                temp+= (1)<<j;
        }
        y[i]=(unsigned char)temp;
    }
}

void xOr(unsigned char *x, unsigned  char* y)
{
    for (int i = 0; i < 4; i++)
        y[i] = x[i] ^ y[i];
}

void leftCircularShift(int *x)
{
    int place = x[55];
    x[55] = 0;
    int temp;
    for (int i = 54; i >= 0; i--){
        temp  = x[i];
        x[i]  = place;
        place = temp;
    }
}

void permutedChoice1(int *x, int *y)
{
    int p1[56] = { 
        57, 49, 41, 33, 25, 17,  9, 63, 55, 47, 39, 31, 23, 15,
         1, 58, 50, 42, 34, 26, 18,  7, 62, 54, 46, 38, 30, 22,
        10,  2, 59, 51, 43, 35, 27, 14,  6, 61, 53, 45, 37, 29,
        19, 11,  3, 60, 52, 44, 36, 21, 13,  5, 28, 20, 12, 4
    };

    for (int i = 0; i < 56; i++) {
        y[i] = x[p1[i - 1]];
    }
}

void permutedChoice2(int *x, int *y)
{
    int p2[48] = {
        14, 17, 11, 24,  1,  5,
        3,  28, 15,  6, 21, 10,
        23, 19, 12,  4, 26,  8,
        16,  7, 27, 20, 13,  2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    for (int i = 0; i < 48; i++){
        y[i] = x[p2[i - 1]];
    }
}

void getKeys(int* x, int *y)
{
    int key[64];
    for(int i=0;i<64;i++) key[i]=x[i];
    int permuted1key[56];
    permutedChoice1(key, permuted1key);

    int keys[16][48];

    for (int i = 1; i <= 16; i++){
        //We shift twice for perfect square indicies
        if (i == 1 || i == 4 || i == 9 || i == 16){

            leftCircularShift(permuted1key);


            leftCircularShift(permuted1key);
        }
        else {
            leftCircularShift(permuted1key);
        }
        int temp[48];
        permutedChoice2(permuted1key, temp);

        memcpy(keys[i - 1], temp, 48 * sizeof(int));
    }

    int k = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 48; j++) {
            y[k] = keys[i][j];
            k++;
        }
    }
}
