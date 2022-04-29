#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

namespace ndfs_cont {
class Graph {
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<int> cumulative_len;
		alignas(64) std::vector<int> sizes;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<int> cycles;
		alignas(64) std::vector<int> nodes;
		alignas(64) std::vector<std::vector<int>> adj;    // adjacency lists
		void Cycles(const int node, const int depth);
	public:
		Graph(const int num_nodes, const int search_depth);  // Constructor
		void AddEdge(const int v, const int w);
		void DFS();  // counts cycles
		void ConvertVectToArr();
	};

	void Foo(std::string code_name, int depth);
}