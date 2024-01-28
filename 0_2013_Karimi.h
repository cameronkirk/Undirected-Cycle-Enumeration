#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <numeric>

namespace karimi {
	class Graph {
	private:
		unsigned short graph_size;
		unsigned short max_depth;
		unsigned short rootnode = 0;
		alignas(64) std::vector<long long> cycles;
		alignas(64) std::vector<char> visited;
		alignas(64) std::vector<char> blocked;
		alignas(64) std::vector<std::vector<unsigned short>> Uadj;
		alignas(64) std::vector<std::vector<unsigned short>> Wadj;
		unsigned short Cycles(const unsigned short node, const unsigned short depth);
		void Unblock(unsigned short k);
		void RemoveNode(unsigned short node);
	public:
		Graph(const unsigned short num_nodes, const unsigned short search_depth);  // Constructor
		void DFS(unsigned short max_message_size);
		void AddUndirectedEdge(const unsigned short v, const unsigned short w);
	};
	void Foo(std::string code_name, unsigned short depth, unsigned short rootnode_order);
}