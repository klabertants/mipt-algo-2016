//
//  main.cpp
//  Chemistry!!!
//
//  Created by Дмитрий Ткаченко on 26/09/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

//Const arrays for walking in neighbourhood.
const int neighb_i[4] = { -1, 1, 0,  0 };
const int neighb_j[4] = { 0,  0, 1, -1 };

//-------------------------------

//Some variables.
vector<string> desc;
int n, m, a, b;
int numbers[60][60];

//-------------------------------

//Some functions.
int capc(char c) {
    switch (c) {
        case 'H' : return 1;
        case 'O' : return 2;
        case 'N' : return 3;
        case 'C' : return 4;
        default : return 0;
    }
    return 0;
}

//-------------------------------

class Flow {
public:
    
    struct edge {
        int a, b, cap, flow;
    };
    
    void init(int n) {
        size = n;
        vector<int> gg;
        for (int i = 0; i < n + 10; i++) {
            g.push_back(gg);
        };
        s = 0;
        t = n - 1;
    }
    
    int dinic() {
        int flow = 0;
        for (;;) {
            if (!bfs())  break;
            memset (ptr, 0, size * sizeof ptr[0]);
            while (int pushed = dfs (s, 1000000000))
                flow += pushed;
        }
        return flow;
    }
    
    void add_edge (int a, int b, int cap) {
        edge e1 = { a, b, cap, 0 };
        edge e2 = { b, a, 0, 0 };
        g[a].push_back ((int) e.size());
        e.push_back (e1);
        g[b].push_back ((int) e.size());
        e.push_back (e2);
    }
    
    bool isSaturated() {
        bool flag = true;
        for (int j = 0; j < size; j++) {
            for (int i = 0; i < g[j].size(); i++) {
                if ( (j == 0) || (e[g[j][i]].b == t) ) {
                    flag *= (e[g[i][j]].cap == e[g[i][j]].flow);
                }
            }
        }
        return flag;
    }
    
private:
    
    bool bfs() {
        int qh=0, qt=0;
        q[qt++] = s;
        memset (d, -1, size * sizeof d[0]);
        d[s] = 0;
        while (qh < qt && d[t] == -1) {
            int v = q[qh++];
            for (size_t i=0; i<g[v].size(); ++i) {
                int id = g[v][i],
                to = e[id].b;
                if (d[to] == -1 && e[id].flow < e[id].cap) {
                    q[qt++] = to;
                    d[to] = d[v] + 1;
                }
            }
        }
        return d[t] != -1;
    }
    
    int dfs (int v, int flow) {
        if (!flow)  return 0;
        if (v == t)  return flow;
        for (; ptr[v]<(int)g[v].size(); ++ptr[v]) {
            int id = g[v][ptr[v]],
            to = e[id].b;
            if (d[to] != d[v] + 1)  continue;
            int pushed = dfs (to, min (flow, e[id].cap - e[id].flow));
            if (pushed) {
                e[id].flow += pushed;
                e[id^1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }
    
    int size, s, t, d[60 * 60], ptr[60 * 60], q[60 * 60];
    vector<edge> e;
    vector<vector<int>> g;
};

int main() {
    
    //Scanning for size.
    scanf("%d%d\n", &n, &m);
    
    //Scanning for desc.
    desc.resize(n);
    for (int i = 0; i < n; i++) {
        getline(cin, desc[i]);
        desc[i].resize(m);
    }
    
    int cntNotEmpty = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cntNotEmpty += (desc[i][j] != '.');
        }
    }
    
    if (cntNotEmpty == 0) {
        cout << "Invalid";
        return 0;
    }
    
    Flow flow;
    
    int prev = 1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (desc[i][j] != '.') {
                numbers[i][j] = prev;
                prev++;
            }
            else {
                numbers[i][j] = -1;
            }
        }
    }
    
    flow.init(cntNotEmpty + 2);
    
    //Adding edges.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if  ((i + j) % 2 && capc(desc[i][j])) {
                for (int k = 0; k < 4; k++) {
                    int ni = i + neighb_i[k];
                    if (ni < 0 || ni >= n) {
                        continue;
                    }
                    int nj = j + neighb_j[k];
                    if (nj < 0 || nj >= m) {
                        continue;
                    }
                    if  (capc(desc[ni][nj])) {
                        flow.add_edge(numbers[i][j], numbers[ni][nj], 1);
                    }
                }
            }
        }
    }
    
    int s_size = 0;
    int t_size = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if ((i + j) % 2 && capc(desc[i][j])) {
                flow.add_edge(0, numbers[i][j], capc(desc[i][j]));
                s_size += capc(desc[i][j]);
                continue;
            }
            if (((i + j) % 2 == 0) && capc(desc[i][j])) {
                flow.add_edge(numbers[i][j], cntNotEmpty + 1, capc(desc[i][j]));
                t_size += capc(desc[i][j]);
            }
        }
    }
    
    if (s_size == t_size && t_size == flow.dinic()) {
        cout << "Valid";
    }
    else {
        cout << "Invalid";
    }
    return 0;
}