#include <stdio.h>


#define endl printf("\n");

void fill_IntArray(int size, int* array, int reducedKeySpace);
int random01();
void toCharArray(int charArraySize, int* array, unsigned char* charArray);

__uint64_t random_state =1234;




int main(){
    int k1[64];
    int k2[64];

    fill_IntArray(64, k1, 16);
    random_state++;
    fill_IntArray(64, k2, 16);

    unsigned char ck1[8], ck2[8];
    toCharArray(8, k1, ck1);
    toCharArray(8, k2, ck2);

    endl printf("{");
    for(int i=0;i<8;i++) printf("%x,",ck1[i]);
    
    endl printf("{");
    for(int i=0;i<8;i++) printf("%x,",ck2[i]);


    endl printf("{");
    for(int i=0;i<63;i++) printf("%d ,",k1[i]);
    printf("%d }",k1[63]); endl



    endl printf("{");
    for(int i=0;i<63;i++) printf("%d ,",k2[i]);
    printf("%d }",k2[63]); endl



}


void fill_IntArray(int size, int* array, int reducedKeySpace){

    for(int i=0;i<size;i++){
        if(i<reducedKeySpace){
            array[i]=random01();
        }
        else{
            array[i]=0;
        }
    }
}


void toCharArray(int size, int* x, unsigned  char* y){

    int k=0;
    for(int i=0;i<size;i++){
        int temp=0;
        for(int j=0;j<8;j++,k++){

            if(x[k]==1)
                temp+= (1)<<j;
        }
        y[i]=(unsigned char)temp;
    }
}


int random01(){
    __uint64_t x = random_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    random_state = x;

    return x&1;
}