#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"

#define endl printf("\n");
#define char unsigned char


void ecb_encrypy_Padding( int blockSize, int* key, char* filename);
void ecb_decrypy_Padding( int blockSize, int* key, char* filename);

int pkcs7_Pad(int blockSize,FILE* filename, char ** textstream );

void pkcs7_remove_Pad(int blockSize,FILE* decrypted,char** outstream , int filesize);


int main()
{
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};

    ecb_encrypy_Padding(64,key, "input");

    ecb_decrypy_Padding(64,key,"encrypted");

}



int pkcs7_Pad(int blockSize,FILE* text , char** textstream)
{
    int bytes=blockSize/8;
    
    long long n=0;
    char x;
    int i;

    while((i=fgetc(text))!=EOF){
        n++;
    }
    n--;
    fseek(text, 0, SEEK_SET);    
    
    long long blockNum= n/bytes;
    long long allocate= ((blockNum+1)*bytes);
    int excess=n%bytes;

    (*textstream)= malloc(allocate);

    for(int i=0; i<n - excess; i++){
        x=fgetc(text);
        if(x==EOF) break;
        else (*textstream)[i]=x; 
       // printf("%c",x);
    }
   // endl

    int k=excess;

    //printf("%d",k); endl
    for(int i=0;i<bytes;i++){
        
        if(i<k){
            x=fgetc(text);
            (*textstream)[blockNum*bytes+i]=x;
           // printf("%c",x); 
        }
        else{
            (*textstream)[blockNum*bytes+i]=bytes-excess;
           // printf("a%d",bytes-excess);
        }
    }

    blockNum++;
    return blockNum;
}


void ecb_encrypy_Padding(int blockSize,  int* key, char * filename)
{   

    int bytes=blockSize/8;

    FILE *text  = fopen(filename, "rb");
    
    if(!text){
        printf("Input file %s not found", filename); endl
        return;
    }
    
    char* textstream;
    
    long long blockNum= pkcs7_Pad( blockSize, text , &textstream);

    long long allocate= (blockNum*bytes);



    char* cipherTextStream= malloc(sizeof(char) * allocate);
    for(int i=0;i<blockNum;i++){
        encrypt(textstream+ i*bytes,key,cipherTextStream + i*bytes);
        //for(int j=0;j<8;j++) printf("%c",textstream[i*bytes+j]);
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
    //printf("n%d",n); endl
    fseek(text, 0, SEEK_SET);

    
    long long blockNum= n/bytes;
    long long allocate= ((blockNum)*bytes);
   

    char* textstream= malloc(allocate);
    
    for(int i=0; i<n ; i++){
        x=fgetc(text);
        if(x==EOF){
            printf("encrypted file reading error, Check encrpytion"); endl
            break;
        }
        else
            textstream[i]=x; 
    }    
    
    char* out= malloc(sizeof(char) * n);

    int k=0;
    for(int i=0;i<blockNum;i++){
        decrypt(textstream+ i*bytes,key,out+ i*bytes);
        // for(int i=0;i<8;i++){
        //     printf("%c",out[k]);
        //     k++;
        // }
    }
    
    FILE *deciphered  = fopen("decrypted", "wb");

    pkcs7_remove_Pad(blockSize, deciphered , &out, n-1);

    int padded= (int)out[n-1];
    //printf("",out[n-1]); endl
    
    for(int i=0; i<n-padded; i++){
        fputc(out[i],deciphered);
    }  

    fclose(text);
    fclose(deciphered);
    free(textstream);
    free(out);

}

void pkcs7_remove_Pad(int blockSize,FILE* decrypted,char** outstream , int size)
{
    int padded= (*outstream)[size];
    
    for(int i=0; i<size-padded; i++){
        fputc((*outstream)[i],decrypted);
    }  
}