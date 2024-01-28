#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <algorithm>

namespace szwarcfiter {
	class Graph {
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<long long> cycles;
		alignas(64) std::vector<char> mark;
		alignas(64) std::vector<char> reach;
		alignas(64) std::vector<int> position;
		alignas(64) std::vector<int> stack;

		alignas(64) std::vector<std::vector<int>> adj;
		alignas(64) std::vector<std::vector<int>> B;
		int Cycle(const int node, const int depth);
		void NoCycle(int x, int y);
		void Unmark(int x);
	public:
		Graph(const int num_nodes, const int search_depth);  // Constructor
		void DFS(int rootnode_order);
		void AddEdge(const int v, const int w);
	};
	void Foo(std::string code_name, int depth, int rootnode_order);
}