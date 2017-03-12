//
//  main.cpp
//  ChangeableFlow
//
//  Created by Дмитрий Ткаченко on 20/09/16.
//  Copyright © 2016 Dmitry Tkachenko. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <queue>
#include <cstring>
#include <vector>

const int MAX_NODES = 150;

using namespace std;

class Flow {
public:
    
    int source, sink;
    
    void setSource(int s) {
        source = s;
    }
    
    void setSink(int t) {
        sink = t;
    }
    
    int size() {
        return vertex_count;
    }
    
    void addCapacity(int x, int y) {
        caps[x][y]++;
        memset(parent, -1, sizeof(parent));
        memset(pathCapacity, 0, sizeof(pathCapacity));
        memset(flowPassed, 0, sizeof(int) * MAX_NODES * MAX_NODES);
    }
    
    void decCapacity(int x, int y) {
        caps[x][y]--;
        memset(parent, -1, sizeof(parent));
        memset(pathCapacity, 0, sizeof(pathCapacity));
        memset(flowPassed, 0, sizeof(int) * MAX_NODES * MAX_NODES);
    }
    
    void initFlow() {
        cin >> vertex_count >> edge_count;
        for (int i = 0; i < MAX_NODES; i++) {
            for (int j = 0; j < MAX_NODES; j++) {
                caps[i][j] = 0;
            }
        }
        int x, y, c;
        for (int i = 0; i < edge_count; i++) {
            cin >> x >> y >> c;
            caps[--x][--y] = c;
            graph[x].push_back(y);
            graph[y].push_back(x);
        }
    }
    
    int edmondsKarp(int startNode, int endNode) {
        int maxFlow = 0;
        while (true) {
            int flow = bfs(startNode, endNode);
            if (flow == 0) {
                break;
            }
            maxFlow += flow;
            int currentNode = endNode;
            while(currentNode != startNode) {
                int previousNode = parent[currentNode];
                flowPassed[previousNode][currentNode] += flow;
                flowPassed[currentNode][previousNode] -= flow;
                currentNode = previousNode;
            }
        }
        return maxFlow;
    }
    
private:
    int edge_count, vertex_count;
    int caps[MAX_NODES][MAX_NODES];
    int flowPassed[MAX_NODES][MAX_NODES];
    vector<int> graph[MAX_NODES];
    int parent[MAX_NODES];
    int pathCapacity[MAX_NODES];
    
    int bfs(int startNode, int endNode) {
        memset(parent, -1, sizeof(parent));
        memset(pathCapacity, 0, sizeof(pathCapacity));
        
        queue<int> q;
        q.push(startNode);
        
        parent[startNode] = -2;
        pathCapacity[startNode] = 999;
        
        while(!q.empty()) {
            int currentNode = q.front();
            q.pop();
            
            for(int i = 0; i < graph[currentNode].size(); i++) {
                int to = graph[currentNode][i];
                if (parent[to] == -1) {
                    if (caps[currentNode][to] - flowPassed[currentNode][to] > 0) {
                        parent[to] = currentNode;
                        pathCapacity[to] = min(pathCapacity[currentNode], caps[currentNode][to] - flowPassed[currentNode][to]);
                        if (to == endNode) {
                            return pathCapacity[endNode];
                        }
                        q.push(to);
                    }
                }
            }
        }
        return 0;
    }

};

int main() {
    
    int t;
    Flow flow;
    
    flow.initFlow();
    flow.setSink(flow.size() - 1);
    flow.setSource(0);
    cout << flow.edmondsKarp(flow.source, flow.sink) << endl;
    
    cin >> t;
    int type, x, y;
    for (int i = 0; i < t; i++) {
        cin >> type >> x >> y;
        if (type == 1) {
            flow.addCapacity(--x, --y);
            cout << flow.edmondsKarp(flow.source, flow.sink) << endl;
        }
        else {
            flow.decCapacity(--x, --y);
            cout << flow.edmondsKarp(flow.source, flow.sink) << endl;
        }
    }
    return 0;
}
