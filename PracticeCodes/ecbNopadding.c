#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"

#define endl printf("\n");

void ecb_encrypy_noPadding( int blockSize, int blockNum, int* key, char* filename);
void ecb_decrypy_noPadding( int blockSize, int blockNum, int* key, char* filename);


int main()
{

    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};

    ecb_encrypy_noPadding(64,2,key, "input");

    ecb_decrypy_noPadding(64,2,key,"encrypted");

}




void ecb_encrypy_noPadding(int blockSize, int blockNum, int* key, char * filename)
{
    int bytes=blockSize/8;

    FILE *text  = fopen(filename, "rb");
    
    if(!text){
        printf("Input file %s not found", filename); endl
        return;
    }
    
    char x;
    int n=blockNum*bytes;
    char* textstream= malloc(sizeof(char) * n);
    
    for(int i=0; i<n ; i++){
        x=fgetc(text);
        if(x==EOF) break;
        else textstream[i]=x; 
    }

    char* cipherTextStream= malloc(sizeof(char) * n);
    for(int i=0;i<blockNum;i++){
        encrypt(textstream+ i*bytes,key,cipherTextStream + i*bytes);
    }


    FILE *cipher  = fopen("encrypted", "wb");

    for(int i=0; i<n ; i++){
        fputc(cipherTextStream[i],cipher);
    }

    fclose(text);
    fclose(cipher);
    free(textstream);
    free(cipherTextStream);

}




void ecb_decrypy_noPadding(int blockSize, int blockNum, int* key, char* file )
{
    int bytes=blockSize/8;
    FILE *text  = fopen(file, "rb");

    char x;
    int n=blockNum*bytes;
    char* textstream= malloc(sizeof(char) * n);
    
    for(int i=0; i<n ; i++){
        x=fgetc(text);
        if(x==EOF)
            break;
        else
            textstream[i]=x; 
    }    
    
    char* out= malloc(sizeof(char) * n);

    for(int i=0;i<blockNum;i++){
        decrypt(textstream+ i*bytes,key,out+ i*bytes);
    }
    
    FILE *deciphered  = fopen("decrypted", "wb");

    for(int i=0; i<n ; i++){
        fputc(out[i],deciphered);
    }

    fclose(text);
    fclose(deciphered);
    free(textstream);
    free(out);

}







