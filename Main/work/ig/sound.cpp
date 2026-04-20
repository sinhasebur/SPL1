#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF=1e18;



vector<ll> dijkstra(vector<vector<pair<ll,ll>>>& adj, ll src) {

    ll V = adj.size();

    // Min-heap (priority queue) storing pairs of (distance, node)
    priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> pq;

    vector<ll> dist(V, INT_MAX);

    // Distance from source to itself is 0
    dist[src] = 0;
    pq.emplace(0, src);

    // Process the queue until all reachable vertices are finalized
    while (!pq.empty()) {
        auto top = pq.top();
        pq.pop();

        ll d = top.first;  
        ll u = top.second; 

        // If this distance not the latest shortest one, skip it
        if (d > dist[u])
            continue;

        // Explore all neighbors of the current vertex
        for (auto &p : adj[u]) {
            ll v = p.first; 
            ll w = p.second; 

            //If we found a shorter path to v through u, update it
            //cout<<max(dist[u], w )<<endl;
            if ( max(dist[u], w )< dist[v]) {
                dist[v] = max(dist[u], w );   
                pq.emplace(dist[v], v);
            }
            // if (dist[u] + w < dist[v]) {
            //     dist[v] = dist[u] + w;   
            //     pq.emplace(dist[v], v);
            // }
        }
    }

    // Return the final shortest distances from the source
    return dist;
}





vector<ll> get_path(vector<ll> &parent , ll target_node){

    vector<ll> path;

    while(target_node!=-1){
    path.push_back(target_node);
    target_node = parent[target_node];
    }

    reverse(path.begin(),path.end());
    return path;
}




int main(){

    ll i=1;
    while(i++){

        
        ll c,s,q; cin>>c>>s>>q;
        if(c==0) break;
        if(i!=2) cout<<endl;
        cout<<"Case #"<<i-1<<endl;
        //-----------------------------inputting graph,starting and target node--------------------------------------
        ll nodes,edges;
        edges=s;
        nodes=c;
        vector<vector<pair<ll,ll>>> graph(nodes);
        ll mxwt=0;
        for(ll i=0;i<edges;i++){
            ll node1,node2,weight;
            cin>>node1>>node2>>weight;
            graph[node1-1].push_back({node2-1, weight});
            graph[node2-1].push_back({node1-1, weight});
            mxwt=max(mxwt,weight);
        }



        for(ll i=0;i<q;i++){
            ll starting_node;
            cin>>starting_node;
            ll target_node;
            cin>>target_node;
            //pair<vector<ll>,vector<ll>> distance_and_parent;
            vector <ll>v(nodes);
            //cout<<"a2"<<endl;
            v=dijkstra(graph,starting_node-1);
            
            //cout<<"a1"<<endl;
            // ll mx=0;
            // for(ll i=0;i<v.size();i++){
            //     mx=max(mx,v[i]);
            // }
            if(v[target_node-1]<=mxwt)
                cout<<v[target_node-1]<<endl;
            else{
                cout<<"no path"<<endl;
            }
        }

    }


}




