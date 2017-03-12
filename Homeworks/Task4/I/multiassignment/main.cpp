//
//  main.cpp
//  multiassignment
//
//  Created by Дмитрий Ткаченко on 09/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

const int INF = 1e9 + 10;

using namespace std;

class Graph {
public:
    
    class edge {
    public:
        edge(int to, int backEdge, int capacity, int cost) : to(to), invEdge(backEdge), flow(0), cap(capacity), cost(cost) {}
        
        int to, invEdge, flow, cap, cost;
    };
    
    int n;
    vector<vector<edge> > edges;	// исходящие ребра, соответственно по одному вектору ребер на каждую вершину
    vector<int> phi, dist, flag, from;
    
    Graph(int n) : n(n), edges(n), phi(n), dist(n), flag(n), from(n) {}
    
    void addEdge(int a, int b, int capacity, int cost) {
        edges[a].push_back(edge(b, edges[b].size(), capacity, cost));
        edges[b].push_back(edge(a, edges[a].size() - 1, 0, -cost));
    }
    
    int minCost(int source, int sink) {
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
        
        int resultFlow = 0, resultCost = 0;
        
        while (true) {
            flag.assign(n, 0);
            fill(flag.begin(), flag.end(), 0);
            dist.assign(n, INF);
            dist[source] = 0;
            
            while (true) {
                int i = -1;
                for(int j = 0; j < n; ++j)
                    if(!flag[j] && dist[j] < INF && (i < 0 || dist[i] > dist[j])) {
                        i = j;
                    }
                if(i < 0) break;
                
                flag[i] = 1;
                
                for(int j = 0; j < edges[i].size(); ++j)
                    if(edges[i][j].cap > edges[i][j].flow)
                    {
                        int a = edges[i][j].to;
                        if(!flag[a] && dist[a] > dist[i] + edges[i][j].cost + phi[i] - phi[a])
                        {
                            dist[a] = dist[i] + edges[i][j].cost + phi[i] - phi[a];
                            from[a] = edges[i][j].invEdge;
                        }
                    }
            }
            
            if(!flag[sink]) break;
            
            for(int i = 0; i < n; ++i)
                phi[i] += flag[i] ? dist[i] : dist[sink];
            
            int difFlow = INF, difCost = 0;
            for(int i = sink; i != source; )
            {
                int a = edges[i][from[i]].to;
                int b = edges[i][from[i]].invEdge;
                
                difFlow = min(difFlow, edges[a][b].cap - edges[a][b].flow);
                difCost += edges[a][b].cost;
                
                i = a;
            }
            
            for(int i = sink; i != source; )
            {
                int a = edges[i][from[i]].to;
                int b = edges[i][from[i]].invEdge;
                
                edges[a][b].flow += difFlow;
                edges[i][from[i]].flow -= difFlow;
                
                i = a;
            }
            
            resultFlow += difFlow;
            resultCost += difFlow * difCost;
        }
        
        return resultCost;
    }
};

int main () {
    int n, k, graphSize, a;
    cin >> n >> k;
    graphSize = n * 2 + 2;
    Graph graph (graphSize);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cin >> a;
            graph.addEdge(i, n + j, 1, a);
        }
    }
    for (int i = 0;i < n;++i){
        graph.addEdge(graphSize - 2, i, k, 0);
    }
    for (int j = 0;j < n;++j){
        graph.addEdge(n + j, graphSize - 1, k, 0);
    }
    cout << graph.minCost(graphSize - 2, graphSize - 1) << endl;
    
    vector<vector<int>> t;
    t.resize(n + 1);
    for (int i = 0; i <= n; i++){
        t[i].assign(n + 1, 3);
    }
    for (int i = 0; i <= n; i++){
        t[i][0] = 0;
        t[0][i] = 0;
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < graph.edges[i].size(); j++){
            Graph::edge edge = graph.edges[i][j];
            if (edge.flow == 1){
                if (edge.cost >= 0 && edge.flow == 1) {
                    t[i + 1][edge.to - n + 1] = 1;
                }
            }
        }
    }
    vector<int> u, v, p, path, ans;
    
    for (int z = 0; z < k; z++) {
        u.assign(n + 1, 0);
        v.assign(n + 1, 0);
        p.assign(n + 1, 0);
        path.assign(n + 1, 0);
        ans.assign(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int freeCol = 0;
            vector<int> minv(n + 1, INF);
            vector<char> used(n + 1, false);
            do {
                used[freeCol] = true;
                int freeRow = p[freeCol];
                int delta = INF;
                int nextFreeCol;
                for (int j = 1; j <= n; ++j) {
                    if (!used[j]) {
                        int current = t[freeRow][j] - u[freeRow] - v[j];
                        if (current < minv[j]) {
                            minv[j] = current;
                            path[j] = freeCol;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            nextFreeCol = j;
                        }
                    }
                }
                for (int j = 0; j <= n; ++j) {
                    if (used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                freeCol = nextFreeCol;
            } while (p[freeCol] != 0);
            do {
                int nextFreeCol = path[freeCol];
                p[freeCol] = p[nextFreeCol];
                freeCol = nextFreeCol;
            } while (freeCol);
        }
        for (int j = 1; j <= n; j++) {
            ans[p[j]] = j;
        }
        for (int i = 1; i <= n; i++){
            cout << ans[i] << " ";
        }
        cout << endl;
        for (int i = 1; i <= n; i++){
            t[i][ans[i]] = 3;
        }
    }
}