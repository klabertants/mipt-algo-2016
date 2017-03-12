#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>


const int INF = 1000*1000*1000;

class Edge
{
public:
    Edge(int to, int backEdge, int capacity, int cost, int num) : to(to), backEdge(backEdge), flow(0), capacity(capacity), cost(cost), num(num) {}

	int to;			// номер вершины, в которую идет ребро
	int backEdge;			// номер обратного к данному ребру ребра, нумерация конечно в списке вершины next
	int flow;			// текущий поток
	int capacity;			// пропускная способность ребра
	int cost;			// стоимость потока по данному ребру
    int num;
};


class Graph
{
public:
	int n;
    int minCost;
    int maxFlow;
	std::vector<std::vector<Edge> > edges;	// исходящие ребра, соответственно по одному вектору ребер на каждую вершину
	std::vector<int> potentials;		// вектор потенциалов, по одному потенциалу на каждую вершину :)
	std::vector<int> distances;			// вектор текущих расстояний для алгоритма Дейкстры
	std::vector<int> f;			// вектор флажков для алгоритма Дейкстры
	std::vector<int> from;		// вектор, по которому восстанавливается путь после алгоритма Дейкстры; точное
														// определение - номер ребра в списке ребер для данной вершины, по которому
														// надо перейти, чтобы попасть в вершину-предшественника

	Graph(int n) : n(n), edges(n), potentials(n), distances(n), f(n), from(n) {}

	void addEdge(int a, int b, int capacity, int cost, int num)
	{
		edges[a].push_back(Edge(b, edges[b].size(), capacity, cost, num));
		edges[b].push_back(Edge(a, edges[a].size() - 1, 0, -cost, num));
	}


	void solve(int source, int target)
	{
		{
            potentials.assign(n, INF);
			potentials[source] = 0;
			for(int i = 0; i < n; ++i)
				for(int j = 0; j < n; ++j)
					if(potentials[j] < INF)
						for(int k = 0; k < edges[j].size(); ++k)
							if(edges[j][k].capacity > 0 && potentials[edges[j][k].to] > potentials[j] + edges[j][k].cost)
								potentials[edges[j][k].to] = potentials[j] + edges[j][k].cost;
		}

        minCost = 0;
        maxFlow = 0;

		while (true)
		{
            f.assign(n, 0);
            distances.assign(n, INF);
            distances[source] = 0;

			while (true)
			{
				int i = -1;
				for(int j = 0; j < n; ++j)
					if(!f[j] && distances[j] < INF && (i < 0 || distances[i] > distances[j]))
						i = j;
				if(i < 0) break;

				f[i] = 1;

				for(int j = 0; j < edges[i].size(); ++j)
					if(edges[i][j].capacity > edges[i][j].flow)
					{
						int a = edges[i][j].to;
						if(!f[a] && distances[a] > distances[i] + edges[i][j].cost + potentials[i] - potentials[a])
						{
                            distances[a] = distances[i] + edges[i][j].cost + potentials[i] - potentials[a];
							from[a] = edges[i][j].backEdge;
						}
					}
			}

			if(!f[target]) break;

			for(int i = 0; i < n; ++i)
				potentials[i] += f[i] ? distances[i] : distances[target];

			int differenceFlow = INF, differenceCost = 0;
			for(int i = target; i != source; )
			{
				int a = edges[i][from[i]].to;
				int b = edges[i][from[i]].backEdge;

				differenceFlow = std::min(differenceFlow, edges[a][b].capacity - edges[a][b].flow);
				differenceCost += edges[a][b].cost;

				i = a;
			}

			for(int i = target; i != source; )
			{
				int a = edges[i][from[i]].to;
				int b = edges[i][from[i]].backEdge;

                edges[a][b].flow += differenceFlow;
                edges[i][from[i]].flow -= differenceFlow;

				i = a;
			}

			maxFlow += differenceFlow;
			minCost += differenceFlow * differenceCost;
		}
	}
};

std::vector<std::vector<std::pair<int, int> > > g;//first - num of Vertex, second - num of edge

std::set<int> badNumbers;

void bfs (int n)
{
    std::queue<int> q;//p - номер вершины, номер ребра
    q.push (1);
    std::vector<bool> used (n);
    std::vector<int> d (n);
    std::vector<std::pair<int, int> > p(n);
    used[1] = true;
    p[1].second = -1;
    p[1].first = -1;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (size_t i = 0;i < g[v].size(); ++i) {
            if (badNumbers.count(g[v][i].second) == 0) {
                int to = g[v][i].first;
                if (!used[to]) {
                    used[to] = true;
                    q.push(to);
                    d[to] = d[v] + 1;
                    p[to].first = v;
                    p[to].second = g[v][i].second;
                }
            }
        }
    }
    std::vector<int> path;
    for (int v = n - 1;p[v].first != -1; v = p[v].first){
        path.push_back(p[v].second);
        badNumbers.insert(p[v].second);
    }
    std::cout << path.size() << " ";
    for (int i = path.size() - 1;i >= 0;--i){
        std::cout << path[i] << " ";
    }
    std::cout << std::endl;
}

int main ()
{
    int n, m, k;
    std::cin >> n >> m >> k;
    Graph graph (n + 1); //0 - source
    for (int i = 0;i < m;++i){
        int a, b, c;
        std::cin >> a >> b >> c;
        graph.addEdge(a, b, 1, c, i);
        graph.addEdge(b, a, 1, c, i);
    }
    graph.addEdge(0, 1, k, 0, INF);
    graph.solve(0, n);
    if (graph.maxFlow < k){
        std::cout << -1 << std::endl;
        return 0;
    }
    double ans = (double)graph.minCost;
    ans /= k;
    std::cout.precision(100);
    std::cout << ans << std::endl;
    //printf ("%.6lf\n", ans);
    g.resize(n + 1);
    for (int i = 1;i < graph.edges.size();++i){
        for (int j = 0;j < graph.edges[i].size();++j){
            Edge e = graph.edges[i][j];
            if (e.flow == e.capacity && e.flow > 0){
                g[i].push_back(std::make_pair(e.to, e.num + 1));
            }
        }
    }
    for (int i = 0;i < k;i++){
        bfs(n + 1);
    }
    return 0;
}