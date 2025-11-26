#include<stdio.h>
#include<string.h>


#define endl printf("\n");
// #define char unsigned char

void swapBits(char* string);
void swap(char* a, char* b);
void mangler(char* temp, int* key);
void fillExpansionMatrix(char* temp, int* x);
void charToBinary(char c, int*x);
void sbox(int* x, int* z);
int binToInt(int x);
void pbox(int* x);
void int32bitstoChar(int* x, char* y);
void xor(char * x, char* y);
void encrypt(char* l , char* r, int* z);
void decrypt(char* l , char* r, int* z);


int main()
{
    char plainText[9]; 
    scanf("%8s", plainText);
    


    swapBits(plainText);

    printf("64 bits are: ");
    for(int i=0; i<8; i++) {
        printf("%02X ", plainText[i]);
    } 
    endl
    

    char leftHalf[4]={plainText[0],plainText[1],plainText[2],plainText[3]};
    char rightHalf[4]={plainText[4],plainText[5],plainText[6],plainText[7]};
 
    int key[64];

    encrypt(leftHalf,rightHalf,key);
    
    char cipherText[8]; 
    for(int i=0;i<4; i++) cipherText[i]=leftHalf[i];
    for(int i=0;i<4; i++) cipherText[4+i]=rightHalf[i];

    printf("Ciphered bits are: ");
    for(int i=0; i<8; i++) {
        printf("%02X ", cipherText[i]);
    } 
    endl endl endl endl

    decrypt(leftHalf,rightHalf,key);

    char decipherText[8]; 
    for(int i=0;i<4; i++) decipherText[i]=leftHalf[i];
    for(int i=0;i<4; i++) decipherText[4+i]=rightHalf[i];

    printf("deciphered bits are: ");
    for(int i=0; i<8; i++) {
        printf("%02X ", decipherText[i]);
    } 
    endl


    


}


void decrypt(char* l , char* r, int * z){
int keys[16][48]={{1,1,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,1,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1}}; 

    for(int i=15;i>=0;i--){
        endl
        endl
        printf("Round 16-%d decryption :", i);
        endl
        endl
        printf("Current bits are: ");
        
        for(int i=0; i<4; i++) printf("%02X ", l[i]);
        for(int i=0; i<4; i++) printf("%02X ", r[i]);
        endl

        // int k[48];
        // memcpy(k, keys[i], 48*sizeof(int));
        
        char rtemp[4],ltemp[4];
        memcpy(rtemp,r, 4*sizeof(char));
        memcpy(ltemp,l, 4*sizeof(char));

        
        mangler(l,keys[i]);
        xor(rtemp,l);

        memcpy(r,ltemp,4*sizeof(char));

    }

}



void encrypt(char* l , char* r, int * z){
    int keys[16][48]={{1,1,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,1,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1},
{1,0,1,0,1,0,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,1,1}}; 

    for(int i=0;i<16;i++){
        endl
        endl
        printf("Round %d encryption :", i);
        endl
        endl
        printf("XCurrent bits are: ");
        
        for(int i=0; i<4; i++) printf("%02X ", l[i]);
        for(int i=0; i<4; i++) printf("%02X ", r[i]);
        endl

        // int k[48];
        // memcpy(k, keys[i], 48*sizeof(int));
        
        char rtemp[4],ltemp[4];
        memcpy(rtemp,r, 4*sizeof(char));
        memcpy(ltemp,l, 4*sizeof(char));
        
        // printf("before: ");
        // for(int i=0; i<4; i++) printf("%02X ", r[i]); endl
        mangler(r, keys[i]); // changes r
        // printf("after: ");
        
        // for(int i=0; i<4; i++) printf("%02X ", r[i]);
        // endl endl

        xor(ltemp, r);// this func puts xored value in r
        memcpy(l,rtemp, 4*sizeof(char));
    }

}



void swapBits(char* temp)
{
    for(int i=0;i<4;i++){
        swap(&temp[i],&temp[4+i]);
    }
}


void swap(char* a, char* b)
{
    char temp;
    temp=*a; *a=*b; *b=temp;
}


void mangler (char* temp, int* key)
{
    // printf("Mangler received: ");
    // for(int i=0;i<4;i++) printf("%02x ", temp[i]);
    // endl 

    int expandedMatrix[48];
    fillExpansionMatrix(temp,expandedMatrix);

    endl
    printf("Expanded Matrix is: ");
    
    for(int i=0;i<48;i++) {if(!(i%8))endl printf("%d ",expandedMatrix[i]);  }
    
    
    for(int i=0;i<48;i++){
        expandedMatrix[i]=expandedMatrix[i] ^ key[i];
    }

    int matrix[32];
    
    sbox(expandedMatrix,matrix);

    endl
    printf("Matrix after sbox is: ");
    
    for(int i=0;i<32;i++) {if(!(i%8))endl printf("%d ",matrix[i]);  }
    // int reducedMatrix[4];
    // combine4n4Bits(reducedMatrixTemp, reducedMatrix);
    
    

    pbox(matrix);

    int32bitstoChar(matrix, temp);

    // printf("Mangler made: ");
    // for(int i=0;i<4;i++) printf("%02x ", temp[i]);
    // endl 

}

void fillExpansionMatrix(char* x,int *y)
{   
    int matrix[4][8];
    for(int i=0;i<4;i++){
        charToBinary((unsigned char)x[i],matrix[i]);
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
    
    
    int k=0;
    for(int i=0;i<48;i++){
        y[i] =linearForm[expansionMatrix[i]-1];
    }


}


void charToBinary(char c, int* x)
{   
    // unsigned char uc = (unsigned char)c;
    

    for (int i = 0; i < 8; i++) {
        x[i] = (c >> (7 - i)) & 1;  
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
            {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, 
             {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8}, 
             {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0}, 
             {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
            }, 
            {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10}, 
             {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5}, {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15}, {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}}, {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1}, {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7}, {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}}, {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9}, {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4}, {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}}, {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6}, {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14}, {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}}, {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8}, {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6}, {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}}, {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6}, {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2}, {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}}, {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2}, {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8}, {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};

    
    
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


void int32bitstoChar(int* x, char* y){
    
    int k=0;
    for(int i=0;i<4;i++){
        int temp=0;
        for(int j=0;j<8;j++,k++){
            // printf("%d ", x[k]);
            if(x[k]==1)
                temp+= (1)<<j;
        }
        // endl
        // printf("%d ", temp);
        // endl 
        y[i]=(char)temp;
    }
}

void xor(char *x, char* y){
    
    // printf("received for xor x: ");
    // for(int i=0;i<4;i++) printf("%02x ", x[i]);
    // endl 
    // printf("received for xor y: ");
    // for(int i=0;i<4;i++) printf("%02x ", y[i]);
    // endl 

    for(int i=0; i<4;i++){
        int tx=x[i], ty=y[i];
        y[i]= tx^ty;
    }

    // printf("After xor x: ");
    // for(int i=0;i<4;i++) printf("%02x ", x[i]);
    // endl 
    // printf("After xor y: ");
    // for(int i=0;i<4;i++) printf("%02x ", y[i]);
    // endl 
}


