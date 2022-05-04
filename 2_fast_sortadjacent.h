#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <windows.h>

namespace fast_sort_adj
{
	class Graph {
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<long long> cycles;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<std::vector<unsigned short>> adj;
		void Cycles(const int node, const int depth, const int adjnode);
	public:
		Graph(const int num_nodes, const int search_depth);  // Constructor
		void Sort();
		void nodeSwap(int x, int y);
		void DFS();
		void AddEdge(const int v, const int w);
		void RemoveNode(int node);
		int	NodeSize(int node);
	};
	void Foo(std::string code_name, int depth);
}