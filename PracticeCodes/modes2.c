#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryptions.h"

#define endl printf("\n");
#define uchar unsigned char


void xor(uchar *x, uchar* y, int n);



void ecb_encrypt( int blockSize, int* key, uchar* filename, uchar* encryption);
void ecb_decrypt( int blockSize, int* key, uchar* filename, uchar* encryption);
// issues- can leak data for images or so, needs padding

void cbc_encrypt(  int blockSize, int* key, uchar* filename, uchar* iv, uchar* encryption);
void cbc_decrypt( int blockSize, int* key, uchar* filename, uchar* iv, uchar* encryption);
// corruption easily 


void cfb_encrypt(  int blockSize, int* key, uchar* filename, uchar* iv, uchar* encryption);
void cfb_decrypt( int blockSize, int* key, uchar* filename, uchar* iv, uchar* encryption);
// stream cipher, character by character



// padding PKCS#7
int pkcs7_Pad(int blockSize,FILE* filename, uchar ** textstream ); // returns blocks needed
void pkcs7_remove_Pad(int blockSize,FILE* decrypted,uchar** outstream , int filesize);


int main()
{
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
    int key2[128]={1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0};
    //ecb_encrypy(64,key, "input");

    //ecb_decrypy(64,key,"encrypted");

    uchar iv[8]={'a','b','c','d','e','f','g','h'};

    cfb_encrypt(64,key2, "input",iv, "2des");

    cfb_decrypt(64,key2,"encrypted",iv, "2des");

}




int pkcs7_Pad(int blockSize,FILE* text , uchar** textstream)
{
    int bytes=blockSize/8;
    long long n=0;
    uchar x;
    int i;

    while((i=fgetc(text))!=EOF){
        n++;
    }
    
    fseek(text, 0, SEEK_SET);

    long long blockNum= n/bytes;
    long long allocate= ((blockNum+1)*bytes);
    int excess=n%bytes;

    (*textstream)= malloc(allocate);

    for(int i=0; i<n - excess; i++){
        x=fgetc(text);
        if(x==EOF) break;
        else (*textstream)[i]=x;
    }

    int k=excess;

    for(int i=0;i<bytes;i++){

        if(i<k){
            x=fgetc(text);
            (*textstream)[blockNum*bytes+i]=x;
        }
        else{
            (*textstream)[blockNum*bytes+i]=bytes-excess;
        }
    }

    blockNum++;
    return blockNum;
}


