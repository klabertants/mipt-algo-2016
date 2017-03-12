//
//  main.cpp
//  matan
//
//  Created by Дмитрий Ткаченко on 19/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <cmath>
#include <cstdio>

using namespace std;

const int INF = 1e9 + 10;
const int TO_ADD = 4747;

class edge {
    
public:
    edge() {};
    edge (int a_, int b_, int cap_, int f_) : a(a_), b(b_), cap(cap_), f(f_) {};
    
    int a, b, cap, f;
};

class Flow {
public:
    
    int size, s, t;
    vector<vector<int>> g;
    vector<int> d, p, used;
    vector<edge> e;
    set<int> p1;
    
    Flow() {
        p1.clear();
        s = 0;
        t = 1;
    }
    
    void add_edge(int a_, int b_, int cap_) {
        edge e1(a_, b_, cap_, 0);
        edge e2(b_, a_, 0, 0);
        g[a_].push_back((int)e.size());
        e.push_back(e1);
        g[b_].push_back((int)e.size());
        e.push_back(e2);
    }
    
    bool bfs() {
        queue<int> q;
        q.push(s);
        d.assign(size, -1);
        d[s] = 0;
        while (!q.empty() && d[t] == -1) {
            int top = q.front(); q.pop();
            for (int i = 0; i < g[top].size(); i++) {
                int j = g[top][i];
                int b = e[j].b;
                if (d[b] == -1 && (e[j].f < e[j].cap)) {
                    q.push(b);
                    d[b] = d[top] + 1;
                }
            }
        }
        return (d[t] != -1);
    }
    
    int dfs(int v, int flow) {
        if (!flow) {
            return 0;
        }
        if (v == t) {
            return flow;
        }
        for (;p[v] < g[v].size(); p[v]++) {
            int j = g[v][p[v]];
            int b = e[j].b;
            if (d[b] != d[v] + 1) {
                continue;
            }
            int push = dfs(b, min(flow, (int)e[j].cap - e[j].f));
            if (push) {
                e[j].f += push;
                e[j ^ 1].f -= push;
                return push;
            }
        }
        return 0;
    }
    
    void dfs2(int v) {
        p1.insert(v);
        for (int i = 0; i < e.size(); i++) {
            if (e[i].a == v && e[i].cap != e[i].f && p1.find(e[i].b) == p1.end()) {
                p1.insert(e[i].b);
                dfs2(e[i].b);
            }
        }
    }
    
    int dinic() {
        int flow = 0;
        while (1) {
            if (!bfs()) {
                break;
            }
            p.assign(size, 0);
            while (int push = dfs(s, INF)) {
                flow += push;
            }
        }
        return flow;
    }
    
    int path() {
        int ans = 0;
        dfs2(s);
        for (auto x : p1) {
            ans += used[x];
        }
        return ans;
    }
    
    void read(istream& is) {
        is >> size;
        size += 2;
        g.resize(size); d.resize(size); used.resize(size);
        for (int i = 2; i < size; i++) {
            is >> used[i];
            add_edge(s, i, used[i] + TO_ADD);
            add_edge(i, t, TO_ADD);
        }
        int tt, k;
        for (int i = 2; i < size; i++) {
            is >> k;
            for (int j = 0; j < k; j++) {
                is >> tt;
                add_edge(i, ++tt, INF + TO_ADD);
            }
        }
    } 
    
};

int main() {
    Flow flow;
    flow.read(cin); flow.dinic();
    cout << flow.path();
    return 0;
}