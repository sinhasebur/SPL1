#include<stdio.h>
#include<string.h>
void swapBits(char* string);
void swap(char* a, char* b);

int main(){
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
    
    swapBits(plainText);
    printf("\nswapped bits are: \n");
    for(int i = 0;i<8;i++) {
        printf("%02X ", plainText[i]);
    }
    printf("\n");

    
}


void swapBits(char* temp){
    for(int i=0;i<4;i++){
        swap(&temp[i],&temp[4+i]);
    }
}

void swap(char* a, char* b){
    char temp;
    temp=*a; *a=*b; *b=temp;
}