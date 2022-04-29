#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <algorithm>

namespace ndfs_multi {
	class Graph {
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<int> cycles;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<std::vector<int>> adj;
		void Cycles(const int node, const int depth);
	public:
		Graph(const int num_nodes, const int search_depth);  // Constructor
		void DFS(int rootnode_order, int offset, int numthreads);
		void AddEdge(const int v, const int w);
		std::vector<int> ReturnCycles();
	};
	std::vector<int> Foo(std::string code_name, int depth, int rootnode_order, int offset, int numthreads);
}