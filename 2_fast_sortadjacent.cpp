#include "2_fast_sortadjacent.h"
using namespace fast_sort_adj;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	visited.resize(num_nodes, false);
	cycles.resize(30, 0);
	max_depth = search_depth;
}

void Graph::AddEdge(int v, int w) {
	adj[v].push_back( (unsigned short)w ); // Add w to v’s list.
}

int Graph::NodeSize(int node)
{
	return (int) adj[node].size();
}

void Graph::Sort()
{
	for (int i = 0; i < graph_size; i++)
		std::sort(adj[i].begin(), adj[i].end());

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

void Graph::nodeSwap(int x, int y)
{
	adj[x].swap(adj[y]);
	for (auto j : adj[x])
	{
		for (int k = 0; k < adj[j].size(); ++k)
		{
			if (adj[j][k] == y)
			{
				adj[j][k] = x;
				break;
			}
		}
	}
	for (auto j : adj[y])
	{
		for (int k = 0; k < adj[j].size(); ++k)
		{
			if (adj[j][k] == x)
			{
				adj[j][k] = y;
				break;
			}
		}
	}
}

void Graph::Cycles(int v, int depth, int adjnode) {
	for (auto k : adj[v]) {
		#include "0_nodes_vis_counter.h"
		if (!visited[k]) {
			visited[k] = true;
			for (auto i : adj[k]) {
				#include "0_nodes_vis_counter.h"
				if (!visited[i]) {
					if (depth + 2 < max_depth) {
						visited[i] = true;
						Cycles(i, depth + 2, adjnode);
						visited[i] = false;
					}
					else {
						for (auto j : adj[i])
						{
							#include "0_nodes_vis_counter.h"
							//(4) Using condition node d < adjnode to check if the node at max_depth-1 is an root-adjacent node.
							if (j < adjnode)
							{
								if (adj[j].front() == rootnode)
								{
									if (!visited[j])
									{
										cycles[depth + 2]++;
									}
								}
							}
							else
							{
								//continue;
								break;
							}
						}
					}
				}
				else if (i == rootnode)
					cycles[depth]++;
			}
			visited[k] = false;
		}
	}
}

void Graph::DFS()
{
	for (int startnode = 2 * graph_size / 3; startnode != graph_size; ++startnode){
		//int startnode = i;

	//for (int startnode = 0; startnode < graph_size; startnode++) {
		#include "0_nodes_vis_counter.h"
		if (size(adj[startnode]) < 2)
		{
			RemoveNode(startnode);
			continue;
		}
		//This is a little tricky;
		//For each rootnode we can enumerate all its cycles by exploring n-1 of its n edges, as typically every cycle is found twice, once forwards and once backwards
		//We choose for the skipped edge to be the one of largest size eg. if rootnode was attached to nodes of size [2, 2, 2, 1000] it'd be smarter to NOT ensure that
		//the 100-degree node is explored in the path, ie. branch from a 2-degree node looking for a 100-degree node, rather than branching from a 100-degree node

		//After that; nodes in the path just before the final depth must be a root-adjacent node, we can renumber them to be either higher or lower than the rest of the set
		//so that this can be checked efficiently. In this example we choose for them to be lower.

		//(1) So, we sort the adj list by reverse size of linked nodes
		//(2) Renumber them within the graph as nodes 0 to k
		//(3) And iterate backwards through the graph so that node 0 is chosen last and skipped
		//(4) Using condition node d < adjnode to check if the node at max_depth-1 is an root-adjacent node. See "cycles" function above.

		//(1) So, we sort the adj list by reverse size of linked nodes
		std::sort(adj[startnode].begin(), adj[startnode].end(), [this](short const i1, short const i2) {	return adj[i1].size() > adj[i2].size();	});

		//(2) Renumber them within the graph as nodes 0 to k
		int c = 0;
		for (auto idx = 0; idx < adj[startnode].size(); ++idx)
		{

			int k = adj[startnode][idx];
			int temp = 0;

			while (size(adj[temp+c]) < 2)
				c++;
			nodeSwap(temp + c, k);
			c++;
		}
		std::sort(adj[startnode].begin(), adj[startnode].end());
		for (auto x : adj[startnode])
		{
			std::sort(adj[x].begin(), adj[x].end());
			for (auto y : adj[x])
			{
				std::sort(adj[y].begin(), adj[y].end());
			}
		}

		visited[startnode] = true;
		rootnode = startnode;
		while (adj[startnode].size() > 1)
		{
			//(3) And iterate backwards through the graph so that node 0 is chosen last and skipped
			auto i = adj[startnode].end() - 1;
			#include "0_nodes_vis_counter.h"
			visited[*i] = true;
			for (auto j : adj[*i]) {
				#include "0_nodes_vis_counter.h"
				if (!visited[j]) {
					visited[j] = true;
					Cycles(j, 4, *i);
					visited[j] = false;
				}
			}
			visited[*i] = false;
			adj[*i].erase(std::remove(adj[*i].begin(), adj[*i].end(), startnode), adj[*i].end());
			if (adj[*i].size() == 1)
			{
				RemoveNode(*i);
			}
			adj[startnode].erase(std::remove(adj[startnode].begin(), adj[startnode].end(), *i), adj[startnode].end());
		}
		RemoveNode(startnode);
	}
	for (auto k : cycles)
		std::cout << k << ' ';
}


void fast_sort_adj::Foo(std::string codeName, int depth) {
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
	g.Sort();

	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS();

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}