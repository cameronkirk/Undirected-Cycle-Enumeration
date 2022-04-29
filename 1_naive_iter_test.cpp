#include "1_naive_iter_test.h" 
using namespace ndfs_iter_test;
//__declspec(noalias)

Graph::Graph(int num_nodes, int search_depth) {
	this->graph_size = num_nodes;
	adj.resize(num_nodes);
	visited.resize(num_nodes, false);
	cycles.resize(30, 0);
	max_depth = search_depth;
}

void Graph::AddEdge(int v, int w) {
	adj[v].push_back(w); // Add w to v’s list.
}

void Graph::ConvertVectToArr() {
	cumulative_len.push_back(0);
	for (int i = 1; i < graph_size; i++)
	{
		cumulative_len.push_back(int(cumulative_len.back() + adj[i - 1].size() + 1));
	}
	for (int i = 0; i < graph_size; i++)
	{
		nodes.push_back(int(adj[i].size()));
		for (auto j : adj[i])
		{
			nodes.push_back(cumulative_len[j]);
		}
		adj[i].clear();
	}
	nodes.shrink_to_fit();
}

void Graph::Cycles(int v, int d){
}

void Graph::DFS()
{
	//LARGE_INTEGER freq, t1, t2;
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&t1);
	//stack of positions in each list; stack of sizes
	std::vector< int> currentPath(max_depth*10);
	std::vector< int> adjPosition(max_depth+2);
	//std::vector<int>::iterator temp;
	std::vector<int>::iterator it, temp;
	int currentNode;
	char depth;
	int c = 0;
	int stoppingNode = 2 * graph_size / 3;
	int StackSize;
	cycles2.resize(30, 0);
	std::vector<char> visited(nodes.size(), 0);
	for (auto i : cumulative_len)
	{
		if (c++ >= stoppingNode)
			break;
		//if (c++ <= stoppingNode && c > 1)
		//	continue;
		depth=1;
		adjPosition[0] = nodes[i];
		StackSize = nodes[i];
		for ( int x = 1; x <= nodes[i]; ++x)
		{
			currentPath[x] = nodes[i+x];
		}

		//nodes[i] |= (1UL << 21);
		//nodes[i] *= -1;
		//nodes[i] = -abs(nodes[i]);
		visited[i] = true;

		while (StackSize != 0)
		{
			currentNode = currentPath[StackSize];
			++depth;
			//++cycles2[0];

			temp = nodes.begin() + nodes[currentNode] + currentNode;
			
			for (it = nodes.begin() + currentNode + 1; it <= temp; ++it)
			{
				//++cycles2[1];
				if (*it == i)
				{
					++cycles[depth];
					//break;
				}
				else if (depth < max_depth)
				{
					//++cycles2[2];
					//if (!((unsigned int)nodes[*it] >= (unsigned int)128))
					//if (nodes[*it] < 128 && nodes[*it] >0)
					//if (*it > i)
					
						//++cycles2[3];
						//if (!visited[*it])
						
							//StackSize++;
							//++cycles2[4];
							//++StackSize;
							
					
							//int temp = 1 - ;
						currentPath[StackSize+1] = *it;	
						//adjPosition[depth]+= 1-visited[*it];
						//StackSize += 1-visited[*it];
						adjPosition[depth] += visited[*it] ? 0 : 1;
						StackSize += visited[*it] ? 0  : 1;
					

				}
			}
			//for (it++; it <= temp; ++it)
			//{
			//	if (depth < max_depth)
			//	{
			//		//++cycles2[2];
			//		//if (!((unsigned int)nodes[*it] >= (unsigned int)128))
			//		//if (nodes[*it] < 128 && nodes[*it] >0)
			//		//if (*it > i)
			//		{
			//			//++cycles2[3];
			//			if (nodes[*it] > 0)
			//			{
			//				//++cycles2[4];
			//				currentPath[++StackSize] = *it;
			//				++adjPosition[depth];
			//			}
			//		}

			//	}
			//}

			if (adjPosition[depth] != 0)
			{
				//++cycles2[5];
				//nodes[currentNode] |= (1UL << 21);
				//nodes[currentNode] *= -1;
				visited[currentNode] = true;
				//nodes[currentNode] = -abs(nodes[currentNode]);
			}
			else
			{
				//++cycles2[6];
				--adjPosition[--depth];
				currentNode = currentPath[--StackSize];

				while (adjPosition[depth] == 0 && depth > 0)
				{
					//++cycles2[7];
					//nodes[currentNode] &= ~(1UL << 21);
					//nodes[currentNode] *= -1;
					visited[currentNode] = false;
					//nodes[currentNode] = abs(nodes[currentNode]);
					currentNode = currentPath[--StackSize];
					--adjPosition[--depth];
				}
			}
			
		}

		//++cycles2[8];
		//nodes[i] *= -1;
		//nodes[i] &= ~(1UL << 21);
		visited[i] = false;
		int size_i = nodes[i];
		
		for (int j = 1; j <= size_i; ++j)
		{
			//int size_j = nodes[nodes[i + j]];
			//--nodes[nodes[i + j]];
			//for (int k = 1; k <= size_j; ++k)
			//	std::cout << nodes[nodes[i + j] + k] << " ";
			//std::cout << "\n";
		

			//for (int k = 1; k <= size_j; ++k)
			//	std::cout << nodes[nodes[i + j] + k] << " ";
			//std::cout << "\n";
			//succint version
			//auto k = nodes[nodes[i + j]];
			auto t = nodes.begin() + nodes[i + j];
			std::remove(t + 1, t + *t + 1, i);
			*(t + *t) = 0;
			//nodes[nodes[i + j]]--;
			//nodes[nodes[i + j]]--;
			//nodes[nodes[i + j]]--;
			//nodes[nodes[i + j]]--;
			//if (*t == 6)
			{
				//k--;
				
				//*t = k-1;
				//std::cout <<c << " "<< * t << " " << i << " " << nodes[i + j] << "\n";
			}
			//std::cout << *t << "\n";
			//std::cout << "\n";
			nodes[i + j] = 0;
			//std::cout << *t << "\n";
		}
		nodes[i] = 0; 
		visited[i] = true;
		//if ((++c % 200)==0) {
		//	QueryPerformanceCounter(&t2);
		//std::cout <<c<< " :: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
		//}
		//if (c++ >= 10)
		

	}
	cycles[1] = cycles[2] = 0;
	for (auto k : cycles)
		std::cout << k / 2 << ' ';
	//std::cout << "\n";
	//for (auto k : cycles2)
	//	std::cout << k / 2 << ' ';
}

void ndfs_iter_test::Foo(std::string codeName, int depth)
{
	//Import data to cpp
	std::ifstream myfile;
	myfile.open(codeName);

	int n, k, max_row, max_col;
	myfile >> n >> k >> max_row >> max_col;

	std::vector<int>numbers;
	int num;
	while (myfile >> num)
		numbers.push_back(num);
	myfile.close();
	//Build graph
	Graph g = Graph(n + k, depth);
	int i = 0, z = 0;
	for (int x = n + k; x < n + k + max_row * n; x++)
	{
		if (i == max_row)
		{
			i = 0;
			z++;
		}
		if (numbers[x] != 0)
		{
			g.AddEdge(z, numbers[x] + n - 1);
			g.AddEdge(numbers[x] + n - 1, z);
		}
		i++;
	}
	std::vector<int>().swap(numbers);
	g.ConvertVectToArr();

	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS();

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
	
	//end
}

