#include "2_fast_small2large_nodes_deg2.h"
using namespace fast_s2l_deg2;

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
	for (int i = 0; i != graph_size; ++i)
	{
		#include "0_nodes_vis_counter.h"
		if (size(adj[i]) < 2)
			continue;
		rootnode = i;
		std::sort(adj[i].begin(), adj[i].end(), [this](int const i1, int const i2) {	return adj[i1].size() < adj[i2].size();	});
		visited[i] = true;

		while (adj[i].size() > 2)
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
		if (adj[i].size() == 2)
		{
			//We choose the larger root-adjacent node to expand the path with, and the larger to be the returning node.
			int a = adj[i][0];
			int b = adj[i][1];
			int returnAdj, j;
			if (a > b)
			{
				returnAdj = a;
				j = b;
			}
			else
			{
				returnAdj = b;
				j = a;
			}

			//move return node (root-adjacent node) to the first position in adjacent node's lists\
			//Should edit this to be stable (preserve order of all other elements).
			visited[returnAdj] = true;
			visited[j] = true;
			for (auto z : adj[returnAdj])
			{
				if (!visited[z])
					std::iter_swap(std::find(adj[z].begin(), adj[z].end(), returnAdj), adj[z].begin());
			}
			rootnode = returnAdj;
			Cycles(j, 4);
			visited[j] = false;
			visited[returnAdj] = false;

			//Put back in order, if earlier sort is stable, this is unnecessary
			for (auto z : adj[returnAdj])
			{
				if (!visited[z])
					std::sort(adj[z].begin(), adj[z].end());
			}
					
			visited[j] = false;
			adj[j].erase(std::remove(adj[j].begin(), adj[j].end(), i), adj[j].end());
			if (adj[j].size() == 1)
				RemoveNode(j);
			adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), j), adj[i].end());
		}
	RemoveNode(i);
	}
	for (auto k : cycles)
		std::cout << k << ' ';
}

void fast_s2l_deg2::Foo(std::string codeName, int depth) {
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
	//REORDERING
	std::vector<int>reorder;
	int temp_max, temp_max_idx;

	temp_max_idx = -1;
	temp_max = -1;
	std::vector<char>vis(n + k, 0);
	//*****************************
	//Smallest to Largest
	//*****************************
	for (int x = 0; x < n + k; x++)
	{
		temp_max_idx = -1;
		temp_max = 1000;
		for (int y = 0; y < n + k; y++)
		{
			if (vis[y] == false && g.NodeSize(y) > 1)
			{
				if (g.NodeSize(y) < temp_max)
				{
					temp_max_idx = y;
					temp_max = g.NodeSize(temp_max_idx);
				}
			}
		}
		if (temp_max_idx != -1)
		{
			reorder.push_back(temp_max_idx);
			g.RemoveNode(temp_max_idx);
			vis[temp_max_idx] = true;
		}
	}

	for (int x = 0; x < n + k; x++) {
		if (vis[x] == false)
		{
			reorder.push_back(x);
			g.RemoveNode(x);
			vis[x] = true;
		}
	}
	std::vector<int>rev_reorder(n + k);
	for (int x = 0; x < n + k; x++)
		rev_reorder[reorder[x]] = x;
	Graph g2(n + k, depth);
	i = 0;
	z = 0;
	for (int x = n + k; x < n + k + max_row * n; x++)
	{
		if (i == max_row)
		{
			i = 0;	++z;
		}
		if (numbers[x] != 0)
		{
			g2.AddEdge(rev_reorder[z], rev_reorder[numbers[x] + n - 1]);
			g2.AddEdge(rev_reorder[numbers[x] + n - 1], rev_reorder[z]);
		}
		++i;
	}
	std::vector<int>().swap(numbers);
	std::vector<int>().swap(reorder);
	std::vector<int>().swap(rev_reorder);
	g2.Sort();
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g2.DFS();

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}