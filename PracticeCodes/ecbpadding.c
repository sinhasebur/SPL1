#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"

#define endl printf("\n");

void ecb_encrypy_Padding( int blockSize, int* key, char* filename);
void ecb_decrypy_Padding( int blockSize, int* key, char* filename);


int main()
{
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};

    ecb_encrypy_Padding(64,key, "input");

    ecb_decrypy_Padding(64,key,"encrypted");

}




void ecb_encrypy_Padding(int blockSize,  int* key, char * filename)
{   

    int bytes=blockSize/8;

    FILE *text  = fopen(filename, "rb");
    
    if(!text){
        printf("Input file %s not found", filename); endl
        return;
    }
    
    long long n=0;
    char x;
    int i;

    while((i=fgetc(text))!=EOF){
        n++;
    }

    fseek(text, 0, SEEK_SET);

    
    
    long long blockNum= n/bytes;
    long long allocate= ((blockNum+1)*bytes);
    int excess=n%bytes;


    char* textstream= malloc(allocate);

    for(int i=0; i<n - bytes; i++){
        x=fgetc(text);
        if(x==EOF) break;
        else textstream[i]=x; 
    }


    for(int i=0;i<bytes;i++){
        x=fgetc(text);
        
        if(i<n){
            textstream[blockNum+i]=x;
        }
        else{
            textstream[blockNum+i]=excess;
        }
    }

    blockNum++;


    char* cipherTextStream= malloc(sizeof(char) * allocate);
    for(int i=0;i<blockNum;i++){
        encrypt(textstream+ i*bytes,key,cipherTextStream + i*bytes);
    }



    FILE *cipher  = fopen("encrypted", "wb");

    for(int i=0; i<allocate ; i++){
        fputc(cipherTextStream[i],cipher);
    }

    fclose(text);
    fclose(cipher);
    free(cipherTextStream);
    free(textstream);

}




void ecb_decrypy_Padding(int blockSize, int* key, char* file )
{
    int bytes=blockSize/8;
    FILE *text  = fopen(file, "rb");

    if(!text){
        printf("File to decrypt %s not found", file); endl
        return;
    }
    

    long long n=0;
    char x;
    int i;

    while((i=fgetc(text))!=EOF){
        n++;
    }
    fseek(text, 0, SEEK_SET);

    
    long long blockNum= n/bytes;
    long long allocate= ((blockNum+1)*bytes);
   


    char* textstream= malloc(allocate);
    
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

    int excess= (int)out[n-1];
    //printf("%02X",out[n-1]); endl
    for(int i=0; i<n-excess ; i++){
        fputc(out[i],deciphered);
    }  

    fclose(text);
    fclose(deciphered);
    free(textstream);
    free(out);

}
