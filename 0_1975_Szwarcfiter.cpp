#include "0_1975_Szwarcfiter.h"
using namespace szwarcfiter;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	B.resize(num_nodes);
	mark.resize(num_nodes, 0);
	reach.resize(num_nodes, 0);
	position.resize(num_nodes, graph_size+1);
	cycles.resize(100, 0);
}

void Graph::AddEdge(int v, int w) {
	adj[v].push_back(w); // Add w to v’s list.
}

void Graph::NoCycle(int x, int y) {
	B[y].push_back(x);
	adj[x].erase(find(adj[x].begin(), adj[x].end(), y));
}

void Graph::Unmark(int x) {
	mark[x] = false;
	for (auto y : B[x]) {
		adj[y].push_back(x);
		if (mark[y])
			Unmark(y);
	}
	B[x].clear();
}

int Graph::Cycle(int v, int q) {
	mark[v] = true; 
	char f = false;
	stack.push_back(v);
	int t = (int) stack.size();
	position[v] = t;
	if (!reach[v])
	{
		q = t;
	}
	//!!! WE ADD THIS TEMP VECTOR SO THAT WE DON'T INVALIDATE OUR ITERATORS - ADJ[V] WILL BE EDITED BY NOCYCLE/UNMARK; AND WE'D LOSE OUR PLACE WITHOUT OPERATING ON A SNAPSHOT-COPY. !!!
	std::vector<int> temp = adj[v];
	for (auto w : temp) {

		if (mark[w] == false) {
			if (Cycle(w, q))
				f = true;
			else
				NoCycle(v, w);
		}
		else if (position[w] <= q)
		{
			//!!! Note that we zero index, unlike szwarcfiter's paper.
			if (stack[position[w]] <  v)
			{
				cycles[0]++;
				cycles[stack.size() - position[w]]++;
			}
			f = true;
		}
		else {
			NoCycle(v, w);
		}
	}
	stack.pop_back();
	if (f)
		Unmark(v);
	reach[v] = true;
	position[v] = graph_size+1;
	return f;
}

void Graph::DFS(int rootNodeOrder)
{
	Cycle(0, 0);
	//Output cycle count, we zero the count of degree-two cycles.
	//cycles[0] = 0; cycles[1] = 0; cycles[2] = 0;
	for (auto k : cycles) {
		std::cout << k << ' ';
	}std::cout << "\n";
}

void szwarcfiter::Foo(std::string codeName, int depth, int rootNodeOrder)
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