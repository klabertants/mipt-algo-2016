#include <iostream>
#include <vector>
#include <queue>
#include <string>

#define INF 1000000000

class Edge {
private:
    int f_, c_;
    int from_, to_;
public:
    Edge (int from, int to, int f) : from_(from), to_(to), f_(f), c_(0) {
    }

    int getF ()
    {
        return f_;
    }
    int getC ()
    {
        return c_;
    }
    int getFrom ()
    {
        return from_;
    }
    int getTo ()
    {
        return to_;
    }

    int getRest ()
    {
        return f_ - c_;
    }

    bool isFull ()
    {
        return getRest() == 0;
    }

    bool pushFlow (int add) 
    {
        if (c_ + add > f_) return false;
        c_ += add;
        return true;
    }

    void incF ()
    {
        f_++;
    }
    void decF ()
    {
        f_--;
        if (c_ > f_) {
            c_ = f_;
        }
    }
};

int n = 0, s, t;
std::vector<Edge> edges;
std::vector<std::vector<std::size_t> > network;

std::vector<std::size_t> ptr;
std::vector<int> d;

bool bfs ()
{
    std::queue<int> q;
    for (auto& d_i : d) {
        d_i = -1;
    }
    d[s] = 0;
    q.push (s);
    while (!q.empty() && d[t] == -1) {
        int from = q.front();
        q.pop();

        for (std::size_t i = 0; i < network[from].size(); ++i) {
            int id = network[from][i];
            int to = edges[id].getTo();

            if (d[to] == -1 && !edges[id].isFull()) {
                d[to] = d[from] + 1;
                q.push (to);
            }
        }
    }
    return d[t] != -1;
} 

int dfs (int v, int flow)
{
    if (flow == 0 || v == t) return flow;
    for (; ptr[v] < network[v].size(); ++ptr[v]) {
        std::size_t id = network[v][ptr[v]];

        if (d[edges[id].getTo()] == d[v] + 1) {
            int pushed = dfs(edges[id].getTo(), std::min(flow, edges[id].getRest()));

            if (pushed > 0) {
                edges[id  ].pushFlow( pushed);
                edges[id^1].pushFlow(-pushed);

                return pushed;
            }
        }
    }
    return 0;
}

int dfsChange (int v, int flow)
{
    d[v] = 1;
    if (flow == 0 || v == t) {
        return flow;
    }
    for (int i = 0; i < network[v].size(); ++i) {
        std::size_t id = network[v][i];

        if (d[edges[id].getTo()] == 0 && !edges[id].isFull()) {
            int pushed = dfsChange(edges[id].getTo(), std::min(flow, edges[id].getRest()));

            if (pushed > 0) {
                edges[id  ].pushFlow( pushed);
                edges[id^1].pushFlow(-pushed);

                return pushed;
            }
        }
    }
    return 0;
}

int dinic ()
{
    int maxFlow = 0;
    while (bfs()) {
        for (auto& ptr_i : ptr) {
            ptr_i = 0;
        }
        while (int pushed = dfs (s, INF)) {
            maxFlow += pushed;
        }
    }
    return maxFlow;
}

void addEdge (int from, int to, int f) 
{
    edges.push_back(Edge(from, to, f));
    network[from].push_back (edges.size() - 1);
    edges.push_back(Edge(to, from, 0));
    network[to].push_back (edges.size() - 1);
}

void create ()
{
    network.resize(n);
    ptr.resize(n);
    d.resize(n);
}

void destroy () 
{
    edges.clear();
    for (int i = 0; i < n; ++i)
    {
        network.clear();
    }
    network.clear();
    ptr.clear();
    d.clear();
}

std::vector<std::string> input;
int h, w, a, b;

int getId (int i, int j) 
{
    return i * w + j + 2;
}

int isStar (int i, int j) 
{
    if (i < 0 || i >= h || j < 0 || j >= w) return false;
    return input[i][j] == '*';
}

int main ()
{
    int cntStars = 0;
    std::cin >> h >> w >> a >> b;
    input.resize(h);
    for (int i = 0; i < h; ++i) {
        std::cin >> input[i];
    }

    s = 0, t = 1;
    n = w * h + 2;
    create();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (input[i][j] == '*') {
                cntStars ++;
                if ((i + j) & 1) {
                    if (isStar(i - 1, j)) {
                        addEdge (getId(i - 1, j), getId(i, j), 1);
                    }
                    if (isStar(i + 1, j)) {
                        addEdge (getId(i + 1, j), getId(i, j), 1);
                    }
                    if (isStar(i, j - 1)) {
                        addEdge (getId(i, j - 1), getId(i, j), 1);
                    }
                    if (isStar(i, j + 1)) {
                        addEdge (getId(i, j + 1), getId(i, j), 1);
                    }

                    addEdge (getId(i, j), t, 1);
                } else {
                    addEdge (s, getId(i, j), 1);

                    if (isStar(i - 1, j)) {
                        addEdge (getId(i, j), getId(i - 1, j), 1);
                    }
                    if (isStar(i + 1, j)) {
                        addEdge ( getId(i, j),getId(i + 1, j), 1);
                    }
                    if (isStar(i, j - 1)) {
                        addEdge (getId(i, j), getId(i, j - 1), 1);
                    }
                    if (isStar(i, j + 1)) {
                        addEdge (getId(i, j), getId(i, j + 1), 1);
                    }
                }
            }
        }
    }

    if (a >= 2 * b) {
        std::cout << b * cntStars;
    } else {
        int maxPair = dinic();
        std::cout << a * maxPair + b * (cntStars - 2 * maxPair);
    }
    std::cout << std::endl;

    return 0;
}