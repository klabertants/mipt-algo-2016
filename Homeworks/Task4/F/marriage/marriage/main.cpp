//
//  main.cpp
//  marriage
//
//  Created by Дмитрий Ткаченко on 20/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

const int maxBoys = 30010;
const int maxGirls = 2010;
const int maxK = 100010;

class Graph {
public:
    
    int boysCount, girlsCount, k, a[maxK], b[maxK], leftBound, rightBound, size, ans, current, util[maxBoys];
    vector<int> graph[maxGirls], reverse_graph[maxBoys], prefA, prefB;
    
    void init() {
        cin >> boysCount >> girlsCount >> k;
        for (int i = 0; i < k; i++) {
            cin >> a[i] >> b[i];
            graph[--b[i]].push_back(--a[i]);
            reverse_graph[a[i]].push_back(b[i]);
        }
        prefA.assign(maxGirls, -1);
        prefB.assign(maxBoys, -1);
        ans = 0;
    }
    
    int dfs(int v) {
        util[v] = current;
        for (int i = 0; i < reverse_graph[v].size(); i++) {
            int to = reverse_graph[v][i];
            if ((util[prefA[to]] != current && dfs(prefA[to])) || prefA[to] == -1) {
                prefA[to] = v;
                prefB[v] = to;
                return 1;
            }
        }
        return 0;
    }
    
    void try_inc() {
        current++;
        while (rightBound < boysCount && size < girlsCount) {
            if(dfs(rightBound++) == 1) {
                current++;
                size++;
            }
        }
    }
    
    int variate(int v) {
        util[v] = current;
        for(int i = 0; i < graph[v].size(); ++i) {
            int from = graph[v][i];
            if (leftBound < from && from < rightBound) {
                if (prefB[from] == -1 || (util[prefB[from]] != current && variate(prefB[from]))) {
                    prefA[v] = from;
                    prefB[from] = v;
                    return 1;
                }
            }
        }
        return 0;
    }
    
    int solve() {
        try_inc();
        for (; size == girlsCount && leftBound < boysCount; leftBound++) {
            ans += boysCount - rightBound + 1;
            if (prefB[leftBound] != -1) {
                prefA[prefB[leftBound]] = -1;
                current++;
                if (!variate(prefB[leftBound])) {
                    size--;
                    try_inc();
                }
            }
        }
        return ans;
    }

};

int main() {
    Graph G;
    G.init();
    cout << G.solve();
    return 0;
}