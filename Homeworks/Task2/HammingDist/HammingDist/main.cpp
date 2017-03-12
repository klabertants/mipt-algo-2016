//
//  main.cpp
//  HammingDist
//
//  Created by Дмитрий Ткаченко on 21/09/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <queue>

class Net {
public:
    
    struct edge {
        int a, b, cap, flow;
    };
    
    Net(std::string s, std::string p) {
        
        text = s;
        pattern = p;
        
        int num = 0;
        nums.resize((int)s.length());
        nump.resize((int)p.length());
        
        for (int i = 0; i < s.length(); i++) {
            num += (s[i] == '?');
            nums[i] = num;
        }
        
        numss = num;
        
        for (int j = 0; j < p.length(); j++) {
            num += (p[j] == '?');
            nump[j] = num;
        }
        
        source = 0;
        sink = num + 1;
        size = num + 2;
        q.resize(size);
        
        std::vector<int> g;
        graph.assign(size, g);
        gr.assign(size, g);
        
        std::vector<std::pair<int, int>> ss, pp;
        
        std::pair<int, int> a = std::make_pair(0, 0);
        
        ss.assign((int)s.length(), a);
        pp.assign((int)p.length(), a);
        
        for (int i = 0; i < s.length() - p.length() + 1; i++) {
            for (int j = 0; j < p.length(); j++) {
                
                if (s[i + j] == '?' && p[j] == '?') {
                    add_edge(nums[i + j], nump[j], 1);
                    add_edge(nump[j], nums[i + j], 1);
                }

                if (s[i + j] == '0' && p[j] == '?') {
                    pp[j].first++;
                }
                
                if (p[j] == '0' && s[i + j] == '?') {
                    ss[i + j].first++;
                }
                
                if (s[i + j] == '1' && p[j] == '?') {
                    pp[j].second++;
                }
                
                if (p[j] == '1' && s[i + j] == '?') {
                    ss[i + j].second++;
                }
            }
        }
        
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '?') {
                add_edge(0, nums[i], ss[i].first);
                add_edge(nums[i], sink, ss[i].second);
            }
        }
        
        for (int j = 0; j < p.length(); j++) {
            if (p[j] == '?') {
                add_edge(0, nump[j], pp[j].first);
                add_edge(nump[j], sink, pp[j].second);
            }
        }
    }
    
    int dinic() {
        int flow = 0;
        for (;;) {
            if (!bfs())  break;
            ptr.assign(size, 0);
            while (int pushed = dfs (source, 1000000000) ) {
                flow += pushed;
            }
        }
        return flow;
    }
    
    void getAns() {
        
        std::queue<int> qq;
        std::vector<int> used;
        used.assign(size, 0);
        
        qq.push(source);
        used[source] = 1;
        
        while (!qq.empty()) {
            
            int x = qq.front();
            qq.pop();
            
            if ((x != source) && (x != sink)) {
                
                if (x <= numss) {
                    for (int i = 0; i < text.length(); i++) {
                        if (nums[i] == x) {
                            text[i] = '0';
                        }
                    }
                }
                else {
                    for (int j = 0; j < pattern.length(); j++) {
                        if (nump[j] == x) {
                            pattern[j] = '0';
                        }
                    }
                }
                
            }
            
            for (auto y : gr[x]) {
                for (auto t : edges) {
                    if ((t.a == x) && (t.b == y)) {
                        if ((t.cap > t.flow)) {
                            if (!used[y]) {
                                used[y] = 1;
                                qq.push(y);
                            }
                        }
                    }
                }
            }
        }
        
        for (auto x : text) {
            if (x == '?') {
                x = '1';
            }
        }
        
        for (auto x : pattern) {
            if (x == '?') {
                x = '1';
            }
        }
        
        std::cout <<  text << std::endl << pattern;
    }
    
private:
    
    void add_edge (int a, int b, int cap) {
        edge e1 = { a, b, cap, 0 };
        edge e2 = { b, a, 0, 0 };
        graph[a].push_back ((int) edges.size());
        gr[a].push_back(b);
        edges.push_back (e1);
        graph[b].push_back ((int) edges.size());
        edges.push_back (e2);
    }
    
    bool bfs() {
        int qh = 0, qt = 0;
        q[qt++] = source;
        d.assign(size, -1);
        d[source] = 0;
        while (qh < qt && d[sink] == -1) {
            int v = q[qh++];
            for (size_t i=0; i < graph[v].size(); ++i) {
                int id = graph[v][i],
                to = edges[id].b;
                if (d[to] == -1 && edges[id].flow < edges[id].cap) {
                    q[qt++] = to;
                    d[to] = d[v] + 1;
                }
            }
        }
        return d[sink] != -1;
    }
    
    int dfs (int vertex, int flow) {
        if (!flow) return 0;
        if (vertex == sink)  return flow;
        for (; ptr[vertex] < graph[vertex].size(); ++ptr[vertex]) {
            int id = graph[vertex][ptr[vertex]],
            to = edges[id].b;
            if (d[to] != d[vertex] + 1)  continue;
            int pushed = dfs (to, std::min (flow, edges[id].cap - edges[id].flow));
            if (pushed) {
                edges[id].flow += pushed;
                edges[id^1].flow -= pushed;
                return pushed;
            }
        }
        return 0;
    }
    
    int size, source, sink, numss;
    std::vector<edge> edges;
    std::vector<std::vector<int>> graph, gr;
    std::vector<int> ptr;
    std::vector<int> q, d;
    std::string text, pattern;
    std::vector<int> nums;
    std::vector<int> nump;
};


int main() {
    
    std::string s, p;
    
    std::cin >> s >> p;
    
    int tmp = 0;
    
    for (int i = 0; i < s.length() - p.length() + 1; i++) {
        for (int j = 0; j < p.length(); j++) {
            if (s[i + j] != '?' && p[j] != '?') {
                tmp += (s[i + j] != p[j]);
            }
        }
    }
    
    Net net(s, p);
    
    std::cout << tmp + net.dinic() << std::endl;
    net.getAns();
    
    return 0;
}

