#include "1_naive_iter.h" 
using namespace ndfs_iter;
//__declspec(noalias)

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

void Graph::DFS()
{
	std::vector< int> currentPath(max_depth*10);
	std::vector< int> adjPosition(max_depth+2);
	std::vector<int>::iterator it, temp;
	int currentNode;
	int depth;
	int c = 0;
	int stoppingNode = 2 * graph_size / 3;
	int StackSize;
	for (auto i : cumulative_len)
	{
		if (c++ >= stoppingNode)
			break;
		depth=1;
		adjPosition[0] = nodes[i];
		StackSize = nodes[i];
		for ( int x = 1; x <= nodes[i]; ++x)
		{
			#include "0_nodes_vis_counter.h"
			currentPath[x] = nodes[i+x];
		}
		nodes[i] = -abs(nodes[i]);
		while (StackSize != 0)
		{
			currentNode = currentPath[StackSize];
			++depth;
			temp = nodes.begin() + nodes[currentNode] + currentNode;

			for (it = nodes.begin() + currentNode + 1; it <= temp; ++it)
			{
				#include "0_nodes_vis_counter.h"
				if (*it == i)
				{
					++cycles[depth];
				}
				else if (depth < max_depth)
				{
					{
						if (nodes[*it] > 0)
						{
							currentPath[++StackSize] = *it;
							++adjPosition[depth];
						}
					}
				}
			}
			if (adjPosition[depth] != 0)
			{
				nodes[currentNode] = -abs(nodes[currentNode]);
			}
			else
			{
				--adjPosition[--depth];
				currentNode = currentPath[--StackSize];
				while (adjPosition[depth] == 0)
				{
					nodes[currentNode] = abs(nodes[currentNode]);
					currentNode = currentPath[--StackSize];
					--adjPosition[--depth];
				}
			}
		}
		int size_i = abs(nodes[i]);
		for (int j = 1; j <= size_i; ++j)
		{
			auto t = nodes.begin() + nodes[i + j];
			static_cast<void>(std::remove(t + 1, t + *t + 1, i));
			*(t + *t) = 0;
			--(*t);
			nodes[i + j] = 0;
		}
		nodes[i] = 0; 	
	}
	cycles[1] = 0; cycles[2] = 0;
	for (auto k : cycles)
		std::cout << k / 2 << ' ';
}

void ndfs_iter::Foo(std::string codeName, int depth) 
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
	g.ConvertVectToArr();
	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS();

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}

