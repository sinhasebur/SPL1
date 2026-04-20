#include <bits/stdc++.h>
using namespace std;

#define ll long long

int get(int i){
    if(i==2) return 3;
    else return 2;
}

bool bfs(vector<vector<ll>>& adj){
    ll n = adj.size();
    vector<ll> res;
    vector<ll> visited(n,0);
    queue<ll> q;

    q.push(0);
    visited[0] = 2;

    bool ans=false;

    while(!q.empty()){
        ll node = q.front();
        q.pop();

        res.push_back(node);

        for(ll i = 0; i < adj[node].size(); i++){
            ll v = adj[node][i];
            if(!visited[v]){
                visited[v] =get(visited[node]);
                q.push(v);
            }
            else{
                if(visited[v]==visited[node]){
                    ans=false;
                    return ans;
                }
            }
        }
    }
    ans=true;
    return ans;
}

int main(){

    //nt t; cin>>t;
    while(1){
        ll n;
        cin >> n;
        if(n==0) break;
        ll m; cin >>m;
        vector<vector<ll>> adj(n);

        for(ll i = 0; i < m; i++){
            ll u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        vector<ll> visited(n,0);
        int ans= bfs(adj);

        if(ans){
            cout<<"BICOLORABLE."<<endl;
        }
        else{
            cout<<"NOT BICOLORABLE."<<endl;
        }
    }
    
}