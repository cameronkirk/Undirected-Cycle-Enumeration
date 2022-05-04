#include "2_fast_bisect.h"
using namespace fast_bisect;

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
#include "0_nodes_vis_counter.h"
				cycles[depth]++;
				++i;
				++x;
			}
			for (x; x < temp; ++x, ++i) {
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
									cycles[depth + 2]++;
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

void Graph::DFS()
{
	for (int startnode = 0; startnode < 2 * graph_size / 3; startnode++) {
		#include "0_nodes_vis_counter.h"
		int i = startnode;
		if (adj[i].size() == 3 && adj[i][0] < 5 * graph_size / 6)
		{
			int k2;
			if (adj[i][1] > 5 * graph_size / 6)
			{
				//cycles[1]++;
				k2 = adj[i][0];
			}
			else if (adj[i][1] < 5 * graph_size / 6 && adj[i][2] > 5 * graph_size / 6)
			{
				//cycles[1]++;
				k2 = adj[i][2];
			}
			else
			{
				
				continue;
				
			}

			for (auto j : adj[i])
			{
				if (j != k2)
				{
					auto z = std::find(adj[j].begin(), adj[j].end(), i);
					std::iter_swap(adj[j].begin(), z);
				}
			}

			visited[i] = true;
			visited[k2] = true;
			for (auto j : adj[k2])
			{
				#include "0_nodes_vis_counter.h"
				if (!visited[j])
				{
					rootnode = i;
					visited[j] = true;
					Cycles(j, 4);
					visited[j] = false;
				}
			}
			visited[k2] = false;
			visited[i] = false;

			adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), k2), adj[i].end());
			adj[k2].erase(std::remove(adj[k2].begin(), adj[k2].end(), i), adj[k2].end());

			int u, u2;
			std::queue<int> bfsq;
			if (size(adj[i]) == 1) {
				bfsq.push(i);
			}
			if (size(adj[k2]) == 1) {
				bfsq.push(k2);
			}
			while (!bfsq.empty())
			{
				u = bfsq.front();
				u2 = adj[u].front();
				bfsq.pop();
				adj[u2].erase(std::remove(adj[u2].begin(), adj[u2].end(), u), adj[u2].end());
				if (size(adj[u2]) == 1)
					bfsq.push(u2);
				adj[u].clear();
			}

		}
	}


	Sort();


	for (int startnode = 0; startnode < graph_size * 2 / 3; startnode++) {
		#include "0_nodes_vis_counter.h"
		if (size(adj[startnode]) < 2)
			continue;

		std::sort(adj[startnode].begin(), adj[startnode].end(), [this](short const i1, short const i2) {	return adj[i1].size() < adj[i2].size();	});
		rootnode = startnode;
		visited[startnode] = true;
		while (adj[startnode].size() > 1)
		{
			auto i = adj[startnode].begin();

			#include "0_nodes_vis_counter.h"
			visited[*i] = true;
			for (auto j : adj[*i]) 
			{
				#include "0_nodes_vis_counter.h"
				if (!visited[j]) 
				{
					visited[j] = true;
					Cycles(j, 4);
					visited[j] = false;
				}
			}
			visited[*i] = false;

			adj[*i].erase(std::remove(adj[*i].begin(), adj[*i].end(), startnode), adj[*i].end());
			if (adj[*i].size() == 1)
				RemoveNode(*i);
			adj[startnode].erase(std::remove(adj[startnode].begin(), adj[startnode].end(), *i), adj[startnode].end());
		}
		RemoveNode(startnode);
		adj[startnode].clear();
	}
	for (auto k : cycles)
		std::cout << k << ' ';
}

void fast_bisect::Foo(std::string codeName, int depth)
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

	g.DFS();

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}