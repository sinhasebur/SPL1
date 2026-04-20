#include <stdio.h>
#include <string.h>
#include "modes.h"
#include "encryption.h"
#include <stdlib.h>
#include <stdint.h>

#define endl printf("\n");



int pkcs7_Pad(int blockSize,FILE* text , unsigned char** textstream)
{
    int bytes=blockSize/8;
    long long n=0;
    unsigned char x;

    n=countBytesFile(text);
    

    long long blockNum= n/bytes;
    long long allocate=((blockNum+1)*bytes);
    int excess=n%bytes;

    (*textstream)= malloc(allocate);

    fread(*textstream,sizeof(char),n,text);

    int padNumber=bytes-excess;
    if(excess==0) padNumber=bytes;

    for(int i=0;i<padNumber;i++){
        (*textstream)[n+i]=padNumber;
    }

    blockNum++;
    return blockNum;
}


void pkcs7_remove_Pad(int blockSize,FILE* decrypted,unsigned char** outstream , int size)
{
    int padNumber= (*outstream)[size];

    int bytes=blockSize/8;

    if (padNumber <= 0 || padNumber >bytes ) {
        padNumber= 0;
        printf("Padding error\n");endl
        return;
    }
    else{
        for(int i=size,j=0;j<padNumber;i--,j++){
            if((*outstream)[i]!=padNumber){
                printf("Pad not aligned");endl
                return;
            }
        }
    }

    // for(int i=0; i<size-padded; i++){
    //     fputc((*outstream)[i],decrypted);
    // }
    fwrite(*outstream, sizeof(char),size-padNumber, decrypted);
}




void ecb_encrypt(int blockSize,  int* key, char* infilename, char* outfilename, char* encryptionType)
{

    int bytes=blockSize/8;

    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("Input file %s not found", infilename); endl
        return;
    }

    unsigned char* textstream;

    long long blockNum= pkcs7_Pad( blockSize, text , &textstream);

    long long allocate= (blockNum*bytes);


    unsigned char* cipherTextStream= malloc(sizeof(unsigned char) * allocate);
    for(int i=0;i<blockNum;i++){
        encrypt(textstream+ i*bytes,key,cipherTextStream + i*bytes, encryptionType);
    }


    FILE *cipher  = fopen(outfilename, "wb");

    // for(int i=0; i<allocate ; i++){
    //     fputc(cipherTextStream[i],cipher);
    // }
    fwrite(cipherTextStream, sizeof(char),allocate, cipher);

    fclose(text);
    fclose(cipher);
    free(cipherTextStream);
    free(textstream);

}


void ecb_decrypt(int blockSize, int* key, char* infilename , char* outfilename ,char* encryptionType )
{
    int bytes=blockSize/8;
    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("File to decrypt %s not found", infilename); endl
        return;
    }


    long long n=0;
    unsigned char x;
    
    n=countBytes(infilename);


    long long blockNum= n/bytes;
    long long allocate= ((blockNum)*bytes);


    unsigned char* textstream= malloc(allocate);

    // for(int i=0; i<n ; i++){
    //     x=fgetc(text);
    //     if(x==EOF){
    //         printf("encrypted file reading error, Check encrpytion"); endl
    //         break;
    //     }
    //     else
    //         textstream[i]=x;
    // }

    fread(textstream,sizeof(char),n, text);

    unsigned char* out= malloc(sizeof(unsigned char) * n);

    int k=0;
    for(int i=0;i<blockNum;i++){
        decrypt(textstream+ i*bytes,key,out+ i*bytes, encryptionType);
    }

    FILE *deciphered  = fopen(outfilename, "wb");

    pkcs7_remove_Pad(blockSize, deciphered , &out, n-1);

    fclose(text);
    fclose(deciphered);
    free(textstream);
    free(out);

}





void cbc_encrypt( int blockSize, int* key, char* infilename,char* outfilename,  unsigned char* iv, char* encryptionType)
{
    int bytes=blockSize/8;
    
    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("Input file %s not found", infilename); endl
        return;
    }

    unsigned char* textstream;

    long long blockNum= pkcs7_Pad( blockSize, text , &textstream);

    long long allocate= (blockNum*bytes);



    unsigned char* cipherTextStream= malloc(sizeof(unsigned char) * allocate);
    unsigned char* previous=malloc(sizeof(unsigned char) * bytes);
    for(int i=0;i<blockNum;i++){
        if(i==0){
            Xor(textstream, iv, bytes);
        }
        else{
            Xor(textstream+ i*bytes,previous, bytes);
        }
        encrypt(textstream+ i*bytes,key,cipherTextStream + i*bytes, encryptionType);
        memcpy(previous,(cipherTextStream+i*bytes),sizeof(unsigned char)*bytes);
    }


    FILE *cipher  = fopen(outfilename, "wb");

    for(int i=0; i<allocate ; i++){
        fputc(cipherTextStream[i],cipher);
    }

    fclose(text);
    fclose(cipher);
    free(cipherTextStream);
    free(textstream);

}



