//
//  main.cpp
//  SpoiledParquet
//
//  Created by Дмитрий Ткаченко on 15/09/16.
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
int numbers[310][310];

//-------------------------------

//Some functions.

int number(int i, int j) {
    return i * m + j;
}

bool isSpoiled(int i, int j) {
    return 0 <= i && i < n && 0 <= j && j < m && desc[i][j] == '*';
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
    
    int size, s, t, d[301 * 301], ptr[301 * 301], q[301 * 301];
    vector<edge> e;
    vector<vector<int>> g;
};

int main() {
    
    //Scanning for size and cost.
    scanf("%d%d%d%d\n", &n, &m, &a, &b);
    
    //Scanning for desc.
    desc.resize(n);
    for (int i = 0; i < n; i++) {
        getline(cin, desc[i]);
        desc[i].resize(m);
    }
    
    int cntSpoiled = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cntSpoiled += isSpoiled(i, j);
        }
    }
    
    Flow flow;
    
    int prev = 1;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (isSpoiled(i, j)) {
                numbers[i][j] = prev;
                prev++;
            }
            else {
                numbers[i][j] = -1;
            }
        }
    }
    
    flow.init(cntSpoiled + 2);
    
    //Adding edges.
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if  ((i + j) % 2 && isSpoiled(i, j)) {
                for (int k = 0; k < 4; k++) {
                    int ni = i + neighb_i[k];
                    int nj = j + neighb_j[k];
                    if  (isSpoiled(ni, nj)) {
                        flow.add_edge(numbers[i][j], numbers[ni][nj], 1);
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if ((i + j) % 2 && isSpoiled(i, j)) {
                flow.add_edge(0, numbers[i][j], 1);
                continue;
            }
            if (((i + j) % 2 == 0) && isSpoiled(i, j)) {
                flow.add_edge(numbers[i][j], cntSpoiled + 1, 1);
            }
        }
    }
    
    //If there's no need to buy 1x2 parquet.
    if  (2 * b <= a) {
        cout << b * cntSpoiled;
        return 0;
    }
    
    //The other case.
    int cntPairs = flow.dinic();
    printf("%d\n", cntPairs * a + (cntSpoiled - 2 * cntPairs) * b);
    
    return 0;
}