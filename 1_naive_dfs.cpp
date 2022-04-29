#include "1_naive_dfs.h"
using namespace ndfs;
#define ENABLE_DEBUG
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

void Graph::Cycles(int v, int d) {
	
	//if (!visited[v])
	{
		visited[v] = true;
		for (int i : adj[v]) {
			#include "0_nodes_vis_counter.h"
			if (i == rootnode) {
				++cycles[d];
			}
			else if (d < max_depth) {
				if (!visited[i]) {
					Cycles(i, d + 1);
				}
			}
		}
		visited[v] = false;
	}
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
		Cycles(i, 1);
		visited[i] = true;
		//Remove edges from adjacent nodes that point towards the root-node, then clear root-node
		for (auto z : adj[i])
			adj[z].erase(std::remove(adj[z].begin(), adj[z].end(), i), adj[z].end());
		adj[i].clear();
	}
	//Output cycle count, we zero the count of degree-two cycles.
	cycles[1] = 0; cycles[2] = 0;
	for (auto k : cycles)
		std::cout << k / 2 << ' ';
}

void ndfs::Foo(std::string codeName, int depth, int rootNodeOrder)
{
	//Import data to cpp
	std::ifstream myfile;
	myfile.open(codeName);
	int n, k, max_row, max_col;
	myfile >> n >> k >> max_row >> max_col;
	std::vector<int>numbers;
	int num;
	while (myfile >> num)
		numbers.push_back(num);
	myfile.close();
	//Build graph
	Graph g = Graph(n + k, depth);
	int i = 0, z = 0;
	for (auto x = n + k; x < n + k + max_row * n; x++)
	{
		if (i == max_row)
		{
			i = 0;
			z++;
		}
		if (numbers[x] != 0)
		{
			g.AddEdge(z, numbers[x] + n - 1);
			g.AddEdge(numbers[x] + n - 1, z);
		}
		i++;
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