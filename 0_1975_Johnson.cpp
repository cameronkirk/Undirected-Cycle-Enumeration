#include "0_1975_Johnson.h"
using namespace johnson;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	//B2.resize(num_nodes, std::vector<char>(num_nodes, 0));
	B.resize(num_nodes);
	visited.resize(num_nodes, false);
	blocked.resize(num_nodes, 0);
	cycles.resize(search_depth+1, 0);
	max_depth = search_depth;
}

void Graph::AddEdge(int v, int w) {
	adj[v].push_back(w); // Add w to v’s list.
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

void Graph::Unblock(int u)
{
	blocked[u] = false;
	while (!B[u].empty())
	{
		int w = B[u].back();
		B[u].pop_back();
		//B2[u][w] = 0;
		if (blocked[w])
		{
			Unblock(w);
		}
	}

}

int Graph::Cycles(int v, int d)
{
	int f = false;
	blocked[v] = true;
	for (int w : adj[v])
	{
		cycles[0]++;
		if (w == rootnode)
		{
			cycles[d]++;
			f = true;
		}
		else if (!blocked[w])
		{
			if (d < max_depth)
			{
				if (Cycles(w, d + 1))
				{
					f = true;
				}
			}
			else
				f = true;
		}
	}
	if (f)
		Unblock(v);
	else
	{
		for (int w : adj[v])
		{
			if (std::find(B[w].begin(), B[w].end(), v) == B[w].end())
			{
				B[w].push_back(v);
			}
			//if (B2[w][v] == 0)
			//{
			//	B[w].push_back(v);
			//	B2[w][v] = 1;
			//}
		}
	}
	return f;
}


void Graph::DFS(int rootNodeOrder)
{
	for (int i = 0; i != graph_size; i++)
	{
		//for (int i2 = i; i2 != graph_size; i2++) {
		//	if (blocked[i2])
		//	{
		//		std::cout << "ERROR";
		//		B[i2].clear();
		//		std::fill(B2[i2].begin(), B2[i2].end(), 0);
		//		blocked[i2] = false;
		//	}
		//}
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
	for (auto k : cycles) {
		std::cout << k / 2 << ' ';
	}
}

void johnson::Foo(std::string codeName, int depth, int rootNodeOrder)
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
	while (y < n + k + max_row * n)
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
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS(rootNodeOrder);

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}