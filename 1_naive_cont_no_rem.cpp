#include "1_naive_cont_no_rem.h"
using namespace ndfs_cont_no_rem;

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

void Graph::ConvertVectToArr() {
	cumulative_len.push_back(0);
	for (int i = 1; i < graph_size; i++)
	{
		cumulative_len.push_back(int(cumulative_len.back() + adj[i - 1].size() + 1));
	}
	for (int i = 0; i < graph_size; i++)
	{
		nodes.push_back(int(adj[i].size()));
		for (auto j : adj[i])
		{
			nodes.push_back(cumulative_len[j]);
		}
		adj[i].clear();
	}
	nodes.shrink_to_fit();
}

void Graph::Cycles(int v, int d) 
{
		const int temp = nodes[v];
		auto adjNode = &nodes[v+1];
		nodes[v] = -abs(nodes[v]);
		for (int i = 0; i < temp; ++i,++adjNode)
		{
			#include "0_nodes_vis_counter.h"
			if (*adjNode == rootnode)
			{
				++cycles[d];
			}
			else if (d < max_depth)
			{
				if 	(nodes[*adjNode] > 0)
				{
					Cycles(*adjNode, d + 1);
				}
			}
		}
		nodes[v] = abs(nodes[v]);
}

void Graph::DFS()
{
	auto c = 0;
	int stoppingNode = 2 * graph_size / 3;
	for (int z = 0; z != graph_size; ++z)
	{
		int i = cumulative_len[z];
		rootnode = i;
		if (c >= stoppingNode)
		{
			break;
		}
		c++;
		Cycles(i, 1);
		auto size_i = nodes[i];
		for (auto j = 1; j <= size_i; ++j)
		{
			auto t = nodes.begin() + nodes[i + j];
			std::remove(t + 1, t + *t + 1, i);
			*(t + *t) = 0;
			//--(*t);
			nodes[i + j] = 0;
		}
		nodes[i] = 0;
	}
	cycles[1] = 0; cycles[2] = 0;
	for (auto k : cycles)
		std::cout << k / 2 << ' ';

}

void ndfs_cont_no_rem::Foo(std::string codeName, int depth) {
	//Import data to cpp
	std::ifstream myfile;
	myfile.open(codeName);
	int n, k, max_row, max_col;
	myfile >> n >> k >> max_row >> max_col;
	Graph g = Graph(n + k, depth);
	std::vector<int>numbers;
	int num;
	while (myfile >> num)
		numbers.push_back(num);
	myfile.close();
	//Build graph
	int i = 0, z = 0;
	for (int x = n + k; x < n + k + max_row * n; x++)
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
	g.ConvertVectToArr();
	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS();
	
	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}


