#include "2_fast_dfs.h"
using namespace fast_dfs;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	visited.resize(num_nodes, false);
	cycles.resize(30, 0);
	max_depth = search_depth;
}

void Graph::AddEdge(int v, int w) {
	adj[v].push_back( (unsigned short)w );; // Add w to v’s list.
}

int Graph::NodeSize(int node)
{
	return (int) adj[node].size();
}

void Graph::Sort(int rootNodeOrder)
{
	if (rootNodeOrder%2==1) 
	{
		for (int i = 0; i < graph_size; i++)
			std::sort(adj[i].rbegin(), adj[i].rend());
	}
	else
	{
		for (int i = 0; i < graph_size; i++)
			std::sort(adj[i].begin(), adj[i].end());
	}

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


void Graph::Cycles(int v, int depth) {
	for (auto k : adj[v]) {
		#include "0_nodes_vis_counter.h"
		if (!visited[k]) {
			visited[k] = true;


			const auto temp = adj[k].size();
			int x = 0;
			auto i = (adj[k].begin());
			if (*i == rootnode)
			{
				cycles[depth]++;
				++i;
				++x;
			}
			
			
			for (x; x < temp; ++x, ++i) {
			//for (auto i : adj[k]) {
				#include "0_nodes_vis_counter.h"
				if (!visited[*i]) {
					if (depth + 2 < max_depth) {
						visited[*i] = true;
						Cycles(*i, depth + 2);
						visited[*i] = false;
					}
					else {
						for (auto j : adj[*i])
						{
							#include "0_nodes_vis_counter.h"
							if (adj[j][0] == rootnode)
							{
								if (!visited[j])
								{
									cycles[depth+2]++;
								}
							}
						}
					}
				}
				 
			}
			visited[k] = false;
		}
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
		#include "0_nodes_vis_counter.h"
		if (size(adj[i]) < 2)
			continue;
		rootnode = i;
		std::sort(adj[i].begin(), adj[i].end(), [this](int const i1, int const i2) {	return adj[i1].size() < adj[i2].size();	});
		visited[i] = true;
		while (adj[i].size() > 1)
		{
			auto j = *adj[i].begin();
			#include "0_nodes_vis_counter.h"
			visited[j] = true;
			for (auto k : adj[j]) 
			{
				#include "0_nodes_vis_counter.h"
				if (!visited[k]) 
				{
					visited[k] = true;
					Cycles(k, 4);
					visited[k] = false;
				}
			}
			visited[j] = false;

			adj[j].erase(std::remove(adj[j].begin(), adj[j].end(), rootnode), adj[j].end());
			if (adj[j].size() == 1)
				RemoveNode(j);
			adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), j), adj[i].end());
		}
		RemoveNode(i);
	}
	for (auto k : cycles)
		std::cout << k << ' ';
}

void fast_dfs::Foo(std::string codeName, int depth, int rootNodeOrder)
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
	g.Sort(rootNodeOrder);

	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS(rootNodeOrder);

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}