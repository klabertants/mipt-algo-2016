#include <cstdio>
#include <vector>
#include <iostream>
#include <queue>
#include <set>

const int INF = 1000*1000*1000;
const int ADDITION = 1005;

class Algorithm
{
public:
    class Edge
    {
    public:
        int from, to, capacity;
        int flow;
        Edge(){}
        Edge (int from_, int to_, int capacity_, int flow_) :
        from(from_), to(to_),capacity(capacity_), flow(flow_)
        {}
    };
    
    Algorithm();
    void getInput(std::istream& in);
    void addEdge(int from, int to, int capacity);
    int dinic();
    bool bfs();
    int dfs(int v, int flow);
    void dfs2(int v);
    int getSaturated();
    
    size_t vertexCount;
    int source, target;
    std::vector<std::vector<int> > graph;
    std::vector<Edge> edges;
    std::vector<int> distances;
    std::vector<int> pointers;
    std::vector<int> usability;
    std::set<int> firstDole;
};


int main()
{
    Algorithm algorithm;
    algorithm.getInput(std::cin);
    algorithm.dinic();
    int saturated = algorithm.getSaturated();
    std::cout << saturated << std::endl;
    return 0;
}

Algorithm::Algorithm()
{
    source = 0;
    target = 1;
    firstDole.clear();
}

void Algorithm::getInput(std::istream& in)
{
    in >> vertexCount;
    vertexCount += 2;
    graph.resize(vertexCount);
    distances.resize(vertexCount);
    usability.resize(vertexCount);
    for (int i = 2;i < vertexCount;++i){
        in >> usability[i];
        addEdge(source, i, usability[i] + ADDITION);
        addEdge(i, target, ADDITION);
    }
    int k, t;
    for (int i = 2;i < vertexCount;++i){
        in >> k;
        for (int j = 0;j < k;++j){
            in >> t;
            ++t;
            addEdge(i, t, INF + ADDITION);
            //addEdge (t, i, INF);
        }
    }
}

void Algorithm::addEdge(int from, int to, int capacity)
{
    Edge e1(from, to, capacity, 0);
    Edge e2(to, from, 0, 0);
    graph[from].push_back(edges.size());
    edges.push_back(e1);
    graph[to].push_back(edges.size());
    edges.push_back(e2);
}


int Algorithm::dinic()
{
    int flow = 0;
    for(;;){
        if (!bfs()){
            break;
        }
        pointers.assign(vertexCount, 0);
        while (int pushed = dfs(source, INF)){
            flow += pushed;
        }
    }
    return flow;
}

int Algorithm::getSaturated()
{
    dfs2(source);
    int ans = 0;
    std::set<int>::iterator it;
    for (it = firstDole.begin(); it != firstDole.end();it++){
        ans += usability[*it];
    }
    return ans;
}

void Algorithm::dfs2 (int v)
{
    firstDole.insert(v);
    for (int i = 0;i < edges.size();++i){
        if (edges[i].from == v && edges[i].capacity != edges[i].flow && firstDole.count(edges[i].to) == 0){
            firstDole.insert(edges[i].to);
            dfs2(edges[i].to);
        }
    }
}

bool Algorithm::bfs()
{
    std::queue<int> queue;
    queue.push(source);
    distances.assign(vertexCount, -1);
    distances[source] = 0;
    while ((!queue.empty()) && (distances[target] == -1)){
        int v = queue.front();
        queue.pop();
        for (int i = 0;i < graph[v].size();i++){
            int id = graph[v][i];
            int to = edges[id].to;
            if (distances[to] == -1 && (edges[id].flow < edges[id].capacity)){
                queue.push(to);
                distances[to] = distances[v] + 1;
            }
        }
    }
    return (distances[target] != -1);
}


int Algorithm::dfs(int v, int flow)
{
    if (!flow){
        return 0;
    }
    if (v == target){
        return flow;
    }
    for (;pointers[v] < graph[v].size();++pointers[v]){
        int id = graph[v][pointers[v]];
        int to = edges[id].to;
        if (distances[to] != distances[v] + 1){
            continue;
        }
        int pushed = dfs(to, std::min
                         (flow, (int)edges[id].capacity - edges[id].flow));
        if (pushed){
            edges[id].flow += pushed;
            edges[id ^ 1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}