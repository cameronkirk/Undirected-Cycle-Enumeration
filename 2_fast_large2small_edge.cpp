#include "2_fast_large2small_edge.h"
using namespace fast_l2s_edge;

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
			size_t x = 0;
			auto i = adj[k].begin();
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
	NodeSizes.resize(30);
	for (int x = 0; x < graph_size; ++x)
	{
		NodeSizes[adj[x].size()].push_back(x);
	}
	int index = 29;

	while (true) {
		//Each node is sorted into layers by their maximum degree, this isn't actively updated as edges are removed, instead we only update from the top down as we search for the current maximum degree node.
		//If a node has degree that matches the layer, then it has equal maximum degree, so we choose that node.
		//Otherwise, we remove node from the current layer and push it onto the correct layer.

		int startnode, adjnode;
		do	{
			while (NodeSizes[index].empty()) {
				index--;
			}
			startnode = NodeSizes[index].back();
			NodeSizes[adj[startnode].size()].push_back(startnode);
			NodeSizes[index].pop_back();
		} while (adj[startnode].size() != index);
		
		if (index < 2)	{
			break;
		}

		////Iterate through each maximum degree node to find the one linked to the lowest degree node, and select that edge.
		////Doesn't really help that much, maybe a few percent - causes time to blow-out for large graphs (iterating over full set is costly)
		////Doesn't do anything for the early nodes whilst the graph remains regular.
		////However, overall it seems to reduce nodes visited by 2-5%, but the reverse - taking the maximum degree adjacent node reduces perf ~10%.
		int bestNodeHigh = startnode;
		int bestNodeLow = adj[startnode][0];
		size_t lowest = adj[adj[startnode][0]].size();

		//for (auto it = NodeSizes[index].end(), itB = NodeSizes[index].begin(), itE = NodeSizes[index].end(); it != itB;)
		for (auto it = NodeSizes[index].end(), itB = NodeSizes[index].begin(), itE = NodeSizes[index].end(); it != itB && itE - it < 100;)
		{
			if (adj[*it].size() != index)
			{
				NodeSizes[adj[*it].size()].push_back(*it);
				*it = NodeSizes[index].back();
				NodeSizes[index].pop_back();
				--it;				//it = NodeSizes[index].erase(it);
			}
			else
			{
				for (auto k : adj[*it])
				{
					if (adj[k].size() < lowest)
					{
						lowest = adj[k].size();
						bestNodeHigh = *it;
					}
				}
				--it;
				if (lowest == 2)
					break;
			}
		}
		startnode = bestNodeHigh;

		//Sort adj of selected startnode by size, and choose lowest as root-adjacent node to path from.
		std::sort(adj[startnode].begin(), adj[startnode].end(), [this](short const i1, short const i2) {	return adj[i1].size() < adj[i2].size();	});
		adjnode = adj[startnode][0];

		//Move rootnode to first entry in adjacency lists
		for (auto idx : adj[startnode])
		{
			if (idx != adjnode)
			{
				auto u = std::find(adj[idx].begin(), adj[idx].end(), startnode);
				std::iter_swap(u, adj[idx].begin());
			}
		}
		
		visited[startnode] = true;
		rootnode = startnode;
		int i = adjnode;

		if (cycles[0] == 5)
		{
			std::cout << "\n|" << startnode << "|=" << adj[startnode].size() << " :: ";
			for (auto k : adj[startnode])
			{
				std::cout << adj[k].size() << " ";
			}std::cout << " :: |" << i << "|=" << adj[i].size() << "\n";
		}


		visited[i] = true;
		for (auto j: adj[i]) {
			if (!visited[j]) {
				visited[j] = true;
				Cycles(j, 4);
				visited[j] = false;
			}
		}
		#include "0_nodes_print_counter.h"
		adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), startnode), adj[i].end());
		if (adj[i].size() == 1)
			RemoveNode(i);
		visited[i] = false;

		adj[startnode].erase(std::remove(adj[startnode].begin(), adj[startnode].end(), i), adj[startnode].end());
		if (adj[startnode].size() == 1)
			RemoveNode(startnode);
		visited[startnode] = false;
	}
	for (auto k : cycles)
		std::cout << k << ' ';
}


void fast_l2s_edge::Foo(std::string codeName, int depth)
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

	//REORDERING
	std::vector<int>reorder;
	int temp_max, temp_max_idx;
	//int temp_min, temp_min_idx;

	temp_max_idx = -1;
	temp_max = -1;
	std::vector<char>vis(n + k, 0);
	for (int x = 0; x < n + k; x++)
	{
		temp_max_idx = -1;
		temp_max = -1;
		for (int y = 0; y < n + k; y++)
		{
			if (vis[y] == false && g.NodeSize(y) > 1)
			{
				if (g.NodeSize(y) > temp_max)
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
	int i = 0;
	int z = 0;
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
