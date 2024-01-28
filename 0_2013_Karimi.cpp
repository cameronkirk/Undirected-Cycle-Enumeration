#include "0_2013_Karimi.h"
using namespace karimi;

Graph::Graph(unsigned short num_nodes, unsigned short search_depth) {
	this->graph_size = num_nodes;
	Uadj.resize(num_nodes * 2 / 3);
	Wadj.resize(num_nodes / 3);

	visited.resize(num_nodes, false);
	blocked.resize(num_nodes, 0);
	cycles.resize(100, 0);
	max_depth = search_depth;
}

void Graph::AddUndirectedEdge(unsigned short v, unsigned short w) {
	Uadj[v].push_back(w); // Add w to v’s list.
	Wadj[w].push_back(v); // Add w to v’s list.

}

void Graph::DFS(unsigned short max_node_size)
{
	unsigned short n = (unsigned short) Uadj.size();
	unsigned short m = (unsigned short) Wadj.size();
	std::vector< std::vector<std::vector<unsigned short>>> M_uw(max_node_size, std::vector< std::vector<unsigned short>>(n, std::vector<unsigned short>(m, 0)));
	std::vector< std::vector<std::vector<unsigned short>>> M_wu(max_node_size, std::vector< std::vector<unsigned short>>(m, std::vector<unsigned short>(n, 0)));

	unsigned short g = 6;

	std::vector<long long> counter(g, 0);

	for (unsigned short i = 0; i < n; i++)
	{
		unsigned short iSize = (unsigned short) Uadj[i].size();
		for (unsigned short i3 = 0; i3 < m; i3++)
			std::sort(Wadj[i3].rbegin(), Wadj[i3].rend());

		for (unsigned short z = i; z < n; z++)
		{
			for (auto x : Uadj[z])
			{
				for (unsigned short c = 0; c < iSize; c++)
				{
					M_uw[c][z][x] = 0;
					M_wu[c][x][z] = 0;

				}
			}
		}
	
		//INITIALISATION
		for (unsigned short c = 0; c < iSize; c++)
		{
			M_uw[c][i][Uadj[i][c]] = 1;
		}

		Uadj[i].clear();
		std::vector<unsigned short> temp(iSize, 0);

		for (unsigned short k = 0; k < g - 1; k++)
		{

			//MESSAGE PASSING FROM W
			
			for (unsigned short c = 0; c < iSize; c++)
			{
				for (unsigned short j = 0; j < m; j++)
				{
					unsigned short temp = 0;
					for (auto h : Wadj[j])
					{
						if (h < i)	break;
						temp += M_uw[c][h][j];
					}
					for (auto i2 : Wadj[j])
					{
						if (i2 < i)	break;
						M_wu[c][j][i2] = temp - M_uw[c][i2][j];
					}
				}
			}

			for (unsigned short c = 0; c < iSize; c++)
			{
				M_uw[c][i][Uadj[i][c]] = 0;
			}

			for (unsigned short c = 0; c < iSize; c++)
				for (unsigned short c2 = 0; c2 < iSize; c2++)
					if(c!=c2)
						counter[k] += M_wu[c][Uadj[i][c2]][i];

			//MESSAGE PASSING FROM U
			for (unsigned short c = 0; c < iSize; c++)
			{
				for (unsigned short i2 = i + 1; i2 < n; i2++)
				{
					unsigned short temp = 0;
					for (auto h : Uadj[i2])
					{
						temp += M_wu[c][h][i2];
					}
					for (auto z : Uadj[i2])
					{
						M_uw[c][i2][z] = temp - M_wu[c][z][i2];
					}
				}
			}
		}
		for (auto p : Uadj[i])
			Wadj[p].erase(std::remove(Wadj[p].begin(), Wadj[p].end(), i), Wadj[p].end());

	}

	//Output cycle count, we zero the count of degree-two cycles.
	//counter[1] = 0; counter[2] = 0;
	for (auto k : counter)
		std::cout << k / 2 << ' ';
	//std::cout << counter[1];

}

void karimi::Foo(std::string codeName, unsigned short depth, unsigned short rootNodeOrder)
{
	//Import data to cpp
	unsigned short n, k, max_row, max_col, temp;
	std::vector<unsigned short> numbers;
	std::ifstream myfile;
	myfile.open(codeName);
	myfile >> n >> k >> max_row >> max_col;
	while (myfile >> temp)
		numbers.push_back(temp);
	myfile.close();

	//Build graph
	unsigned short alist = 0;
	if (codeName.substr(codeName.size() - 2) == ".a") {
		alist = 1;
	}
	Graph g = Graph(n + k, depth);
	int x = 0;
	int y = n + k;
	while (y < n + k + max_row * n)
	{
		for (int z = 0; z < max_row; z++)
		{
			if (numbers[y] > 0)
			{
				if (alist) {
					g.AddUndirectedEdge(x, numbers[y] - 1);
				}
				else
				{
					g.AddUndirectedEdge(x, numbers[y] - 1);
				}

			}
			y++;
		}
		x++;
	}
	std::vector<unsigned short>().swap(numbers);

	//Search for cycles, and start timers
	LARGE_INTEGER freq, t1, t2;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&t1);

	g.DFS(max_row);

	QueryPerformanceCounter(&t2);
	std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";
}