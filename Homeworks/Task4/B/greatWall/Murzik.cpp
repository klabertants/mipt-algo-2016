#include <bits/stdc++.h>

using namespace std;

struct Edge {
	int u;
	int v;
	int c;
	int f;

	int r() {
		return c - f;
	}
};

struct Graph {
	Graph(int n) : n(n), m(0), first(n, -1) {}

	int n;
	int m;
	int flow = 0;
	vector<Edge> edges;
	vector<int> next;
	vector<int> first;
	vector<bool> present;

	void add_edge(int u, int v, int c) {
		assert(u < n && v < n);
		add_edge_internal(u, v, c);
		add_edge_internal(v, u, 0);
	}

	void add_edge_internal(int u, int v, int c) {
		edges.push_back({u, v, c, 0});
		next.push_back(first[u]);
		first[u] = (int) next.size() - 1;
		present.push_back(true);
		++m;
	}

	int first_edge(int u) {
		int i = first[u];
		for (; i != -1 && !present[i]; i = next[i]);
		return i;
	}

	int next_edge(int i) {
		assert(i != -1);
		i = next[i];
		for (; i != -1 && !present[i]; i = next[i]);
		return i;
	}
};

bool getLevelGraph(Graph &g, int s, int t) {
	int n = g.n;
	vector<int> level(n, -1);
	vector<int> curr = {s}, next;
	for (int i = 0; !curr.empty(); ++i) {
		for (int u : curr) {
			if (level[u] == -1) {
				level[u] = i;
				for (int j = g.first[u]; j != -1; j = g.next_edge(j)) {
					Edge e = g.edges[j];
					int v = e.v;
					if (e.r() > 0 && level[v] == -1) {
						next.push_back(v);
					}
				}
			}
		}
		curr.clear();
		swap(next, curr);
	}

	for (int u = 0; u < n; ++u) {
		for (int j = g.first[u]; j != -1; j = g.next_edge(j)) {
			Edge e = g.edges[j];
			int v = e.v;
			g.present[j] = level[u] + 1 == level[v];
		}
	}
	return level[t] != -1;
}

int dfs(Graph &g, vector<int> &curr, int t, int u, int max_flow) {
	dbgt(u, max_flow);
	if (max_flow == 0) {
		return 0;
	}
	if (u == t) {
		return max_flow;
	}
	for (; curr[u] != -1; curr[u] = g.next_edge(curr[u])) {
		Edge &e = g.edges[curr[u]];
		dbgtt(curr[u], e.r(), e.u, e.v);
		int flow = dfs(g, curr, t, e.v, min(max_flow, e.r()));
		if (flow > 0) {
			e.f += flow;
			g.edges[curr[u] ^ 1].f -= flow;
			return flow;
		}
	}
	return 0;
}

void createBlockingFlow(Graph &g, int s, int t) {
	int n = g.n;
	vector<int> curr;
	for (int u = 0; u < n; ++u) {
		curr.push_back(g.first_edge(u));
	}
	while (int flow = dfs(g, curr, t, s, INT_MAX)) {
		dbg(flow);
		g.flow += flow;
	}
}

bool used[100000];

void dfs2(Graph &g, int u) {
	if (used[u]) {
		return;
	}
	used[u] = 1;
	for (int i = g.first_edge(u); i != -1; i = g.next_edge(i)) {
		Edge &e = g.edges[i];
		if (e.r() > 0) {
			dfs2(g, e.v);
		}
	}
}

int ts[100][100];

const int inf = INT_MAX / 4;

int main() {

	int n, m, k, l;
	cin >> n >> m >> k >> l;

	for (int i = 0; i <= n + 1; ++i) {
		for (int j = 0; j <= m + 1; ++j) {
			ts[i][j] = 1;
		}
	}
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			ts[i][j] = 0;
		}
	}

	while (k--) {
		int x, y;
		cin >> x >> y;
		ts[x][y] = 1;
	}
	while (l--) {
		int x, y;
		cin >> x >> y;
		ts[x][y] = 2;
	}
	int xa, ya, xb, yb;
	cin >> xa >> ya >> xb >> yb;

	Graph g((n + 1) * (m + 1) * 2);
	const int di[] = {-1, 0, 0, +1};
	const int dj[] = {0, -1, +1, 0};
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (ts[i][j] != 1) {
				if (ts[i][j] == 2) {
					g.add_edge((i * m + j) * 2 + 1, (i * m + j) * 2, 1);
				}
				for (int k = 0; k < 4; ++k) {
					int ni = i + di[k];
					int nj = j + dj[k];
					if (ts[ni][nj] != 1) {
						g.add_edge((i * m + j) * 2, (ni * m + nj) * 2 + (ts[ni][nj] == 2), inf);
					}
				}
			}
		}
	}

	int s = (xa * m + ya) * 2;
	int t = (xb * m + yb) * 2;
	while (1) {
		dbg(g.flow);
		g.present.assign(g.m * 2, true);
		if (!getLevelGraph(g, s, t)) {
			break;
		}
		createBlockingFlow(g, s, t);
	}

	dfs2(g, s);
	vector<pair<int, int>> ans;
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (ts[i][j] == 2 && used[(i * m + j) * 2] != used[(i * m + j) * 2 + 1]) {
				ans.push_back({i, j});
			}
		}
	}

	if (g.flow >= inf){
		cout << -1 << endl;
		return 0;
	}

	cout << ans.size() << endl;
	for (auto p : ans) {
		cout << p.first << " " << p.second << endl;
	}
	return 0;
}