#include "1_naive_dfs_no_rem_unroll_size.h"
using namespace ndfs_no_rem_unroll_size;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	visited.resize(num_nodes, false);
	cycles.resize(30, 0);
	max_depth = search_depth;
}

void Graph::AddEdge(int v, int w) {
	adj[v].push_back(w); // Add w to v’s list.
}

void Graph::RemoveNode(int node)
{
	std::vector<int> bfsq = { node };
	while (!bfsq.empty())
	{
		int u = bfsq.back();
		bfsq.pop_back();
		for (auto u2 : adj[u])
		{
			adj[u2].erase(std::remove(adj[u2].begin(), adj[u2].end(), u), adj[u2].end());
			if (size(adj[u2]) == 1)
				bfsq.push_back(u2);
		}
		adj[u].clear();
	}
}

void Graph::Cycles(int v, int d) {
	visited[v] = true;
	std::vector<int>::iterator i = adj[v].begin();
	for (int x = 0; x < 3; ++x, ++i) {
	//for (int i : adj[v]) {
		#include "0_nodes_vis_counter.h"
		if (!visited[*i])
		{
			visited[*i] = true;
			std::vector<int>::iterator j = adj[*i].begin();
			for (int y = 0; y < 6; ++y, ++j) {
			//for (int j : adj[i]) {
				#include "0_nodes_vis_counter.h"
				if (*j == rootnode)
					++cycles[d];
				else if (d + 2 <= max_depth) {
					if (!visited[*j]) {
						Cycles(*j, d + 2);
					}
				}
			}
			visited[*i] = false;
		}
	}
	visited[v] = false;
}

void Graph::DFS(int rootNodeOrder)
{
	int start, end, it;
	switch (rootNodeOrder) {
	case 0:		start = 0;						end = 2 * graph_size / 3;		it =  1;	break;
	case 1:		start = 2 * graph_size / 3 - 1;	end = -1;						it = -1;	break;
	case 2:		start = 2 * graph_size / 3;		end = graph_size;				it =  1;	break;
	case 3:		start = graph_size - 1;			end = 2 * graph_size / 3 - 1;	it = -1;	break;
	default:	start = 0;						end = 2 * graph_size / 3;		it =  1;	break;
	}
	for (int i = start; i != end; i += it)
	{
		rootnode = i;
		Cycles(i, 2);
		visited[i] = true;
		//RemoveNode(i);
		adj[i].clear();
	}
	//Output cycle count, we zero the count of degree-two cycles.
	cycles[1] = 0; cycles[2] = 0;
	for (auto k : cycles)
		std::cout << k / 2 << ' ';
}

void ndfs_no_rem_unroll_size::Foo(std::string codeName, int depth, int rootNodeOrder)
{
	//Import data to cpp
	int n, k, max_row, max_col, temp;
	std::vector<int> numbers;
	std::ifstream myfile;
	myfile.open(codeName);
	myfile >> n >> k >> max_row >> max_col;
	while (myfile >> temp)
		numbers.push_back(temp);
	myfile.close();

	//Build graph
	int alist = 0;
	if (codeName.substr(codeName.size() - 2) == ".a") {
		alist = 1;
	}
	Graph g = Graph(n + k, depth);
	int x = 0;
	int y = n + k;
	while (x < n)
	{
		for (int z = 0; z < max_row; z++)
		{
			if (numbers[y] > 0)
			{
				if (alist) {
					g.AddEdge(x, numbers[y] + n - 1);
					g.AddEdge(numbers[y] + n - 1, x);
				}
				else
				{
					g.AddEdge(x, numbers[y] - 1);
				}

			}
			y++;
		}
		x++;
	}
	std::vector<int>().swap(numbers);

	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS(rootNodeOrder);

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}