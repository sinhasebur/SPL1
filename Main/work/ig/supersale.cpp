#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


vector<int> val ;
vector<int> wt ;

int dp[10000][35];


int knapsack(int ind, int wt_left) {
    if(wt_left == 0 || ind < 0) return 0;
    
    if(dp[ind][wt_left] != -1) return dp[ind][wt_left];
    
    // Don't take current item
    int ans = knapsack(ind-1, wt_left);
    
    // Take current item if possible
    if(wt_left >= wt[ind]) {
        ans = max(ans, knapsack(ind-1, wt_left - wt[ind]) + val[ind]);
    }
    
    return dp[ind][wt_left] = ans;
}

int main(){

    memset(dp,-1,sizeof(dp));

   //int w = 4;

   int t; cin>>t;

   while(t--){
        int n; cin>>n;
        for(int i=0;i<n;i++){
            int x, y; cin>>x>>y;
            val.push_back(x);
            wt.push_back(y);
        }
        int g; cin>>g;
        int sum=0;
        for(int i=0;i<g;i++){
            int x; cin>>x;
            sum+=knapsack(val.size()-1,x);
        }
        cout<<sum<<endl;
        vector<int> emp;
        val=emp;
        wt=emp;
        memset(dp,-1,sizeof(dp));
        
   }
    
}