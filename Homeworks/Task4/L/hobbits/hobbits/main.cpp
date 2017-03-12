//
//  main.cpp
//  hobbits
//
//  Created by Дмитрий Ткаченко on 17/10/2016.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>

using namespace std;

class Graph {
public:
    
    Graph() {};

    bool matchVisit(int v) {
        used[v] = 1;
        for (int i = 0;i < leftSize;i++){
            if (graph[v][i] == '1') {
                if (match[i] == -1 ||
                    (!used[match[i]] && matchVisit(match[i]))) {
                    match[i] = v;
                    return true;
                }
            }
        }
        return false;
    }
    
    void controlVisit(int v) {
        used[v] = 1;
        for (int i = 0;i < graph[v].size();i++){
            if (graph[v][i] == '1') {
                if (!right[i] && !used[match[i]]) {
                    controlVisit(match[i]);
                    right[i] = true;
                }
            }
        }
    }
    
    void transitiveClosure() {
        for (int k = 0; k < leftSize; k++){
            for (int i = 0; i < leftSize; i++){
                for (int j = 0; j < leftSize; j++) {
                    if (graph[i][j] == '0'){
                        if (graph[i][k] == graph[k][j] && graph[i][k] == '1'){
                            graph[i][j] = '1';
                        }
                    }
                }
            }
        }
    }
    
    int getRightSize() {return rightSize; };
    
    int getLeftSize() { return leftSize; };
    
    void read(istream& in) {
        in >> leftSize;
        rightSize = leftSize;
        graph.resize(leftSize);
        for (int i = 0; i < leftSize; i++) {
            graph[i].resize(leftSize);
        }
        for (int i = 0; i < leftSize; i++) {
            for (int j = 0; j < rightSize; j++) {
                in >> graph[i][j];
            }
        }
    }
    
    void clear() {
        leftSize = 0;
        rightSize = 0;
        right.clear();
        used.clear();
        match.clear();
        for (int i = 0; i < graph.size(); i++){
            graph[i].clear();
        }
        graph.clear();
    }
    
    pair<set<int>, set<int> > findControlSet() {
        transitiveClosure();
        
        match.assign(rightSize, -1);
        used.assign(leftSize, 0);
        right.assign(rightSize, 0);
        
        for (int i = 0;i < leftSize;i++){
            used.assign(leftSize, 0);
            matchVisit(i);
        }
        vector<char> free;
        free.assign(leftSize, 1);
        for (int i = 0;i < rightSize;i++){
            if (match[i] != -1){
                free[match[i]] = false;
            }
        }
        used.assign(leftSize, 0);
        for (int i = 0;i < leftSize;i++) {
            if (free[i]){
                controlVisit(i);
            }
        }
        pair<set<int>, set<int> > res;
        for (int i = 0;i < rightSize;i++){
            if (match[i] != -1){
                if (right[i]){
                    res.second.insert(i);
                }
                else{
                    res.first.insert(match[i]);
                }
            }
        }
        return res;
    }

private:
    
    
    
    vector<char> right;
    vector<char> used;
    vector<int> match;
    vector<vector<char> > graph;
    size_t leftSize, rightSize;
};


int main () {
    Graph g;
    g.read(cin);
    pair<set<int>, set<int> > tmp = g.findControlSet();
    vector<int> ans;
    int n = g.getLeftSize();
    for (int i = 0;i < n;i++){
        if (tmp.first.count(i) == 0 && tmp.second.count(i) == 0){
            ans.push_back(i);
        }
    }
    cout << ans.size() << endl;
    for (int i = 0; i < ans.size(); i++){
        cout << ans[i] + 1 << " ";
    }
    cout << endl;
    g.clear();
    return 0;
}

