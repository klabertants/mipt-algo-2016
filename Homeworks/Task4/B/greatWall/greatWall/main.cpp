//
//  main.cpp
//  greatWall
//
//  Created by Дмитрий Ткаченко on 03/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

const int INF = 1e9 + 10;
const int di[] = {-1, 0, 0, +1};
const int dj[] = {0, -1, +1, 0};
int table[100][100];
int n, m, k, l;

class Graph {
public:
    struct edge {
        int u, v, c, f;
        
        int dif() {
            return c - f;
        }
    };

    Graph(int n) : n(n), m(0), first(n, -1) {}

    int n, m, flow = 0;;
    vector<edge> edges;
    vector<int> next, first;
    vector<bool> used;

    void add_edge(int u, int v, int c) {
        smart_add_edge(u, v, c);
        smart_add_edge(v, u, 0);
    }

    void smart_add_edge(int u, int v, int c) {
        edges.push_back({u, v, c, 0});
        next.push_back(first[u]);
        first[u] = (int) next.size() - 1;
        used.push_back(true);
        ++m;
    }

    int first_edge(int u) {
        int i = first[u];
        for (; i != -1 && !used[i]; i = next[i]);
        return i;
    }

    int next_edge(int i) {
        i = next[i];
        for (; i != -1 && !used[i]; i = next[i]);
        return i;
    }
};

bool getLevel(Graph &g, int s, int t) {
    int n = g.n;
    vector<int> level(n, -1);
    vector<int> curr = {s}, next;
    for (int i = 0; !curr.empty(); ++i) {
        for (int u : curr) {
            if (level[u] == -1) {
                level[u] = i;
                for (int j = g.first[u]; j != -1; j = g.next_edge(j)) {
                    Graph::edge e = g.edges[j];
                    int v = e.v;
                    if (e.dif() > 0 && level[v] == -1) {
                        next.push_back(v);
                    }
                }
            }
        }
        curr.clear();
        swap(next, curr);
    }

    for (int u = 0; u < n; ++u) {
        for (int j = g.first[u]; j != -1; j = g.next_edge(j)) {
            Graph::edge e = g.edges[j];
            int v = e.v;
            g.used[j] = level[u] + 1 == level[v];
        }
    }
    return level[t] != -1;
}

int dfs(Graph &g, vector<int> &curr, int t, int u, int max_flow) {
    if (max_flow == 0) {
        return 0;
    }
    if (u == t) {
        return max_flow;
    }
    for (; curr[u] != -1; curr[u] = g.next_edge(curr[u])) {
        Graph::edge &e = g.edges[curr[u]];
        int flow = dfs(g, curr, t, e.v, min(max_flow, e.dif()));
        if (flow > 0) {
            e.f += flow;
            g.edges[curr[u] ^ 1].f -= flow;
            return flow;
        }
    }
    return 0;
}

void blockingFlow(Graph &g, int s, int t) {
    int n = g.n;
    vector<int> curr;
    for (int u = 0; u < n; ++u) {
        curr.push_back(g.first_edge(u));
    }
    while (int flow = dfs(g, curr, t, s, 2147483647)) {
        g.flow += flow;
    }
}

bool used[100000];

void smartDfs(Graph &g, int u) {
    if (used[u]) {
        return;
    }
    used[u] = 1;
    for (int i = g.first_edge(u); i != -1; i = g.next_edge(i)) {
        Graph::edge &e = g.edges[i];
        if (e.dif() > 0) {
            smartDfs(g, e.v);
        }
    }
}

int main() {
    cin >> n >> m >> k >> l;

    for (int i = 0; i <= n + 1; i++) {
        for (int j = 0; j <= m + 1; j++) {
            table[i][j] = 1;
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            table[i][j] = 0;
        }
    }

    while (k--) {
        int x, y;
        cin >> x >> y;
        table[x][y] = 1;
    }
    
    while (l--) {
        int x, y;
        cin >> x >> y;
        table[x][y] = 2;
    }
    
    int xa, ya, xb, yb;
    cin >> xa >> ya >> xb >> yb;
    
    int s = (xa * m + ya) * 2;
    int t = (xb * m + yb) * 2;

    Graph g((n + 1) * (m + 1) * 2);
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (table[i][j] != 1) {
                if (table[i][j] == 2) {
                    g.add_edge((i * m + j) * 2 + 1, (i * m + j) * 2, 1);
                }
                for (int k = 0; k < 4; ++k) {
                    int ni = i + di[k];
                    int nj = j + dj[k];
                    if (table[ni][nj] != 1) {
                        g.add_edge((i * m + j) * 2, (ni * m + nj) * 2 + (table[ni][nj] == 2), INF);
                    }
                }
            }
        }
    }

    while (true) {
        g.used.assign(g.m * 2, true);
        if (!getLevel(g, s, t)) {
            break;
        }
        blockingFlow(g, s, t);
    }

    smartDfs(g, s);
    vector<pair<int, int>> ans;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (table[i][j] == 2 && used[(i * m + j) * 2] != used[(i * m + j) * 2 + 1]) {
                ans.push_back({i, j});
            }
        }
    }

    if (g.flow >= INF){
        cout << -1 << endl;
        return 0;
    }

    cout << ans.size() << endl;
    for (auto p : ans) {
        cout << p.first << " " << p.second << endl;
    }
    return 0;
}