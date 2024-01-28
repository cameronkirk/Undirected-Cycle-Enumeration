#include "2_fast_bisect.h"
using namespace fast_bisect;

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	visited.resize(num_nodes, false);
	cycles.resize(30, 0);
	max_depth = search_depth;
}


void Graph::AddEdge(int v, int w) {
	adj[v].push_back( (unsigned short)w ); // Add w to v’s list.
}

void Graph::Sort()
{
	for (int i = 0; i < graph_size; i++)
		std::sort(adj[i].begin(), adj[i].end());
}

void Graph::SwapNode(unsigned short n1, unsigned short n2)
{
	//Not optimal, set edges pointing to first node to temp, then do second node, then fix first
	//Need to do this in case a node points to both n1 and n2
	std::vector<unsigned short> temp = adj[n1];
	adj[n1] = adj[n2];
	adj[n2] = temp;
	for(auto k : adj[n1])
		for (auto z = 0; z < adj[k].size(); z++)
		{
			if (adj[k][z] == n2)
				adj[k][z] = graph_size+10;
		}
	for (auto k : adj[n2])
		for (auto z = 0; z < adj[k].size(); z++)
		{
			if (adj[k][z] == n1)
				adj[k][z] = n2;
		}
	for (auto k : adj[n1])
		for (auto z = 0; z < adj[k].size(); z++)
		{
			if (adj[k][z] == graph_size + 10)
				adj[k][z] = n1;
		}
}

void Graph::RemoveNode(int node)
{
	std::vector<int> bfsq = { node };
	while (!bfsq.empty())
	{
		int u = bfsq.back();
		bfsq.pop_back();
		for (auto u2 : adj[u])
		{
			adj[u2].erase(std::remove(adj[u2].begin(), adj[u2].end(), u), adj[u2].end());
			if (size(adj[u2]) == 1)
				bfsq.push_back(u2);
		}
		adj[u].clear();
	}
}

void Graph::Cycles(int v, int depth) {
	for (auto k : adj[v]) {
		#include "0_nodes_vis_counter.h"
		if (!visited[k]) {
			visited[k] = true;
			const auto temp = adj[k].size();
			int x = 0;
			auto i = (adj[k].begin());
			if (*i == rootnode)
			{
				#include "0_nodes_vis_counter.h"
				cycles[depth]++;
				++i;
				++x;
			}
			for (x; x < temp; ++x, ++i) {
				#include "0_nodes_vis_counter.h"
				if (!visited[*i]) {
					if (depth + 2 < max_depth) {
						visited[*i] = true;
						Cycles(*i, depth + 2);
						visited[*i] = false;
					}
					else {
						for (auto j : adj[*i])
						{
							#include "0_nodes_vis_counter.h"
							if (adj[j][0] == rootnode)
							{
								if (!visited[j])
								{
									cycles[depth + 2]++;
								}
							}
						}
					}
				}

			}
			visited[k] = false;
		}
	}
}

