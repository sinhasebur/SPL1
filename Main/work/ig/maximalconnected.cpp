#include <bits/stdc++.h>
using namespace std;

#define ll long long

vector<ll> dfs(vector<vector<ll>>& adj, vector<ll>& visited, int x){
    ll n = adj.size();
    vector<ll> res;
    stack<ll> st;
    st.push(x);


    ///in dfs we use stack and mark when pop from stack
    //in bfs we use queue and mark when push
    while(!st.empty()){
        ll node = st.top();
        st.pop();

        if(visited[node]) continue;

        visited[node] = 1;
        res.push_back(node);

        for(ll i = adj[node].size() - 1; i >= 0; i--){
            ll v = adj[node][i];
            if(!visited[v]) st.push(v);
        }
    }
    return res;
}



int main(){
    int t; cin>>t;
    char nc; cin>>nc;
    int n=-1;
    int m;
    int first=0;
    while(t--){
        if(n==-1)
            n=nc-'A'+1;
        else{
            n=m;
        }
        vector <vector<ll>> adjList(n);
        
        string s;
        while(cin>>s){
            //cout<<s.size()<<endl;
            if(s.size()==1){
                m=s[0]-'A'+1;
                break;
            }
            else{
                adjList[s[0]-'A'].push_back(s[1]-'A');
                adjList[s[1]-'A'].push_back(s[0]-'A');
            }
        }

        vector <ll> visited(n,0);

        int ans=0;
        for(int i=0;i<n;i++){
            if(!visited[i]){
                dfs(adjList, visited,i);
                ans++;
            }
        }

        if(!first){
            cout<<ans<<endl;
            first=1;
        }
            
        else{
            cout<<endl<<ans<<endl;
        }
        
    }
}
