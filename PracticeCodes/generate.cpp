#include <bits/stdc++.h>

using namespace std;

int main(){
    string x;
    cin>>x;
    for(int i=0;i<16;i++){
        if(x[i]=='0') x[i]='1';
        else if(x[i]=='1') x[i]='0';
        else{}
        
        cout<<"{"<<x<<"}"<<endl; 
       
    }
}