void Graph::RemoveVarCross()
{
	std::vector<int> varA1, varA2;
	std::vector<int> varB1, varB2;
	for (auto s = 0; s < 2 * graph_size / 3; s++) {
		int c = 0;
		if (s < graph_size / 3)
		{
			for (auto k : adj[s])
			{
				if (k > 5 * graph_size / 6)
					c++;
			}
			if (c == 2)
				varA1.push_back(s);
			if (c > 2)
				varA2.push_back(s);
		}

		if (s > graph_size / 3)
		{
			for (auto k : adj[s])
			{
				if (k < 5 * graph_size / 6)
					c++;
			}
			if (c == 2)
				varB1.push_back(s);
			if (c > 2)
				varB2.push_back(s);
		}
	}
	varA1.reserve(varA1.size() + varA2.size());
	varA1.insert(varA1.end(), varA2.begin(), varA2.end());

	varB1.reserve(varB1.size() + varB2.size());
	varB1.insert(varB1.end(), varB2.begin(), varB2.end());

	while (!varA1.empty() && !varB1.empty())
	{
		SwapNode(varA1.back(), varB1.back());
		varA1.pop_back();
		varB1.pop_back();
	}
}
void Graph::RemoveParCross()
{
	std::vector<int> parA1, parA2;
	std::vector<int> parB1, parB2;
	for (auto s = 2 * graph_size / 3; s < graph_size; s++) {
		int c = 0;
		if (s < 5 * graph_size / 6)
		{
			for (auto k : adj[s])
			{
				if (k > graph_size / 3)
					c++;
			}
			if (c == 3)
				parA1.push_back(s);
			if (c > 3)
				parA2.push_back(s);
		}

		if (s > 5 * graph_size / 6)
		{
			for (auto k : adj[s])
			{
				if (k < graph_size / 3)
					c++;
			}
			if (c == 3)
				parB1.push_back(s);
			if (c > 3)
				parB2.push_back(s);
		}
	}
	parA1.reserve(parA1.size() + parA2.size());
	parA1.insert(parA1.end(), parA2.begin(), parA2.end());

	parB1.reserve(parB1.size() + parB2.size());
	parB1.insert(parB1.end(), parB2.begin(), parB2.end());

	while (!parA1.empty() && !parB1.empty())
	{
		SwapNode(parA1.back(), parB1.back());
		parA1.pop_back();
		parB1.pop_back();
	}
}
void Graph::CheckCross()
{
	std::vector<int> crossings(graph_size, 0);
	for (int startnode = 0; startnode < 2 * graph_size / 3; startnode++) {
		for (auto k : adj[startnode])
		{
			if (startnode < graph_size / 3)
			{
				if (k > 5 * graph_size / 6)
					crossings[startnode]++;

			}
			if (startnode > graph_size / 3)
			{
				if (k < 5 * graph_size / 6)
					crossings[startnode]++;
			}
		}
	}

	std::vector<int> crossDist(10, 0);
	for (int startnode = 0; startnode < 2 * graph_size / 3; startnode++) {
		crossDist[crossings[startnode]]++;
	}

	std::cout << "\n CROSSINGS VAR: ";
	for (auto k : crossDist)
		std::cout << k << " ";

	std::fill(crossings.begin(), crossings.end(), 0);
	for (int startnode = 2 * graph_size / 3; startnode < graph_size; startnode++) {
		for (auto k : adj[startnode])
		{
			if (startnode < 5 * graph_size / 6)
			{
				if (k > graph_size / 3)
					crossings[startnode]++;

			}
			if (startnode > 5 * graph_size / 6)
			{
				if (k < graph_size / 3)
					crossings[startnode]++;
			}
		}
	}

	std::fill(crossDist.begin(), crossDist.end(), 0);
	for (int startnode = 2 * graph_size / 3; startnode < graph_size; startnode++) {
		crossDist[crossings[startnode]]++;
	}

	std::cout << "\n CROSSINGS PAR: ";
	for (auto k : crossDist)
		std::cout << k << " ";
	
	int sum = 0, idx=0, esum = 0;
	for (auto k : crossDist)
		sum += k*idx++;
	for (int s = 0; s < graph_size; s++)
		esum += int(adj[s].size());
	std::cout << " | Edge Crossings: " << sum << " / " << esum/2;

}
void Graph::DFS()
{
	for (int reps = 0; reps < 4; reps++)
	{
		//CheckCross();
		RemoveVarCross();
		RemoveParCross();
	}

	for (int startnode = 0; startnode < 2*graph_size / 3; startnode++) {
		#include "0_nodes_vis_counter.h"
		if (size(adj[startnode]) < 2) {
			continue;
		}
		rootnode = startnode;
		visited[startnode] = true;

		for (auto i : adj[startnode])
		{
			if ((startnode < graph_size / 3 && i > 5 * graph_size / 6) || (startnode > graph_size / 3 && i < 5 * graph_size / 6))
			{
				for (auto idx : adj[startnode])
				{
					if (idx != i)
					{
						auto u = std::find(adj[idx].begin(), adj[idx].end(), startnode);
						std::iter_swap(u, adj[idx].begin());
					}
				}
				visited[i] = true;
				for (auto j : adj[i])
				{
					#include "0_nodes_vis_counter.h"
					if (!visited[j])
					{
						visited[j] = true;
						Cycles(j, 4);
						visited[j] = false;
					}
				}
				visited[i] = false;
				#include "0_nodes_print_counter.h"
				adj[i].erase(std::remove(adj[i].begin(), adj[i].end(), startnode), adj[i].end());
				if (adj[i].size() == 1)
					RemoveNode(i);
				adj[startnode].erase(std::remove(adj[startnode].begin(), adj[startnode].end(), i), adj[startnode].end());
				if (adj[startnode].size() == 1)
					RemoveNode(startnode);				
			}
		}
	
		visited[startnode] = false;
	}
	//std::cout << "\n";
	//for (auto k : cycles)
	//	std::cout << k << ' ';
	//std::cout << "\n";

	Sort();

	for (int startnode = 0; startnode < 2*graph_size / 3; startnode++) {
		#include "0_nodes_vis_counter.h"
		if (size(adj[startnode]) < 2) {
			RemoveNode(startnode);
			adj[startnode].clear();
			continue;
		}
		std::sort(adj[startnode].begin(), adj[startnode].end(), [this](short const i1, short const i2) {	return adj[i1].size() < adj[i2].size();	});
		rootnode = startnode;
		visited[startnode] = true;
		while (adj[startnode].size() > 1)
		{
			auto i = adj[startnode].begin();

			#include "0_nodes_vis_counter.h"
			visited[*i] = true;
			for (auto j : adj[*i]) 
			{
				#include "0_nodes_vis_counter.h"
				if (!visited[j]) 
				{
					visited[j] = true;
					Cycles(j, 4);
					visited[j] = false;
				}
			}
			visited[*i] = false;

			#include "0_nodes_print_counter.h"
			adj[*i].erase(std::remove(adj[*i].begin(), adj[*i].end(), startnode), adj[*i].end());
			if (adj[*i].size() == 1)
				RemoveNode(*i);
			adj[startnode].erase(std::remove(adj[startnode].begin(), adj[startnode].end(), *i), adj[startnode].end());
		}
		RemoveNode(startnode);
		adj[startnode].clear();
	}
	for (auto k : cycles)
		std::cout << k << ' ';
}

void fast_bisect::Foo(std::string codeName, int depth)
{
	//Import data to cpp
	int n, k, max_row, max_col, temp;
	std::vector<int> numbers;
	std::ifstream myfile;
	myfile.open(codeName);
	myfile >> n >> k >> max_row >> max_col;
	while (myfile >> temp)
		numbers.push_back(temp);
	myfile.close();

	//Build graph
	int alist = 0;
	if (codeName.substr(codeName.size() - 2) == ".a") {
		alist = 1;
	}
	Graph g = Graph(n + k, depth);
	int x = 0;
	int y = n + k;
	while (x < n)
	{
		for (int z = 0; z < max_row; z++)
		{
			if (numbers[y] > 0)
			{
				if (alist) {
					g.AddEdge(x, numbers[y] + n - 1);
					g.AddEdge(numbers[y] + n - 1, x);
				}
				else
				{
					g.AddEdge(x, numbers[y] - 1);
				}

			}
			y++;
		}
		x++;
	}
	std::vector<int>().swap(numbers);

	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);
	g.DFS();

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}