void ecb_encrypt(int blockSize,  int* key, uchar * filename, uchar* encryption)
{

    int bytes=blockSize/8;

    FILE *text  = fopen(filename, "rb");

    if(!text){
        printf("Input file %s not found", filename); endl
        return;
    }

    uchar* textstream;

    long long blockNum= pkcs7_Pad( blockSize, text , &textstream);

    long long allocate= (blockNum*bytes);



    uchar* cipherTextStream= malloc(sizeof(uchar) * allocate);
    for(int i=0;i<blockNum;i++){
        encrypt(textstream+ i*bytes,key,cipherTextStream + i*bytes, encryption);
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




void ecb_decrypt(int blockSize, int* key, uchar* file ,uchar* encryption )
{
    int bytes=blockSize/8;
    FILE *text  = fopen(file, "rb");

    if(!text){
        printf("File to decrypt %s not found", file); endl
        return;
    }


    long long n=0;
    uchar x;
    int i;

    while((i=fgetc(text))!=EOF){
        n++;
    }

    fseek(text, 0, SEEK_SET);


    long long blockNum= n/bytes;
    long long allocate= ((blockNum)*bytes);


    uchar* textstream= malloc(allocate);

    for(int i=0; i<n ; i++){
        x=fgetc(text);
        if(x==EOF){
            printf("encrypted file reading error, Check encrpytion"); endl
            break;
        }
        else
            textstream[i]=x;
    }

    uchar* out= malloc(sizeof(uchar) * n);

    int k=0;
    for(int i=0;i<blockNum;i++){
        decrypt(textstream+ i*bytes,key,out+ i*bytes, encryption);
    }

    FILE *deciphered  = fopen("decrypted", "wb");

    pkcs7_remove_Pad(blockSize, deciphered , &out, n-1);

    fclose(text);
    fclose(deciphered);
    free(textstream);
    free(out);

}

void pkcs7_remove_Pad(int blockSize,FILE* decrypted,uchar** outstream , int size)
{
    int padded= (*outstream)[size];

    for(int i=0; i<size-padded; i++){
        fputc((*outstream)[i],decrypted);
    }
}




void cbc_encrypt( int blockSize, int* key, uchar* filename,  uchar* iv, uchar* encryption)
{
    int bytes=blockSize/8;

    FILE *text  = fopen(filename, "rb");

    if(!text){
        printf("Input file %s not found", filename); endl
        return;
    }

    uchar* textstream;

    long long blockNum= pkcs7_Pad( blockSize, text , &textstream);

    long long allocate= (blockNum*bytes);



    uchar* cipherTextStream= malloc(sizeof(uchar) * allocate);
    uchar* previous=malloc(sizeof(uchar) * bytes);
    for(int i=0;i<blockNum;i++){
        if(i==0){
            xor(textstream, iv, bytes);
        }
        else{
            xor(textstream+ i*bytes,previous, bytes);
        }
        encrypt(textstream+ i*bytes,key,cipherTextStream + i*bytes, encryption);
        memcpy(previous,(cipherTextStream+i*bytes),sizeof(uchar)*bytes);
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


void xor(uchar *y, uchar* x, int n){

    for(int i=0; i<n;i++){
        int tx=x[i], ty=y[i];
        y[i]= tx^ty;
    }

}


void cbc_decrypt( int blockSize, int* key, uchar* file,  uchar* iv, uchar* encryption)
{
    int bytes=blockSize/8;
    FILE *text  = fopen(file, "rb");

    if(!text){
        printf("File to decrypt %s not found", file); endl
        return;
    }


    long long n=0;
    uchar x;
    int i;

    while((i=fgetc(text))!=EOF){
        n++;
    }

    fseek(text, 0, SEEK_SET);


    long long blockNum= n/bytes;
    long long allocate= ((blockNum)*bytes);


    uchar* textstream= malloc(allocate);

    for(int i=0; i<n ; i++){
        x=fgetc(text);
        if(x==EOF){
            printf("encrypted file reading error, Check encrpytion"); endl
            break;
        }
        else
            textstream[i]=x;
    }

    uchar* out= malloc(sizeof(uchar) * n);

    uchar* previous=malloc(sizeof(uchar) * bytes);

    for(int i=0;i<blockNum;i++){
        
        decrypt(textstream+ i*bytes,key,out + i*bytes, encryption);
        if(i==0){
            xor(out+ i*bytes, iv, bytes);
        }
        else{
            xor(out+ i*bytes,previous,bytes);
        }
        
        memcpy(previous,textstream + i*bytes, sizeof(uchar)*bytes);
    }

    FILE *deciphered  = fopen("decrypted", "wb");

    pkcs7_remove_Pad(blockSize, deciphered , &out, n-1);



    fclose(text);
    fclose(deciphered);
    free(textstream);
    free(out);
}






void cfb_encrypt( int blockSize, int* key, uchar* filename,  uchar* iv, uchar* encryption)
{
    int bytes=blockSize/8;

    FILE *text  = fopen(filename, "rb");

    if(!text){
        printf("Input file %s not found", filename); endl
        return;
    }

    uchar output[bytes];
    uchar outText[bytes];

    memcpy(output, iv, sizeof(uchar)*bytes);

    FILE *encrypted  = fopen("encrypted", "wb");

    

    encrypt(output, key,outText,encryption);
    
    int i;
    uchar discard[bytes-1];
    uchar select;
    while((i=fgetc(text))!=EOF){
        select=outText[0];
        //memcpy(discard, outText[1],sizeof(uchar)*(bytes-1) );
        for(int i=1;i<bytes;i++) discard[i-1]=outText[i];
        select=select^i;
        fputc(select, encrypted);
        memcpy(outText,discard,sizeof(uchar)*(bytes-1) );
        outText[bytes-1]=select;
    }

    fclose(encrypted);
    fclose(text);

    
}



void cfb_decrypt( int blockSize, int* key, uchar* filename,  uchar* iv, uchar* encryption)
{
    int bytes=blockSize/8;

    FILE *ciphertext  = fopen(filename, "rb");

    if(!ciphertext){
        printf("Encryption file %s not found", filename); endl
        return;
    }

    uchar output[bytes];
    uchar outText[bytes];

    memcpy(output, iv, sizeof(uchar)*bytes);

    FILE *decrypted  = fopen("decrypted", "wb");

    

    encrypt(output, key,outText,encryption);
    
    
    int i;
    uchar discard[bytes-1];
    uchar select;
    while((i=fgetc(ciphertext))!=EOF){
        select=outText[0];
        //memcpy(discard, outText[1],sizeof(uchar)*(bytes-1) );
        for(int i=1;i<bytes;i++) discard[i-1]=outText[i];
        select=select^i;
        fputc(select, decrypted);
        memcpy(outText,discard,sizeof(uchar)*(bytes-1) );
        outText[bytes-1]=i;
    }

    fclose(decrypted);
    fclose(ciphertext);

}    
