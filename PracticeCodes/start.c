#include<stdio.h>
#include<string.h>
#include <stdbool.h>

void swapBits(char* string);
void swap(char* a, char* b);
void mangler(char* temp);
void fillExpansionMatrix(char* temp);
void charToBinary(int c, int*x);

int main()
{
    char plainText[9]; 
    scanf("%8s", plainText);
    
    printf("64 bits are: \n");

    for(int i = 0;i<8;i++) {
        printf("%02X ", plainText[i]);
    }
    swapBits(plainText);
    
    printf("\nswapped bits are: \n");
    for(int i = 0;i<8;i++) {
        printf("%02X ", plainText[i]);
    }
    
    // swapBits(plainText);
    // printf("\nswapped bits are: \n");
    // for(int i = 0;i<8;i++) {
    //     printf("%02X ", plainText[i]);
    // }
    // printf("\n");

    char leftHalf[4]={plainText[0],plainText[1],plainText[2],plainText[3]};
    char rightHalf[4]={plainText[4],plainText[5],plainText[6],plainText[7]};

    mangler(rightHalf);

    
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

void mangler (char* temp)
{
    bool expandedMatrix[8][6];
    fillExpansionMatrix(temp);
}

void fillExpansionMatrix(char* x)
{   
    int matrix[4][8];
    // 1 2 3 4 5 6 7 8
    // 9 10 11 12 13 14 15 16
    // 17 18 19 20 21 22 23 24
    // 25 26 27 28 29 30 31 32
    
    for(int i=0;i<4;i++){
        charToBinary(x[i],matrix[i]);
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
    
    int expandedMatrix[6][8];
    int k=0;
    for(int i=0;i<6;i++){
        for(int j=0;j<8;j++){
            expandedMatrix[i][j]=linearForm[expansionMatrix[k]-1];
            printf("%d", expandedMatrix[i][j]);
            k++;
        }
        printf("\n");
    }

    // for(int i=0;i<4;i++){
    //     for(int j=0;j<8;j++){
    //         printf("%d", matrix[i][j]);
    //     }
    //     printf("\n");
    // }

}

void charToBinary(int c, int* x)
{
    for(int i=7;i>=0;i--){
        x[i]=c%2; 
        c=c/2;
        // printf(".%d", x[i]);
    }
    printf("\n");
    
}