#include "encryption.h"
#include "string.h"
#include "des.h"
#include "2des.h"
#include "3des.h"
#include "aes.h"
#include "modes.h"
#include "authenticationModes.h"
#include "conversions.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>


void writeInCSV(FILE* csv, char* algo, char* mode, char* oper, clock_t start,clock_t end, int size);

void encrypt(unsigned char* text, int* key, unsigned char* cipherText, char* encryptiontype)
{
    int type= getType(encryptiontype);

    switch(type){
        case 1:// des
        {   static int keysM[16][48];
            static int desStateEn=0;
            if(desStateEn==0){
                getKeys(key,keysM[0]);
                //AESencryptFaster(text,cipherText, AESkey, 10,expandedKey);
                DES_EncryptFaster(text, key, cipherText,keysM);
                desStateEn=1;
            }
            else{
                DES_EncryptFaster(text, key, cipherText,keysM);
            }
            break;
        }
        case 2:
        {
            int key1[64],key2[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            _2DES_Encrypt(text, key1,key2, cipherText);
            break;
        }
        case 3:
        {
            int key1[64],key2[64],key3[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            memcpy(key3, key+128, sizeof(int)*64);
            _3DES_Encrypt(text, key1,key2,key3, cipherText);
            break;
        }
        case 4:{
        
            static unsigned char AESkey[16];
            static int aesStateEn=0;
            static unsigned char expandedKey[176];

            if(aesStateEn==0){
                char hexKey[33];bitstoHex(key,hexKey,128);
                hextoBytes(hexKey,AESkey);

                //AESencrypt(text,cipherText, AESkey,10, expandedKey);
                keyExpansion(AESkey,expandedKey);
                aesStateEn=1;
                AESencryptFaster(text,cipherText, AESkey, 10,expandedKey);
            }
            else{
                AESencryptFaster(text,cipherText, AESkey, 10,expandedKey);
            }
            break;
        }

    }
}



void decrypt(unsigned char* cipherText, int* key, unsigned char* resultText,  char* encryptiontype)
{
    int type=getType(encryptiontype);

    switch(type){
        case 1:
            static int dkeysM[16][48];
            static int desStateDec=0;
            if(desStateDec==0){
                getKeys(key,dkeysM[0]);
                //AESencryptFaster(text,cipherText, AESkey, 10,expandedKey);
                //DES_DecryptFaster(cipherText, key, cipherText,dkeysM);
                DES_DecryptFaster(cipherText, key, resultText, dkeysM);
                desStateDec=1;
            }
            else{
                //DES_DecryptFaster(cipherText, key, cipherText,dkeysM);
                DES_DecryptFaster(cipherText, key, resultText, dkeysM);
            }
            break;
        case 2:
        {
            int key1[64], key2[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            _2DES_Decrypt(cipherText, key1, key2, resultText);
            break;
        }
        case 3:
        {
            int key1[64],key2[64],key3[64];
            memcpy(key1, key, sizeof(int)*64);
            memcpy(key2, key+64, sizeof(int)*64);
            memcpy(key3, key+128, sizeof(int)*64);
            _3DES_Decrypt(cipherText, key1,key2,key3, resultText);
            break;
        }
        case 4:{
            static unsigned char AESkeyd[16];
            static int aesStateDec=0;
            static unsigned char expandedKeyDec[176];

            if(aesStateDec==0){
                char hexKey[33];bitstoHex(key,hexKey,128);
                hextoBytes(hexKey,AESkeyd);

                //AESencrypt(text,cipherText, AESkey,10, expandedKey);
                keyExpansion(AESkeyd,expandedKeyDec);
                aesStateDec=1;
                AESdecryptFaster(cipherText,resultText, AESkeyd, 10,expandedKeyDec);
            }
            else{
                AESdecryptFaster(cipherText,resultText, AESkeyd, 10,expandedKeyDec);
            }
            break;


        }

    }
}



int getType( char* x){
    if(x[0]=='D'||x[0]=='d'){
        return 1;
    }
    else if(x[0]=='2'||x[0]=='t') return 2;
    else if(x[0]=='3') return 3;
    else if (x[0]=='a'||x[0]=='A') return 4;
    else {
        printf("INVALID ENCRYPTION TYPE \n\n");
        exit(1);
        //return 0;
    }
}


char * encNumber(int i){
    if(i==1) return "des";
    if(i==2) return "2des";
    if(i==3) return "3des";
    if(i==4) return "aes";

}

int getBlockSize(int i){
    if(i==4) return 128;
    else return 64;
}

long long countBytes(char* inFilename){
    FILE* f= fopen(inFilename, "rb");
    // int x,n=0;
    // while((x=fgetc(f))!=EOF){
    //     n++;
    // }
    fseek(f,0,SEEK_END);
    long long n=ftell(f);
    fclose(f);
    return n;
}

long long countBytesFile(FILE* x){
    //FILE* f= fopen(inFilename, "rb");
    // int x,n=0;
    // while((x=fgetc(f))!=EOF){
    //     n++;
    // }
    fseek(x,0,SEEK_END);
    long long n=ftell(x);
    fseek(x,0,SEEK_SET);
    return n;
}

void benchmark(int* key, unsigned char* iv, char* inFilename, char* outFilename) {


    FILE* csv= fopen(outFilename, "wb");

    char* headers="Algo,Mode,Operation,Time(Sec),Throughput(MB/sec)\n";
    fwrite(headers, sizeof(char), strlen(headers), csv);

    int size=countBytes(inFilename);
    if(size==0){
        printf("Input file empty"); exit(1);
    }
    double time=0;

    clock_t start, end;
    
    //ecb 

    // //des
    // start=clock();
    // ecb_encrypt(64,key,inFilename, "output", "des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","ECB","Encrypt", start,end, size);

    // start=clock();
    // ecb_decrypt(64,key,"output", "del", "des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","ECB","Decrypt", start,end, size);

    // //2des
    // start=clock();
    // ecb_encrypt(64,key,inFilename, "output", "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","ECB","Encrypt", start, end, size);

    // start=clock();
    // ecb_decrypt(64,key,"output", "del", "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","ECB","Decrypt", start, end, size);

    // //3des
    // start=clock();
    // ecb_encrypt(64,key,inFilename, "output", "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","ECB","Encrypt", start, end, size);

    // start=clock();
    // ecb_decrypt(64,key,"output", "del", "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","ECB","Decrypt", start, end, size);
    
    
    // //AES
    // start=clock();
    // ecb_encrypt(128,key,inFilename, "output", "aes");
    // end=clock();
    // writeInCSV(csv,"AES","ECB","Encrypt", start, end, size);

    // start=clock();
    // ecb_decrypt(128,key,"output", "del", "aes");
    // end=clock();
    // writeInCSV(csv,"AES","ECB","Decrypt", start, end, size);



    // //cbc

    // //des
    // start=clock();
    // cbc_encrypt(64,key,inFilename, "output", iv,"des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","CBC","Encrypt", start,end, size);

    // start=clock();
    // cbc_decrypt(64,key,"output","del",iv, "des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","CBC","Decrypt", start,end, size);

    // //2des

    // start=clock();
    // cbc_encrypt(64,key,inFilename, "output",iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","CBC","Encrypt", start, end, size);

    // start=clock();
    // cbc_decrypt(64,key, "output","del", iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","CBC","Decrypt", start, end, size);

    // //3des
    // start=clock();
    // cbc_encrypt(64,key,inFilename, "output",iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","CBC","Encrypt", start, end, size);

    // start=clock();
    // cbc_decrypt(64,key,"output","del",iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","CBC","Decrypt", start, end, size);

    // //AES
    // start=clock();
    // cbc_encrypt(128,key,inFilename, "output",iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","CBC","Encrypt", start, end, size);

    // start=clock();
    // cbc_decrypt(128,key,"output","del", iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","CBC","Decrypt", start, end, size);

    //     //des
    // start=clock();
    // cfb_encrypt(64,key,inFilename, "output", iv,"des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","CFB","Encrypt", start,end, size);

    // start=clock();
    // cfb_decrypt(64,key,"output","del",iv, "des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","CFB","Decrypt", start,end, size);

    // //2des

    // start=clock();
    // cfb_encrypt(64,key,inFilename, "output",iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","CFB","Encrypt", start, end, size);

    // start=clock();
    // cfb_decrypt(64,key, "output","del", iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","CFB","Decrypt", start, end, size);

    // //3des
    // start=clock();
    // cfb_encrypt(64,key,inFilename, "output",iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","CFB","Encrypt", start, end, size);

    // start=clock();
    // cfb_decrypt(64,key, "output","del", iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","CFB","Decrypt", start, end, size);

    // //AES
    // start=clock();
    // cfb_encrypt(128,key,inFilename, "output",iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","CFB","Encrypt", start, end, size);

    // start=clock();
    // cfb_decrypt(128,key,"output","del", iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","CFB","Decrypt", start, end, size);


    //     //des
    // start=clock();
    // ofb_encrypt(64,key,inFilename, "output", iv,"des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","OFB","Encrypt", start,end, size);

    // start=clock();
    // ofb_decrypt(64,key,"output","del",iv, "des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","OFB","Decrypt", start,end, size);

    // //2des

    // start=clock();
    // ofb_encrypt(64,key,inFilename, "output",iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","OFB","Encrypt", start, end, size);

    // start=clock();
    // ofb_decrypt(64,key, "output","del", iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","OFB","Decrypt", start, end, size);

    // //3des
    // start=clock();
    // ofb_encrypt(64,key,inFilename, "output",iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","OFB","Encrypt", start, end, size);

    // start=clock();
    // ofb_decrypt(64,key, "output","del",iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","OFB","Decrypt", start, end, size);

    // //AES
    // start=clock();
    // ofb_encrypt(128,key,inFilename, "output",iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","OFB","Encrypt", start, end, size);

    // start=clock();
    // ofb_decrypt(128,key,"output","del", iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","OFB","Decrypt", start, end, size);

    // //des
    // start=clock();
    // counter_encrypt(64,key,inFilename, "output", iv,"des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","Counter","Encrypt", start,end, size);

    // start=clock();
    // counter_decrypt(64,key,"output","del",iv, "des");
    // end=clock();
    // time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    // writeInCSV(csv,"DES","Counter","Decrypt", start,end, size);

    // //2des

    // start=clock();
    // counter_encrypt(64,key,inFilename, "output",iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","Counter","Encrypt", start, end, size);

    // start=clock();
    // counter_decrypt(64,key, "output","del", iv, "2des");
    // end=clock();
    // writeInCSV(csv,"2DES","Counter","Decrypt", start, end, size);

    // //3des
    // start=clock();
    // counter_encrypt(64,key,inFilename, "output",iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","Counter","Encrypt", start, end, size);

    // start=clock();
    // counter_decrypt(64,key, "output","del",iv, "3des");
    // end=clock();
    // writeInCSV(csv,"3DES","Counter","Decrypt", start, end, size);

    // //AES
    // start=clock();
    // counter_encrypt(128,key,inFilename, "output",iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","Counter","Encrypt", start, end, size);

    // start=clock();
    // counter_decrypt(128,key,"output","del", iv, "aes");
    // end=clock();
    // writeInCSV(csv,"AES","Counter","Decrypt", start, end, size);


    // //gcm
    // char tag[128];
    // start=clock();
    // gcm_encrypt(key,inFilename, iv, "output",tag);
    // end=clock();
    // writeInCSV(csv,"AES","GCM","Encrypt", start, end, size);

    // start=clock();
    // gcm_decrypt(key, "output",iv, "del",tag);
    // end=clock();
    // writeInCSV(csv,"AES","GCM","Decrypt", start, end, size);

        //des
    start=clock();
    ecb_encrypt(64,key,inFilename, "output", "des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","ECB","Encrypt", start,end, size);

    start=clock();
    ecb_decrypt(64,key,"output", "ecb-des", "des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","ECB","Decrypt", start,end, size);

    //2des
    start=clock();
    ecb_encrypt(64,key,inFilename, "output", "2des");
    end=clock();
    writeInCSV(csv,"2DES","ECB","Encrypt", start, end, size);

    start=clock();
    ecb_decrypt(64,key,"output", "ecb-2des", "2des");
    end=clock();
    writeInCSV(csv,"2DES","ECB","Decrypt", start, end, size);

    //3des
    start=clock();
    ecb_encrypt(64,key,inFilename, "output", "3des");
    end=clock();
    writeInCSV(csv,"3DES","ECB","Encrypt", start, end, size);

    start=clock();
    ecb_decrypt(64,key,"output", "ecb-3des", "3des");
    end=clock();
    writeInCSV(csv,"3DES","ECB","Decrypt", start, end, size);
    
    
    //AES
    start=clock();
    ecb_encrypt(128,key,inFilename, "output", "aes");
    end=clock();
    writeInCSV(csv,"AES","ECB","Encrypt", start, end, size);

    start=clock();
    ecb_decrypt(128,key,"output", "ecb-aes", "aes");
    end=clock();
    writeInCSV(csv,"AES","ECB","Decrypt", start, end, size);



    //cbc

    //des
    start=clock();
    cbc_encrypt(64,key,inFilename, "output", iv,"des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","CBC","Encrypt", start,end, size);

    start=clock();
    cbc_decrypt(64,key,"output","cbc-des",iv, "des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","CBC","Decrypt", start,end, size);

    //2des

    start=clock();
    cbc_encrypt(64,key,inFilename, "output",iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","CBC","Encrypt", start, end, size);

    start=clock();
    cbc_decrypt(64,key, "output","cbc-2des", iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","CBC","Decrypt", start, end, size);

    //3des
    start=clock();
    cbc_encrypt(64,key,inFilename, "output",iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","CBC","Encrypt", start, end, size);

    start=clock();
    cbc_decrypt(64,key,"output","cbc-3des",iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","CBC","Decrypt", start, end, size);

    //AES
    start=clock();
    cbc_encrypt(128,key,inFilename, "output",iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","CBC","Encrypt", start, end, size);

    start=clock();
    cbc_decrypt(128,key,"output","cbc-aes", iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","CBC","Decrypt", start, end, size);

        //des
    start=clock();
    cfb_encrypt(64,key,inFilename, "output", iv,"des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","CFB","Encrypt", start,end, size);

    start=clock();
    cfb_decrypt(64,key,"output","cfb-des",iv, "des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","CFB","Decrypt", start,end, size);

    //2des

    start=clock();
    cfb_encrypt(64,key,inFilename, "output",iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","CFB","Encrypt", start, end, size);

    start=clock();
    cfb_decrypt(64,key, "output","cfb-2des", iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","CFB","Decrypt", start, end, size);

    //3des
    start=clock();
    cfb_encrypt(64,key,inFilename, "output",iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","CFB","Encrypt", start, end, size);

    start=clock();
    cfb_decrypt(64,key, "output","cfb-3des", iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","CFB","Decrypt", start, end, size);

    //AES
    start=clock();
    cfb_encrypt(128,key,inFilename, "output",iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","CFB","Encrypt", start, end, size);

    start=clock();
    cfb_decrypt(128,key,"output","cfb-aes", iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","CFB","Decrypt", start, end, size);


        //des
    start=clock();
    ofb_encrypt(64,key,inFilename, "output", iv,"des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","OFB","Encrypt", start,end, size);

    start=clock();
    ofb_decrypt(64,key,"output","ofb-des",iv, "des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","OFB","Decrypt", start,end, size);

    //2des

    start=clock();
    ofb_encrypt(64,key,inFilename, "output",iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","OFB","Encrypt", start, end, size);

    start=clock();
    ofb_decrypt(64,key, "output","ofb-2des", iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","OFB","Decrypt", start, end, size);

    //3des
    start=clock();
    ofb_encrypt(64,key,inFilename, "output",iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","OFB","Encrypt", start, end, size);

    start=clock();
    ofb_decrypt(64,key, "output","ofb-3des",iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","OFB","Decrypt", start, end, size);

    //AES
    start=clock();
    ofb_encrypt(128,key,inFilename, "output",iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","OFB","Encrypt", start, end, size);

    start=clock();
    ofb_decrypt(128,key,"output","ofb-aes", iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","OFB","Decrypt", start, end, size);

    //des
    start=clock();
    counter_encrypt(64,key,inFilename, "output", iv,"des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","Counter","Encrypt", start,end, size);

    start=clock();
    counter_decrypt(64,key,"output","counter-des",iv, "des");
    end=clock();
    time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    writeInCSV(csv,"DES","Counter","Decrypt", start,end, size);

    //2des

    start=clock();
    counter_encrypt(64,key,inFilename, "output",iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","Counter","Encrypt", start, end, size);

    start=clock();
    counter_decrypt(64,key, "output","counter-2des", iv, "2des");
    end=clock();
    writeInCSV(csv,"2DES","Counter","Decrypt", start, end, size);

    //3des
    start=clock();
    counter_encrypt(64,key,inFilename, "output",iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","Counter","Encrypt", start, end, size);

    start=clock();
    counter_decrypt(64,key, "output","counter-3des",iv, "3des");
    end=clock();
    writeInCSV(csv,"3DES","Counter","Decrypt", start, end, size);

    //AES
    start=clock();
    counter_encrypt(128,key,inFilename, "output",iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","Counter","Encrypt", start, end, size);

    start=clock();
    counter_decrypt(128,key,"output","counter-aes", iv, "aes");
    end=clock();
    writeInCSV(csv,"AES","Counter","Decrypt", start, end, size);


    //gcm
    char tag[128];
    start=clock();
    gcm_encrypt(key,inFilename, iv, "output",tag);
    end=clock();
    writeInCSV(csv,"AES","GCM","Encrypt", start, end, size);

    start=clock();
    gcm_decrypt(key, "output",iv, "gcm-aes",tag);
    end=clock();
    writeInCSV(csv,"AES","GCM","Decrypt", start, end, size);


    fclose(csv);
    remove("output");
    remove("delete");
}


void writeInCSV(FILE* csv, char* algo, char* mode, char* oper, clock_t start,clock_t end, int size){
    double time= (double)(end-start)/(double)(CLOCKS_PER_SEC);
    fprintf(csv,"%s,%s,%s,%f,%f\n",algo, mode, oper, time, size/(time*1000*1000));
    printf("%s %s %s took %f sec at %f MB/s\n", algo, mode, oper, time, size/(time*1000*1000));
}