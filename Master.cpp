//Miscellaneous Notes:
// -----------------------------------
// Why use vector<CHAR> instead of vector<BOOL>?
//		Vector<bool> is actually less performant from testing (5-10%), so we don't use it to maintain visited status. 
//		This is because memory addressing occurs per byte, so it has to use bitshifts to access specific bits.
//		This might change as graphs grow even larger and are harder to contain within the CPU (L1) cache.
// -----------------------------------
// Why does the printout sometimes hang (freeze)?
//		This occurs when you click inside the command window - it's a windows "bug"; 
//		it's meant to be a "feature" to help you copy/paste from the window, or stop the data you need being flushed.
//		As a solution just press any key to resume operation
//		stackoverflow.com/questions/33883530/why-is-my-command-prompt-freezing-on-windows-10
// -----------------------------------
// Why so much repeat code?
//		Wanted to keep each piece compartmentalised and contained - code to remove nodes changes in some methods, 
//		distance collection code is also variable, sometimes need to sort edges, make continuous, etc.
// -----------------------------------
// 
// 
//--------------------------------------------------------------
#include <string>
#include <iomanip>
#include <thread>
#include <future>
//--------------------------------------------------------------
#include "1_naive_dfs.h"
#include "1_naive_dfs_no_rem.h"
#include "1_naive_dfs_no_rem_unroll.h"
#include "1_naive_dfs_no_rem_unroll_size.h"
#include "1_naive_cont.h"
#include "1_naive_cont_no_rem.h"
#include "1_naive_iter.h"
#include "1_naive_iter_no_rem.h"
//--------------------------------------------------------------
#include "1_naive_test.h"
#include "1_naive_multi.h"
//--------------------------------------------------------------

int main()
{
	std::cout << "CYCLE ENUMERATION \n";
	for (int index = 1; index < 9; ++index)
	{
		int depth = 10;
		std::string tempStr;
		switch (index)
		{
		case 1:		tempStr = "Codes/PEGReg252x504";		depth = 12; break;
		case 2:		tempStr = "Codes/PEGirReg252x504";		depth = 10; break;
		case 3:		tempStr = "Codes/816.3.174";			depth = 10; break;
		case 4:		tempStr = "Codes/816.55.178";			depth = 10; break;
		case 5:		tempStr = "Codes/PEGReg504x1008";		depth = 14; break;
		case 6:		tempStr = "Codes/PEGirReg504x1008";		depth = 10; break;
		case 7:		tempStr = "Codes/8000.4000.3.483";		depth = 10; break;
		case 8:		tempStr = "Codes/10000.10000.3.631";	depth = 10; break;
		case 9:		tempStr = "Codes/9633966";				depth = 20; break;
		}
		std::cout << "_______________________________________________________ \n";
		std::cout << tempStr << "\n";

		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	NAIVE RECURSION AND CACHE / BRANCH PREDICTION TESTING
		//_________________________________________________________________________________________________________________________
		//	how much performance can we squeeze out by using better programming techniques (avoiding vector of vectors for continguous arrays, 
		//  combining visited data with node sizes to compress data, etc). This is mostly this is just compressing the data into a single array, 
		//  but we also test an iterative implentation (while-loop instead of recursive function calls).
		if (false)
		{
			//	A naive dft - uses recursive function calls, and vectors of vectors for edgelists 
			//	(each edgelist is in a different location ie. noncontiguous).
			std::cout << std::setw(35) << "Recursive Naive DFS V  :: "; ndfs::Foo(tempStr, depth, 0);


			//	Contiguous array, compare implementations for integrating visited information into the array to increase locality
			std::cout << std::setw(35) << "Contig. Array NDFS    :: "; ndfs_cont::Foo(tempStr, depth);


			//	Iterative naive dft method (uses the same contig array as above).
			std::cout << std::setw(35) << "Iterative Naive DFS   :: "; ndfs_iter::Foo(tempStr, depth);


			//	Multithreaded version of naive search (recursive).
			int numthreads = 4;
			LARGE_INTEGER freq, t1, t2;	QueryPerformanceFrequency(&freq);	QueryPerformanceCounter(&t1);
			std::vector<int> cyclesfound(30, 0);	std::vector<std::future<std::vector<int>>> futures;
			for (int i = 0; i < numthreads; i++)
				futures.emplace_back(std::async(std::launch::async, ndfs_multi::Foo, tempStr, depth, 0, i, numthreads));
			for (auto&& f : futures)
			{
				auto temp = f.get();
				for (int z = 0; z < temp.size(); z++)
					cyclesfound[z] += temp[z];
			}QueryPerformanceCounter(&t2);
			std::cout << std::setw(35) << "Recursive Naive MULTI   :: ";
			for (auto k : cyclesfound)
				std::cout << k / 2 << " ";
			std::cout << ":: " << (t2.QuadPart - t1.QuadPart) * 1.0 / freq.QuadPart << "\n";

		}
		//_________________________________________________________________________________________________________________________
		//	Unexpectedly, removing processed nodes/edges from the graph can slow iteration down depending on the graph (substantially 10-25%) 
		//  For regular graphs, the number of nodes in each list is nice and predictable - size doesn't need to be looked up at each step
		//	Removing edges invalidates this, makes it less predictable
		//_________________________________________________________________________________________________________________________
		// 
		if (false)
		{
			//	Recursive method without node remomval, instead just set each node as visited.
			std::cout << std::setw(35) << "Recursive Naive DFS (no rem)   :: "; ndfs_no_rem::Foo(tempStr, depth, 0);
			std::cout << std::setw(35) << "Recursive Naive DFS (unroll)   :: "; ndfs_no_rem_unroll::Foo(tempStr, depth, 0);


			//	Also include code where the node size is set, faster with gcc -O3, slower on intel compiler or gcc -02.
			//	Only works with 10000.10000 graph. for depth 10: intel = 2.9s; gcc 02 = 3.0; gcc 03 = 2.7s; ndfs_no_rem_unroll = 2.9s 
					//std::cout << std::setw(35) << "Recursive NDFS (fixed size)   :: "; ndfs_no_rem_unroll_size::Foo(tempStr, depth, 0);
			

			//	Contiguous without removal
			std::cout << std::setw(35) << "Contig. Naive DFS (no rem)   :: "; ndfs_cont_no_rem::Foo(tempStr, depth);


			//	Iterative method without removal
			std::cout << std::setw(35) << "Iterative Naive DFS (no rem) :: "; ndfs_iter_no_rem::Foo(tempStr, depth);


		}
		//_________________________________________________________________________________________________________________________
		//	Experimental: Quasi-Hamiltonian Path Compression
		//_________________________________________________________________________________________________________________________
		if (false)
		{
			//	Testing Hamiltonian Path
			//std::cout << std::setw(35) << "Iterative Naive DFS   :: "; ndfs_test::Foo(tempStr, depth);
		}

		//_________________________________________________________________________________________________________________________
	}
}


