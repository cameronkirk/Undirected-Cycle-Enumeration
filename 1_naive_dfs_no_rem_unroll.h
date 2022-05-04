#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <algorithm>

namespace ndfs_no_rem_unroll {
	class Graph {
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<int> cycles;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<std::vector<int>> adj;
		void Cycles(int node, int depth);
	public:
		Graph(int num_nodes, int search_depth);  // Constructor
		void RemoveNode(int node);
		void DFS(int rootnode_order);
		void AddEdge(int v, int w);


	};

	void Foo(std::string code_name, int depth, int rootnode_order);
}