void cbc_decrypt( int blockSize, int* key, char* infilename,char* outfilename,  unsigned char* iv, char* encryptionType)
{
    int bytes=blockSize/8;
    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("File to decrypt %s not found", infilename); endl
        return;
    }


    long long n=0;
    unsigned char x;
    int i;

    while((i=fgetc(text))!=EOF){
        n++;
    }

    fseek(text, 0, SEEK_SET);


    long long blockNum= n/bytes;
    long long allocate= ((blockNum)*bytes);


    unsigned char* textstream= malloc(allocate);

    for(int i=0; i<n ; i++){
        x=fgetc(text);
        if(x==EOF){
            printf("encrypted file reading error, Check encrpytion"); endl
            break;
        }
        else
            textstream[i]=x;
    }

    unsigned char* out= malloc(sizeof(unsigned char) * n);

    unsigned char* previous=malloc(sizeof(unsigned char) * bytes);

    for(int i=0;i<blockNum;i++){
        unsigned char currentCipherBlock[16]; 
        memcpy(currentCipherBlock, textstream + i*bytes, bytes);
        
        decrypt(textstream+ i*bytes,key,out + i*bytes, encryptionType);
        if(i==0){
            Xor(out+ i*bytes, iv, bytes);
        }
        else{
            Xor(out+ i*bytes,previous,bytes);
        }
        
        memcpy(previous,currentCipherBlock, sizeof(unsigned char)*bytes);
    }

    FILE *deciphered  = fopen(outfilename, "wb");

    pkcs7_remove_Pad(blockSize, deciphered , &out, n-1);



    fclose(text);
    fclose(deciphered);
    free(textstream);
    free(out);
}









void cfb_encrypt( int blockSize, int* key, char* infilename,char* outfilename,  unsigned char* iv, char* encryptionType)
{
    int bytes=blockSize/8;

    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("Input file %s not found", infilename); endl
        return;
    }

    unsigned char output[bytes];
    unsigned char outText[bytes];

    memcpy(output, iv, sizeof(unsigned char)*bytes);

    FILE *encrypted  = fopen(outfilename, "wb");

    encrypt(output, key,outText,encryptionType);
    
    int i;
    unsigned char discard[bytes-1];
    unsigned char select;
    while((i=fgetc(text))!=EOF){
        encrypt(outText, key, outText, encryptionType);
        select=outText[0];
        //memcpy(discard, outText[1],sizeof(unsigned char)*(bytes-1) );
        for(int i=1;i<bytes;i++) discard[i-1]=outText[i];
        select=select^i;
        fputc(select, encrypted);
        memcpy(outText,discard,sizeof(unsigned char)*(bytes-1) );
        outText[bytes-1]=select;
    }

    fclose(encrypted);
    fclose(text);

    
}


void cfb_decrypt( int blockSize, int* key, char* infilename,char* outfilename,  unsigned char* iv, char* encryptionType)
{
    int bytes=blockSize/8;

    FILE *ciphertext  = fopen(infilename, "rb");

    if(!ciphertext){
        printf("encryptionType file %s not found", infilename); endl
        return;
    }

    unsigned char output[bytes];
    unsigned char outText[bytes];

    memcpy(output, iv, sizeof(unsigned char)*bytes);

    FILE *decrypted  = fopen(outfilename, "wb");

    

    encrypt(output, key,outText,encryptionType);
    
    
    int i;
    unsigned char discard[bytes-1];
    unsigned char select;
    while((i=fgetc(ciphertext))!=EOF){
        encrypt(outText, key, outText, encryptionType);
        select=outText[0];
        //memcpy(discard, outText[1],sizeof(unsigned char)*(bytes-1) );
        for(int i=1;i<bytes;i++) discard[i-1]=outText[i];
        select=select^i;
        fputc(select, decrypted);
        memcpy(outText,discard,sizeof(unsigned char)*(bytes-1) );
        outText[bytes-1]=i;
    }

    fclose(decrypted);
    fclose(ciphertext);

}    







