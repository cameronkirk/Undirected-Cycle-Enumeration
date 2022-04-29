#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <windows.h>

namespace ndfs_iter {
class Graph
	{
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<int> nodes;
		alignas(64) std::vector<int> sizes;
		alignas(64) std::vector<long> cycles;
		alignas(64) std::vector<int> cumulative_len;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<std::vector<int>> adj;    // adjacency lists
	public:
		Graph(const int num_nodes, const int search_depth);  // Constructor
		void AddEdge(const int v, const int w);
		void DFS();  // counts cycles
		void ConvertVectToArr();
	};
	void Foo(std::string codeName, int depth);
}