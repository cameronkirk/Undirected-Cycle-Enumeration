#include "1_naive_multi.h"
using namespace ndfs_multi;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	visited.resize(num_nodes, false);
	cycles.resize(30, 0);
	max_depth = search_depth;
}

std::vector<int> Graph::ReturnCycles() {
	return cycles;
}
void Graph::AddEdge(int v, int w) {
	adj[v].push_back(w); // Add w to v’s list.
}

void Graph::Cycles(int v, int d)
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

void Graph::DFS(int rootNodeOrder, int offset, int numthreads)
{
	int start, end, it;
	switch (rootNodeOrder) {
	case 0:		start = 0;						end = 2 * graph_size / 3;		it = 1;	break;
	case 1:		start = 2 * graph_size / 3 - 1;	end = -1;						it = -1;	break;
	case 2:		start = 2 * graph_size / 3;		end = graph_size;				it = 1;	break;
	case 3:		start = graph_size - 1;			end = 2 * graph_size / 3 - 1;	it = -1;	break;
	default:	start = 0;						end = 2 * graph_size / 3;		it = 1;	break;
	}
	start += offset;
	it *= numthreads;
	for (int x = 0; x < offset; x++)
	{
		visited[x] = true;
		for (auto z : adj[x])
			adj[z].erase(std::remove(adj[z].begin(), adj[z].end(), x), adj[z].end());
		adj[x].clear();
	}
	for (int i = start; i < end; i += it)
	{
		rootnode = i;
		visited[i] = true;
		Cycles(i, 1);

		//Remove edges from adjacent nodes that point towards the root-node, then clear root-node
		for (int x = 0; x < numthreads; x++)
		{
			visited[i + x] = true;
			for (auto z : adj[i + x])
				adj[z].erase(std::remove(adj[z].begin(), adj[z].end(), i + x), adj[z].end());
			adj[i + x].clear();
		}
	}
	cycles[1] = cycles[2] = 0;
}

std::vector<int> ndfs_multi::Foo(std::string codeName, int depth, int rootNodeOrder, int offset, int numthreads)
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
	g.DFS(rootNodeOrder, offset, numthreads);
	return g.ReturnCycles();
}