void ofb_encrypt( int blockSize, int* key, char* infilename, char* outfilename,  unsigned char* iv, char* encryptionType)
{   
    unsigned char nonce[16];
    if(blockSize==64){
        memcpy(nonce, iv, sizeof(char)*8);
    }
    else{
        memcpy(nonce, iv, sizeof(char)*16);
    }
    int bytes=blockSize/8;

    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("Input file %s not found", infilename); endl
        return;
    }

    unsigned char enc_part [bytes];
    unsigned char select [bytes];

    for(int i=0;i<bytes;i++) select[i]=0;
    encrypt(nonce,key, enc_part, encryptionType);
    
    unsigned char count[bytes];

    FILE *encrypted  = fopen(outfilename, "wb");

    int i,k=0;
    int loop=1;
    while(loop){
        k=0;
      
        while( k<bytes){
            if((i=fgetc(text))!=EOF){
                count[k]=i;
            }
            else {loop=0; break;}
            k++;
        }  

        for(int i=0;i<k;i++){
            select[i]=count[i];
        }

        Xor(select, enc_part,k);

        unsigned char temp [bytes];
        encrypt(enc_part, key, temp, encryptionType);
        memcpy(enc_part,temp, sizeof(unsigned char)*bytes);
        
        for(int i=0;i<k;i++){
            fputc(select[i],encrypted);
        }
    
    }
         
    fclose(text);
    fclose(encrypted);
   
}


void ofb_decrypt( int blockSize, int* key, char* infilename,char* outfilename,  unsigned char* nonce, char* encryptionType)
{
    int bytes=blockSize/8;

    FILE *ciphertext  = fopen(infilename, "rb");

    if(!ciphertext){
        printf("Encrypted file %s not found", infilename); endl
        return;
    }

    unsigned char enc_part [bytes];
    unsigned char select [bytes];

    for(int i=0;i<bytes;i++) select[i]=0;
    encrypt(nonce,key, enc_part, encryptionType);
    
    unsigned char count[bytes];

    FILE *decrypted  = fopen(outfilename, "wb");

    int i,k=0;
    int loop=1;
    while(loop){
        k=0;
      
        while( k<bytes){
            if((i=fgetc(ciphertext))!=EOF){
                count[k]=i;
            }
            else {loop=0; break;}
            k++;
        }  

        for(int i=0;i<k;i++){
            select[i]=count[i];
        }

        Xor(select, enc_part,k);
        

        unsigned char temp [bytes];
        encrypt(enc_part, key, temp, encryptionType);
        memcpy(enc_part,temp, sizeof(unsigned char)*bytes);
        
        for(int i=0;i<k;i++){
            fputc(select[i],decrypted);
        }
    
    }
         
    fclose(ciphertext);
    fclose(decrypted);
   
}


void counter_encrypt( int blockSize, int* key, char* infilename, char* outfilename,  unsigned char* iv, char* encryptionType)
{   
    unsigned char nonce[16];
    if(blockSize==64){
        memcpy(nonce, iv, sizeof(char)*8);
    }
    else{
        memcpy(nonce, iv, sizeof(char)*16);
    }
    int bytes=blockSize/8;

    FILE *text  = fopen(infilename, "rb");

    if(!text){
        printf("Input file %s not found", infilename); endl
        return;
    }

    unsigned char keystream [bytes];
    unsigned char plainText [bytes];
    long long counter=0;

    for(int i=0;i<bytes;i++) plainText[i]=0;
    encrypt(nonce,key, keystream, encryptionType);
    
    unsigned char count[bytes];

    FILE *encrypted  = fopen(outfilename, "wb");

    int i,k=0;
    int loop=1;

    while(loop){
        k=0;
      
        while( k<bytes){
            if((i=fgetc(text))!=EOF){
                count[k]=i;
            }
            else {loop=0; break;}
            k++;
        }  

        for(int i=0;i<k;i++){
            plainText[i]=count[i];
        }

        unsigned char tempIV [bytes];
        unsigned char toXor [bytes];
        memcpy(tempIV,nonce,bytes);
        
        for (int j=0;j<8;j++) {
            tempIV[bytes-1-j] ^=(unsigned char) ((counter>>(j*8))&0xFF);
        }
        
        encrypt(tempIV, key, toXor, encryptionType);
        
        Xor(plainText, toXor,k);
            
        
        counter++;
        
        
        for(int i=0;i<k;i++){
            fputc(plainText[i],encrypted);
        }
    
    }
         
    fclose(text);
    fclose(encrypted);
   
}

void counter_decrypt( int blockSize, int* key, char* infilename,char* outfilename,  unsigned char* iv, char* encryptionType)
{   
    counter_encrypt(blockSize, key,infilename, outfilename, iv, encryptionType);
   
}


void Xor(unsigned char *output, unsigned char* other, int n){

    for(int i=0; i<n;i++){
        int tx=other[i], ty=output[i];
        output[i]= tx^ty;
    }

}

