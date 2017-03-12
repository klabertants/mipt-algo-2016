//
//  main.cpp
//  Taxi
//
//  Created by Дмитрий Ткаченко on 06/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>

using namespace std;


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
    
    int size, s, t, d[1010 * 1010], ptr[1010 * 1010], q[1010 * 1010];
    vector<edge> e;
    vector<vector<int>> g;
};


int m, a[1010], b[1010], c[1010], dd[1010], tm[1010];
string t;
Flow flow;

int main() {
    cin >> m;
    
    flow.init(2 * m + 3);
    for (int i = 1; i <= m; i++) {
        cin >> t;
        cin >> a[i] >> b[i] >> c[i] >> dd[i];
        a[i + m] = a[i];
        b[i + m] = b[i];
        c[i + m] = c[i];
        dd[i + m] = dd[i];
        tm[i] = t[0] * 600 + t[1] * 60 + t[3] * 10 + t[4];
        tm[i + m] = tm[i];
    }
    for (int i = 1; i < m; i++) {
        for (int j = i + 1; j <= m; j++) {
            if ( tm[i] + abs(a[i] - c[i]) + abs(b[i] - dd[i]) + abs(a[j] - c[i]) + abs(b[j] - dd[i]) < tm[j]) {
                flow.add_edge(i, j + m, 1);
                flow.add_edge(i + m, j, 1);
            }
        }
    }
    
    for (int i = 1; i <= m; i++) {
        flow.add_edge(0, i, 1);
        flow.add_edge(i + m, 2 * m + 2, 1);
    }
    
    cout << m - flow.dinic();
    return 0;
}
