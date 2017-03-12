//
//  main.cpp
//  brides
//
//  Created by Дмитрий Ткаченко on 17/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

const int INF = 1e9 + 10;

class Graph {
public:
    
    class edge {
    public:
        edge(int to, int invEdge, int capacity, int cost, int num) : to(to), invEdge(invEdge), flow(0), cap(capacity), cost(cost), num(num) {}
        
        int to, invEdge, flow, cap, cost, num;
    };
    
    int n, minCost, maxFlow;
    vector<vector<edge> > edges;
    vector<int> phi, dist, flag, from;
    
    Graph(int n) : n(n), edges(n), phi(n), dist(n), flag(n), from(n) {}
    
    void addEdge(int a, int b, int capacity, int cost, int num) {
        edges[a].push_back(edge(b, edges[b].size(), capacity, cost, num));
        edges[b].push_back(edge(a, edges[a].size() - 1, 0, -cost, num));
    }
    
    
    void minCostMaxFlow(int source, int sink) {
        {
            phi.assign(n, INF);
            phi[source] = 0;
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < n; ++j)
                    if(phi[j] < INF)
                        for(int k = 0; k < edges[j].size(); ++k)
                            if(edges[j][k].cap > 0 && phi[edges[j][k].to] > phi[j] + edges[j][k].cost)
                                phi[edges[j][k].to] = phi[j] + edges[j][k].cost;
        }
        
        minCost = 0;
        maxFlow = 0;
        
        while (true) {
            flag.assign(n, 0);
            dist.assign(n, INF);
            dist[source] = 0;
            
            while (true) {
                int i = -1;
                for(int j = 0; j < n; ++j)
                    if(!flag[j] && dist[j] < INF && (i < 0 || dist[i] > dist[j]))
                        i = j;
                if(i < 0) break;
                
                flag[i] = 1;
                
                for(int j = 0; j < edges[i].size(); ++j) {
                    if(edges[i][j].cap > edges[i][j].flow) {
                        int a = edges[i][j].to;
                        if(!flag[a] && dist[a] > dist[i] + edges[i][j].cost + phi[i] - phi[a]) {
                            dist[a] = dist[i] + edges[i][j].cost + phi[i] - phi[a];
                            from[a] = edges[i][j].invEdge;
                        }
                    }
                }
            }
            
            if(!flag[sink]) {
                break;
            }
            
            for(int i = 0; i < n; ++i) {
                phi[i] += flag[i] ? dist[i] : dist[sink];
            }
            
            
            int difFlow = INF, difCost = 0;
            for(int i = sink; i != source; ) {
                int a = edges[i][from[i]].to;
                int b = edges[i][from[i]].invEdge;
                
                difFlow = min(difFlow, edges[a][b].cap - edges[a][b].flow);
                difCost += edges[a][b].cost;
                
                i = a;
            }
            
            for(int i = sink; i != source; ) {
                int a = edges[i][from[i]].to;
                int b = edges[i][from[i]].invEdge;
                
                edges[a][b].flow += difFlow;
                edges[i][from[i]].flow -= difFlow;
                
                i = a;
            }
            
            maxFlow += difFlow;
            minCost += difFlow * difCost;
        }
    }
};

vector<vector<pair<int, int>>> g;

set<int> badNumbers;

void bfs (int n) {
    queue<int> q;
    q.push(1);
    vector<bool> used (n);
    vector<int> d (n);
    vector<pair<int, int> > p(n);
    used[1] = true;
    p[1].second = -1;
    p[1].first = -1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (size_t i = 0; i < g[v].size(); i++) {
            if (badNumbers.count(g[v][i].second) == 0) {
                int to = g[v][i].first;
                if (!used[to]) {
                    used[to] = true;
                    q.push(to);
                    d[to] = d[v] + 1;
                    p[to].first = v;
                    p[to].second = g[v][i].second;
                }
            }
        }
    }
    vector<int> path;
    for (int v = n - 1; p[v].first != -1; v = p[v].first){
        path.push_back(p[v].second);
        badNumbers.insert(p[v].second);
    }
    cout << path.size() << " ";
    for (int i = path.size() - 1; i >= 0; i--){
        cout << path[i] << " ";
    }
    cout << endl;
}

int main ()
{
    int n, m, k, a, b, c;
    cin >> n >> m >> k;
    Graph graph (n + 1);
    for (int i = 0; i < m; i++) {
        cin >> a >> b >> c;
        graph.addEdge(a, b, 1, c, i);
        graph.addEdge(b, a, 1, c, i);
    }
    graph.addEdge(0, 1, k, 0, INF);
    graph.minCostMaxFlow(0, n);
    if (graph.maxFlow < k){
        cout << -1 << endl;
        return 0;
    }
    double ans = (double)graph.minCost;
    ans /= k;
    cout.precision(47);
    cout << ans << endl;
    g.resize(n + 1);
    for (int i = 1; i < graph.edges.size(); i++) {
        for (int j = 0; j < graph.edges[i].size(); j++) {
            Graph::edge e= graph.edges[i][j];
            if (e.flow == e.cap && e.flow > 0) {
                g[i].push_back(make_pair(e.to, e.num + 1));
            }
        }
    }
    for (int i = 0; i < k; i++) {
        bfs(n + 1);
    }
    return 0;
}
