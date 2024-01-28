#include <string>
#include "0_alist_export.h"
using namespace alistexport;

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

void Graph::AlistExport() {
	std::ofstream myfile("Graphs/W_" + std::to_string(graph_size));
	//std::ofstream myfile("RealWorld/TEMP_310");

	for (int i = 0; i < graph_size; i++)
		std::sort(adj[i].begin(), adj[i].end());

	myfile << graph_size << " 0\n";

	int max = 0;
	for (int i = 0; i < graph_size; i++)
	{
		if (adj[i].size() > max)
			max = static_cast<int>(adj[i].size());
	}
	myfile << max << " 0\n";

	for (int i = 0; i < graph_size; i++)
		myfile << adj[i].size() << " ";
	myfile << "\n";
	for (int i = 0; i < graph_size; i++)
	{
		myfile << "\n";
		//if (adj[i].size() == 0)
		//	myfile << "0";
		int c = 0;
		for (auto k : adj[i])
		{
			myfile << k + 1 << " ";
			c++;
		}
		while (c < max) {
			myfile << "0 ";
			c++;
		}
	}
	myfile.close();
}

void Graph::SortEdges() {
	for (int i = 0; i < graph_size; i++)
	{
		std::sort(adj[i].begin(), adj[i].end());
	}
}

void Graph::RemoveEdges(int k) {
	for (int i = 0; i < graph_size; i++)
	{
		int edge_multiplicity = 1;
		auto j = adj[i].begin();
		while (j < adj[i].end() - 1)
		{
			if (*j == *(j + 1))
			{
				edge_multiplicity++;
				j++;
			}
			else if (edge_multiplicity < k)
			{
				auto f = *j;
				adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), f), adj[i].end());
				adj[f].erase(std::remove(adj[f].begin(), adj[f].end(), i), adj[f].end());
				j = adj[i].begin();
				edge_multiplicity = 1;
			}
			else
			{
				j++;
				edge_multiplicity = 1;
			}
		}
		if (edge_multiplicity < k)
		{
			auto f = *j;
			adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), f), adj[i].end());
			adj[f].erase(std::remove(adj[f].begin(), adj[f].end(), i), adj[f].end());
		}
	}

}
void Graph::UniqueEdges() {
	for (int i = 0; i < graph_size; i++)
	{
		std::sort(adj[i].begin(), adj[i].end());
		adj[i].erase(std::unique(adj[i].begin(), adj[i].end()), adj[i].end());
	}
	int c1 = 0; int c2 = 0;
	for (int i = 0; i < graph_size; i++)
	{
		if (adj[i].size() > 0)
		{
			c1++;
			c2 += static_cast<int>(adj[i].size());
		}
	}std::cout << "Nodes: " << c1 << " Edges: " << c2 << "\n";
}

void Graph::Cycles(int v, int d) {
	visited[v] = true;
	for (int i : adj[v]) {
#include "0_nodes_vis_counter.h"
		if (i == rootnode) {
			++cycles[d];
		}
		else if (d < max_depth) {
			if (!visited[i]) {
				Cycles(i, d + 1);
			}
		}
	}
	visited[v] = false;
}

void Graph::DFS(int rootNodeOrder)
{
	for (int i = 0; i < graph_size; i++)
		std::sort(adj[i].begin(), adj[i].end());
	int start, end, it;
	switch (rootNodeOrder) {
	case 0:		start = 0;						end = graph_size;		it = 1;	break;
	case 1:		start = 2 * graph_size / 3 - 1;	end = -1;						it = -1;	break;
	case 2:		start = 2 * graph_size / 3;		end = graph_size;				it = 1;	break;
	case 3:		start = graph_size - 1;			end = 2 * graph_size / 3 - 1;	it = -1;	break;
	default:	start = 0;						end = 2 * graph_size / 3;		it = 1;	break;
	}
	for (int i = start; i != end; i += it)
	{
		rootnode = i;
		Cycles(i, 1);
		visited[i] = true;
		//Remove edges from adjacent nodes that point towards the root-node, then clear root-node
		for (auto z : adj[i])
			adj[z].erase(std::remove(adj[z].begin(), adj[z].end(), i), adj[z].end());
		adj[i].clear();
	}
	//Output cycle count, we zero the count of degree-two cycles.
	//cycles[1] = 0; cycles[2] = 0; cycles[0] = 0;
	for (auto k : cycles) {
		//cycles[1] += k;
		std::cout << k << ' ';

	}//std::cout << cycles[1];
}

