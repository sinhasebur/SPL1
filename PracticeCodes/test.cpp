#include <bits/stdc++.h>

using namespace std;

int main(){
    FILE *f1  = fopen("input", "rb");
    FILE *f2  = fopen("encrypted", "rb");

    int i,j;
    long long k=0;

    while( ((j=fgetc(f2))!=EOF) && ((i=fgetc(f1))!=EOF)  ){
        k++;
        // if(i==j)cout<<"."<<endl;
        // else{
        //     cout<<"sad at byte"<<k<<endl;
        // }
    }

    if((i=fgetc(f1))!=EOF){
    //     cout<<"f the main had bytes left"<<endl;
        cout<<"f the main had bytes left"<<endl;
    }

    if((j=fgetc(f2))!=EOF){
    //     cout<<"f the decrypted had bytes left"<<endl;
        cout<<"f the encrypt had bytes left"<<endl;
    
    }

    fclose(f1);
    fclose(f2);
}