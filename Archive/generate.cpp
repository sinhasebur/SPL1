#include <bits/stdc++.h>

using namespace std;

int main(){

    cout<<"{";
    int x;
    int w=9912313;
    for(int i=0;i<64;i++){
        x=(w%(i+3))%2;
        
        cout<<x<<",";    
    }

    
}