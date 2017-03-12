//
//  main.cpp
//  ComplexQuestion
//
//  Created by Дмитрий Ткаченко on 06/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cstring>
#include <set>

using namespace std;

int n, k, gcnt, bcnt, q, cnt, tmp;
vector<vector<int>> graph, myGraph;
vector<int> used_g, currentMatching;
vector<char> used, used_b;

//Kuhn's algortihm for matching find.
bool Kuhn (int v) {
    if (used[v])  return false;
    used[v] = true;
    for (size_t i = 0; i < myGraph[v].size(); ++i) {
        int to = myGraph[v][i];
        if (currentMatching[to] == -1 || Kuhn (currentMatching[to])) {
            currentMatching[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    
    //Reading.
    cin >> n >> k;
    graph.resize(n);
    for (int i = 0; i < n; i++){
        cin >> cnt;
        for (int j = 0; j < cnt; j++){
            cin >> tmp;
            graph[i].push_back(--tmp);
        }
    }
    
    cin >> q;
    
    while (q > 0) {
        //Preparign the graf.
        myGraph.clear();
        myGraph.resize(n);
        
        used_g.assign(k, 0);
        used_b.assign(n, false);
        
        //Making edges.
        gcnt = 0;
        cin >> bcnt;
        for (int j = 0; j < bcnt; j++){
            cin >> tmp;
            used_b[--tmp] = true;
            for (int t = 0; t < graph[tmp].size(); t++){
                int curGirl = graph[tmp][t];
                if (used_g[curGirl] == 0){
                    gcnt++;
                    used_g[curGirl] = 1;
                }
            }
        }
        
        for (int j = 0; j < n; j++){
            if (!used_b[j]) {
                for (int t = 0; t < graph[j].size(); t++) {
                    if (used_g[graph[j][t]] == 1) {
                        myGraph[j].push_back(graph[j][t]);
                    }
                }
            }
            else{
                for (int t = 0; t < graph[j].size(); t++){
                    used_g[graph[j][t]] = 2;
                }
            }
        }
        
        //Trying Kuhn.
        int matchingSize = 0;
        currentMatching.assign (k, -1);
        for (int v = 0; v < n; ++v) {
            used.assign (n, false);
            if (Kuhn (v)){
                matchingSize++;
            }
        }
        
        cout << ((matchingSize == gcnt) ? "Yes\n" : "No\n");
        
        q--;
    }
    
    return 0;
}
