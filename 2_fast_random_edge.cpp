#include "2_fast_random_edge.h"
using namespace fast_randedge;

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
	std::vector<int> Nodes(graph_size);
	std::iota(Nodes.begin(), Nodes.end(), 0);

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(0, graph_size -1);
	int n1;
	int n2 = 0;
	while (true) {
		

		//To randomly choose a node we have two options - either keep a list of nodes that remain, and randomly choose an index
		//Or we can keep the remaining nodes contiguous, and when removing nodes, swap them with the node at the back of the list
		//before then deleting them. Makes no real speed difference, code is slightly more simple with the former option.
		std::uniform_int_distribution<> dis(0, (int) Nodes.size() - 1);
		n1 = Nodes[dis(gen)];
		while (adj[n1].size() < 2)
		{
			Nodes[n1] = Nodes.back();
			Nodes.pop_back();
			std::uniform_int_distribution<> dis(0, (int) Nodes.size() - 1);
			n1 = Nodes[dis(gen)];
			if (Nodes.size() < 2)
				break;
		}
		if (Nodes.size() <= 2)
			break;

		//Choose second edge at random
		std::uniform_int_distribution<int>  distr(0, (int) adj[n1].size()-1);
		rootnode = n1;
		n2 = adj[rootnode][distr(gen)];

		//Move chosen rootnode into first position of each adjacent-node's list
		for (auto idx : adj[rootnode])
		{
			if (idx != n2)
			{
				auto u = std::find(adj[idx].begin(), adj[idx].end(), n1);
				std::iter_swap(u, adj[idx].begin());
			}
		}

		visited[n1] = true;

		int i = n2;
		#include "0_nodes_vis_counter.h"

		visited[i] = true;
		for (auto j : adj[i]) {
			#include "0_nodes_vis_counter.h"
			if (!visited[j]) {
				visited[j] = true;
				Cycles(j, 4);
				visited[j] = false;
			}
		}
		visited[i] = false;

		adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), n1), adj[i].end());
		adj[n1].erase(std::remove(adj[n1].begin(), adj[n1].end(), i), adj[n1].end());
		if (adj[n1].size() == 1)
			RemoveNode(n1);
		if (adj[i].size() == 1)
			RemoveNode(i);

		visited[n1] = false;
	}
	for (auto k : cycles)
		std::cout << k << ' ';
}


void fast_randedge::Foo(std::string codeName, int depth)
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
	g.Sort();
	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS();

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";	
}
