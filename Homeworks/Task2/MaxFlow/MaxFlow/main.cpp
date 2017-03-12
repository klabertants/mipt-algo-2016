//Created by Tkachenko Dmitry, 14.09.2016

#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstring>

using namespace std;

class Flow {
public:
    
    struct edge {
        int a, b, cap, flow;
    };
    
    //Initialize the flow.
    void init(int n, int s_, int t_, int c_) {
        size = n;
        s = s_;
        t = t_;
        int x, y, w;
        vector <vector<int>> g_;
        vector <int> g__;
        g_.assign(size + 1, g__);
        g = g_;
        for (int i = 0; i < c_; i++) {
            cin >> x >> y >> w;
            add_edge(x - 1, y - 1, w);
            add_edge(y - 1, x - 1, w);
        }
    }
    
    void clear() {
        g.clear();
        e.clear();
        for (int i = 0; i < 102; i++) {
            d[i] = 0;
            ptr[i] = 0;
            q[i] = 0;
        }
    };
    
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
    
    
private:
    
    void add_edge (int a, int b, int cap) {
        edge e1 = { a, b, cap, 0 };
        edge e2 = { b, a, 0, 0 };
        g[a].push_back ((int) e.size());
        e.push_back (e1);
        g[b].push_back ((int) e.size());
        e.push_back (e2);
    }
    
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
    
    int size, s, t, d[102], ptr[102], q[102];
    vector<edge> e;
    vector<vector<int>> g;
};


int main() {
    Flow flow;
    int s, t, n, c;
    n = 31;
    while (n != 0) {
        cin >> n;
        if (n == 0) break;
        cin >> s >> t >> c;
        flow.init(n, s - 1, t - 1, c);
        cout << flow.dinic() << endl;
    }
    
    return 0;
}
