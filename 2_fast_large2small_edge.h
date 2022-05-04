#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cstdint>
#include <functional>
#include <windows.h>

namespace fast_l2s_edge {
	class Graph {
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<long long> cycles;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<std::vector<int>> NodeSizes;
		alignas(64) std::vector<std::vector<unsigned short>> adj;
		void Cycles(const int node, const int depth);
	public:
		Graph(const int num_nodes, const int search_depth);  // Constructor
		void Sort();
		void DFS();
		void AddEdge(const int v, const int w);
		void RemoveNode(int node);
		int	NodeSize(int node);
	};
	void Foo(std::string code_name, int depth);
}