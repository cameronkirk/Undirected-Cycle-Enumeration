#include "0_1970_Tiernan.h"
using namespace tiernan;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	cycles.resize(20, 0);
	max_depth = search_depth;
}

void Graph::AddEdge(int v, int w) {
	adj[v].push_back(w); // Add w to v’s list.
}

void Graph::DFS(int rootNodeOrder)
{
	std::vector<std::vector<int> > H(graph_size);
	std::vector<int> P(graph_size,-1);
	int k = 1;
	
	for (int i = 0; i != graph_size; i++)
	{
		P.clear();
		P.push_back(i);
		k = 0;
		H[0].clear();
		while (!P.empty())
		{
		EC2:
			int n = P[k];
			for (auto v : adj[P[k]])
			{
				if ((v > P[0]) && (std::find(P.begin(), P.end(), v) == P.end()) && (std::find(H[n].begin(), H[n].end(), v) == H[n].end()))
				{
					k++;
					P.push_back(v);
					goto EC2;
				}
			}
			if (std::find(adj[P[k]].begin(), adj[P[k]].end(), P[0]) != adj[P[k]].end()) {
				cycles[k+1]++;
			}
			if (k == 0)
				break;
			else
			{
				H[P[k]].clear();
				H[P[k - 1]].push_back(P[k]);
				P.pop_back();
				k--;
			}

		}
	}
	//Output cycle count, we zero the count of degree-two cycles.
	cycles[0] = 0; cycles[1] = 0; cycles[2] = 0;
	for (auto k : cycles) {
		std::cout << k / 2 << ' ';
	}std::cout << "\n";
}

void tiernan::Foo(std::string codeName, int depth, int rootNodeOrder)
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
				}else
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