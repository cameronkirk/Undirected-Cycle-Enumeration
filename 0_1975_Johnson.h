#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <algorithm>

namespace johnson {
	class Graph {
	private:
		int graph_size;
		int max_depth;
		int rootnode = 0;
		alignas(64) std::vector<long long> cycles;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<char> blocked;
		alignas(64) std::vector<std::vector<int>> adj;
		alignas(64) std::vector<std::vector<int>> B;
		alignas(64) std::vector<std::vector<char>> B2;
		alignas(64) std::vector<std::vector<char>> Badj;
		int Cycles(const int node, const int depth);
		void Unblock(int k);
		void RemoveNode(int node);
	public:
		Graph(const int num_nodes, const int search_depth);  // Constructor
		void DFS(int rootnode_order);
		void AddEdge(const int v, const int w);
	};
	void Foo(std::string code_name, int depth, int rootnode_order);
}