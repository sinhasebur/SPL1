#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

void findMaxSumInterval(const vector<int>& arr, int &maxSum, int &startIdx, int &endIdx) {
    maxSum = INT_MIN;
    int currentMax = 0;
    int s = 0;

    startIdx = -1;
    endIdx = -1;

    for (int i = 0; i < arr.size(); i++) {
        currentMax += arr[i];

        if (currentMax > maxSum) {
            maxSum = currentMax;
            startIdx = s;
            endIdx = i;
        }

        if (currentMax < 0) {
            currentMax = 0;
            s = i + 1;
        }
    }
}


int main(){

    while(1){
        int n; cin>>n;
        if(n==0) break;
        vector <int> nums;
        for(int i=0;i<n;i++){
            int x; cin>>x;
            nums.push_back(x);
        }
        int ans, startid, endid;
        findMaxSumInterval(nums,ans,startid,endid);

        if(ans <=0){
            cout<<"Losing streak."<<endl; 
        }
        else{
            cout<<"The maximum winning streak is "<<ans<<"."<<endl;
        }
        vector <int> empt;
        nums=empt;
    }
    
}