void alistexport::Foo(std::string codeName, int depth, int rootNodeOrder)
{

	//LARGE_INTEGER freq, t1, t2;
	////Import data to cpp
	//std::ifstream myfile;
	//int bipartiteData;
	////codeName = "RealWorld/INFECTIOUS"; bipartiteData = true;
	//codeName = "RealWorld/ACTORS3"; bipartiteData = false;
	//myfile.open(codeName);
	//int n, k, E;
	//myfile >> E >> n >> k;
	//std::vector<int>numbers;
	//QueryPerformanceFrequency(&freq); 	QueryPerformanceCounter(&t1);


	//Importing alist/graph to vector.
	
	////SLOW 
	//int num1, num2;
	//while (myfile >> num1 >> num2) {
	//	//std::cout << num1 << " " << num2 << "\n";
	//	numbers.push_back(num1);
	//	numbers.push_back(num2);
	//}
	
	//FASTER (10x)
	//int num = 0;
	//std::ifstream fin(codeName);
	//const int z = 8192;
	//char buffer[z];
	//int bytes = 0;
	//char* p = buffer;;
	//int eof = 0;
	//int inputnum = false;
	//while (!eof)
	//{
	//	fin.read(buffer, sizeof(buffer));
	//	p = buffer;
	//	bytes = z;
	//	while (bytes > 0)
	//	{
	//		if (*p > 32)
	//		{
	//			num = num * 10 + *p - '0';
	//			inputnum = true;
	//		}else if (*p == 26)   // End of file marker
	//		{
	//			if(inputnum)
	//				numbers.push_back(num);
	//			eof = true;
	//			break;
	//		}
	//		else if (*p == '\n' || *p == ' ')
	//		{
	//			if (inputnum)
	//				numbers.push_back(num);
	//			inputnum = false;
	//			num = 0;
	//		}
	//	p++;
	//	bytes--;
	//	}	memset(buffer, 26, sizeof(buffer));  //fill buffer with escape character, so if fin.read doesn't fill buffer, rest of buffer is escape character.
	//}
	//myfile.close();
	//fin.close();
	//QueryPerformanceCounter(&t2);	std::cout << "\nImport Graph Data :: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
	//Graph g = Graph(n+k, 4);

	//QueryPerformanceFrequency(&freq);	QueryPerformanceCounter(&t1);
	//for (int i = 3; i < numbers.size(); i = i + 2)
	//{
	//	g.AddEdge(numbers[i] - 1, numbers[i + 1] - 1);
	//	if (bipartiteData) {	
	//		g.AddEdge(numbers[i + 1] - 1, numbers[i] - 1); 
	//	}
	//}
	//std::vector<int>().swap(numbers); //clear memory

	//QueryPerformanceCounter(&t2);	std::cout << "Create Graph / Add Edges :: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
	//
	////Search for cycles, and start timers
	//QueryPerformanceFrequency(&freq);	QueryPerformanceCounter(&t1);
	//g.SortEdges();
	////instance# from paper to edge removal multiplicity keymap
	////we remove all edges that occur k times to get instance n.
	////INSTANCE: 1  2  3  4  5  6  7  8
	////INFECT:   0  5  10 20 40 60 80 103
	//// 
	////INSTANCE: 1  4  7 12 17 22 23 24 25 26 27 28 29 30 31
	////ACTORS:   0  4  7 12 17 22 23 24 26 28 30 33 37 43
	//g.RemoveEdges(56);
	//g.UniqueEdges();
	//g.AlistExport();
	//QueryPerformanceCounter(&t2);	std::cout << "Trim Edges (reduce multiplicity):: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";

	//QueryPerformanceFrequency(&freq);	QueryPerformanceCounter(&t1);
	//g.DFS(rootNodeOrder);
	//QueryPerformanceCounter(&t2);	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";





	for (int depth = 6; depth < 21; depth++)
	{
		Graph g = Graph(depth, depth);
		int i = 0, z = 0;

	//	////COMPLETE GRAPH
	//	//int N = depth;
	//	//for (auto i = 0; i < N; i++)
	//	//{
	//	//	for (auto j = 0; j < N; j++) 
	//	//	{
	//	//		if (i != j)
	//	//			g.AddEdge(i, j);
	//	//	}
	//	//}
	//
		//WHEEL GRAPH
		int N = depth - 1;
		for (auto i = 2; i < N; i++)
		{
			g.AddEdge(i, i + 1);
			g.AddEdge(i, i - 1);
		}
		g.AddEdge(1, N);
		g.AddEdge(1, 2);
		g.AddEdge(N, 1);
		g.AddEdge(N, N - 1);
		for (auto i = 1; i <= N; i++)
		{
			g.AddEdge(0, i);
			g.AddEdge(i, 0);
		}

		g.AlistExport();
	}

}