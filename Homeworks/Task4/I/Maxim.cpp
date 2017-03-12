#include <vector>
#include <algorithm>
#include <iostream>


const int INF = 1000*1000*1000;

class Edge
{
public:
    Edge(int to, int backEdge, int capacity, int cost) : to(to), backEdge(backEdge), flow(0), capacity(capacity), cost(cost) {}

	int to;			// номер вершины, в которую идет ребро
	int backEdge;			// номер обратного к данному ребру ребра, нумерация конечно в списке вершины next
	int flow;			// текущий поток
	int capacity;			// пропускная способность ребра
	int cost;			// стоимость потока по данному ребру
};


class Graph
{
public:
	int n;				
	std::vector<std::vector<Edge> > edges;	// исходящие ребра, соответственно по одному вектору ребер на каждую вершину
	std::vector<int> potentials;		// вектор потенциалов, по одному потенциалу на каждую вершину :)
	std::vector<int> distances;			// вектор текущих расстояний для алгоритма Дейкстры
	std::vector<int> f;			// вектор флажков для алгоритма Дейкстры
	std::vector<int> from;		// вектор, по которому восстанавливается путь после алгоритма Дейкстры; точное
														// определение - номер ребра в списке ребер для данной вершины, по которому
														// надо перейти, чтобы попасть в вершину-предшественника

	Graph(int n) : n(n), edges(n), potentials(n), distances(n), f(n), from(n) {}

	void addEdge(int a, int b, int capacity, int cost)
	{
		edges[a].push_back(Edge(b, edges[b].size(), capacity, cost));
		edges[b].push_back(Edge(a, edges[a].size() - 1, 0, -cost));
	}

	int minCost(int source, int target)
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

		int resultFlow = 0, resultCost = 0;

		while (true)
		{
            f.assign(n, 0);
			fill(f.begin(), f.end(), 0);
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

			resultFlow += differenceFlow;
			resultCost += differenceFlow * differenceCost;
		}

		return resultCost;
	}
};

int main ()
{
    int n, k, graphSize;
    std::cin >> n >> k;
    graphSize = n * 2 + 2;
    Graph graph (graphSize);//0..(n-1) - first dole, n..(2n-1) - second dole,
    for (int i = 0;i < n;++i){ //graphSize - 2 - source, graphSize - 1 - target
        for (int j = 0;j < n;++j){
            int a;
            std::cin >> a;
            graph.addEdge(i, n + j, 1, a);
        }
    }
    for (int i = 0;i < n;++i){
        graph.addEdge(graphSize - 2, i, k, 0);
    }
    for (int j = 0;j < n;++j){
        graph.addEdge(n + j, graphSize - 1, k, 0);
    }
    std::cout << graph.minCost(graphSize - 2, graphSize - 1) << std::endl;


    std::vector<std::vector<int> > table;
    table.resize(n + 1);
    for (int i = 0;i <= n;i++){
        table[i].assign(n + 1, 3);
    }
    for (int i = 0;i <= n;++i){
        table[i][0] = 0;
        table[0][i] = 0;
    }
    for (int i = 0;i < n;i++){
        for (int j = 0;j < graph.edges[i].size();j++){
            Edge edge = graph.edges[i][j];
            if (edge.flow == 1){ //edge from i to edge.to is saturated
                if (edge.cost >= 0 && edge.flow == 1) {
                    table[i + 1][edge.to - n + 1] = 1;
                }
            }
        }
    }
    std::vector<int> u(n + 1);
    std::vector<int> v(n + 1);
    std::vector<int> parents(n + 1);
    std::vector<int> way(n + 1);
    std::vector<int> ans(n + 1);

    for (int z = 0;z < k;++z) {
        u.assign(n + 1, 0);
        v.assign(n + 1, 0);
        parents.assign(n + 1, 0);
        way.assign(n + 1, 0);
        ans.assign(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            parents[0] = i;
            int freeCol = 0;
            std::vector<int> minv(n + 1, INF);
            std::vector<char> used(n + 1, false);
            do {
                used[freeCol] = true;
                int freeRow = parents[freeCol];
                int delta = INF;
                int freeCol1;
                for (int j = 1; j <= n; ++j) {
                    if (!used[j]) {
                        int current = table[freeRow][j] - u[freeRow] - v[j];
                        if (current < minv[j]) {
                            minv[j] = current;
                            way[j] = freeCol;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            freeCol1 = j;
                        }
                    }
                }
                for (int j = 0; j <= n; ++j) {
                    if (used[j]) {
                        u[parents[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                freeCol = freeCol1;
            } while (parents[freeCol] != 0);
            do {
                int freeCol1 = way[freeCol];
                parents[freeCol] = parents[freeCol1];
                freeCol = freeCol1;
            } while (freeCol);
        }
        for (int j = 1; j <= n; ++j) {
            ans[parents[j]] = j;
        }
        for (int i = 1;i <= n;++i){
            std::cout << ans[i] << " ";
        }
        std::cout << std::endl;
        for (int i = 1;i <= n;i++){
            table[i][ans[i]] = 3;
        }
    }
}