#include <string.h>
#include <stdio.h>



void hextoBytes(char* hex, unsigned char* bytes) {

    int len=strlen(hex);
    for (int i=0, k=0; i< len; i+=2,k++) { 
        unsigned char char1=0,char2=0;
        char1= hex[i];
        if(i+1<strlen(hex))
            char2= hex[i+1];
        
        int value1=0,value2=0;
        if(char1>='0' && char1<='9'){
            value1=char1-'0';
        }
        else{
            value1=10+(char1-'a');
        }

        if(char2>='0' && char2<='9'){
            value2=char2-'0';
        }
        else{
            value2=10+(char2-'a');
        }

        bytes[k]=value1*16+value2;
    }
}

void hextoBits(const char* hex,int* bits) {

    int len= strlen(hex);
    for (int i=0; i< len; i++) { 
        unsigned char byte;
        byte= hex[i];
        int value;

        if(byte>='0' && byte<='9'){
            value=byte-'0';
        }
        else{
            value=10+(byte-'a');
        }

        for(int j=0;j<4;j++){
            bits[4*i+j]= (value >> (3-j))&1;
        }
    }
}


void bitstoHex(int* bits,char* hex, int numberBits) {

    int k=0;
    char temp[192];

    for(int i=numberBits-1; i>=0; i=i-4){

        int bit4=0,bit3=0,bit2=0,bit1=0;
        bit4=bits[i];
        if(i-1>=0) bit3=bits[i-1];
        if(i-2>=0) bit2=bits[i-2];
        if(i-3>=0) bit1=bits[i-3];

        int x=8*bit1+4*bit2+2*bit3+bit4;

        if(x>=0 && x<=9){
            temp[k]=x+'0';
        }
        else{
            temp[k]=x-10+'a';
        }
        k++;
    }

    for(int i=0;i<k;i++){
        hex[i]=temp[k-1-i];
    }
    hex[k]='\0';
}


void bytetoHex(unsigned char* bytes, char* hex, int byteNumber){

    int k=0;
    for(int i=0;i<byteNumber;i++){
        int h1, h2;
        h2=bytes[i]&0x0F; 
        h1=(bytes[i]>>4)&0x0F; 
        
        if(h1<=9){hex[k]=h1+'0';}
        else{ hex[k]=h1-10+'a';}
        k++;
        if(h2>=0 && h2<=9){hex[k]=h2+'0';}
        else{ hex[k]=h2-10+'a';}
        k++;
    }
    hex[k]='\0';

}


__uint128_t bytesTo128(unsigned char* bytes){
    __uint128_t integer=0;

    for(int i=0;i<16;i++){
        integer= (integer<<8)|(bytes[i]);
    }
    return integer;
}

void uint128ToBytes(__uint128_t integer, unsigned char* bytes) {
    for (int i = 15; i >= 0; i--) {
        bytes[i] = (unsigned char)(integer & 0xFF);
        integer >>= 8;
    }
}