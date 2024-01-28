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
#include "1_naive_multi.h"
//--------------------------------------------------------------
#include "2_fast_dfs.h"
#include "2_fast_large2small_nodes.h"
#include "2_fast_small2large_nodes.h"
#include "2_fast_small2large_nodes_deg2.h"
#include "2_fast_large2small_edge.h"
#include "2_fast_sortadjacent.h"
#include "2_fast_bisect.h"
#include "2_fast_random_edge.h"
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
			if(index == 8)
				std::cout << std::setw(35) << "Recursive NDFS (fixed size)   :: "; ndfs_no_rem_unroll_size::Foo(tempStr, depth, 0);

			//	Contiguous without removal
			std::cout << std::setw(35) << "Contig. Naive DFS (no rem)   :: "; ndfs_cont_no_rem::Foo(tempStr, depth);

			//	Iterative method without removal
			std::cout << std::setw(35) << "Iterative Naive DFS (no rem) :: "; ndfs_iter_no_rem::Foo(tempStr, depth);
		}
		//_________________________________________________________________________________________________________________________
		//	Experimental, Hamiltonian Path
		//_________________________________________________________________________________________________________________________
		if (false)
		{
			//	Testing Hamiltonian Path
		}
		//_________________________________________________________________________________________________________________________


		//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
		//	REORDERING TESTING
		//_________________________________________________________________________________________________________________________
		//	how does node (or edge) iteration order affect the overall number of nodes visited and time taken?
		if (true)
		{
			//	We note that the choice of iterating over either the parity or variable nodes, in either forwards or reverse makes a substantial  
			//	difference in the runtime
			std::cout << std::setw(35) << "Recursive Naive DFS V  :: "; ndfs::Foo(tempStr, depth, 0); //vector of vectors
			std::cout << std::setw(35) << "Recursive Naive DFS VR :: "; ndfs::Foo(tempStr, depth, 1); //vector of vectors
			std::cout << std::setw(35) << "Recursive Naive DFS P  :: "; ndfs::Foo(tempStr, depth, 2); //vector of vectors
			std::cout << std::setw(35) << "Recursive Naive DFS PR :: "; ndfs::Foo(tempStr, depth, 3); //vector of vectors

			//	First we apply some low hanging fruit - easy optimisations that can be applied to improve dft cycle enumeration
			std::cout << std::setw(35) << "Fast - Variable Nodes :: "; fast_dfs::Foo(tempStr, depth, 0);
			std::cout << std::setw(35) << "Fast - Var. Nodes Rev :: "; fast_dfs::Foo(tempStr, depth, 1);
			std::cout << std::setw(35) << "Fast - Parity Nodes   :: "; fast_dfs::Foo(tempStr, depth, 2);
			std::cout << std::setw(35) << "Fast - Par. Nodes Rev :: "; fast_dfs::Foo(tempStr, depth, 3);

			//	Largest to smallest - choose next largest root node after all its edges have been processed:
			std::cout << std::setw(35) << "Largest to Smallest   :: "; fast_l2s_vertex::Foo(tempStr, depth);

			//	Sort nodes from smallest to largest:
			std::cout << std::setw(35) << "Smallest to Largest   :: "; fast_s2l_vertex::Foo(tempStr, depth);

			//	Special version of smallest to largest:
			//	if the startnode has degree 2, then of the two root-adjacent-nodes, one paths outwards and the other can only be used to path inwards, otherwise the root-node becomes unreachable.
			//	So, instead of checking if we have reached the root-node, we can instead check if we have reached this returning root-adjacent node, thereby reducing the overall depth explored by one level.
			//	We sort the edge lists so that this root-adjacent-node is the first member, therefore only have to check a single edge at the penultimate level. We undo this sort afterwards for the non-degree 2 enumeration.
			std::cout << std::setw(35) << "S2L Deg2 Special Code :: "; fast_s2l_deg2::Foo(tempStr, depth);

			//	Largest to Smallest node updated per edge:
			//	Instead of starting searches from a chosen node, we can start from a chosen edge, trial starting from the largest node at each step
			//	this search would be costly, so we minimise it as follows; Sort nodes into buckets accordant to their degree; at each iteration, 
			//	take a node from the bucket of highest degree, check that node's degree still matches the bucket, if not; sort node into correct bucket 
			//	and take another node.
			std::cout << std::setw(35) << "L2S Updated each Edge :: "; fast_l2s_edge::Foo(tempStr, depth);

			//	Experimental method to arrange nodes to minimise condition rootAdj_1 > rootAdj_2 for duplicate detection. 
			//	if the root-node is the lowest named node x, then we arrange the graph such that the root-adjacent nodes are the next sequentially named nodes
			//	ie. if there are k root-adjacent nodes, then if node y < x+k; then y is a root-adjacent node. So when iterating through the edgelist for the root-adjacent nodes, if y > x+k can break the search for that layer.
			//	this essentially another precursor to distance collection and usage;
			std::cout << std::setw(35) << "Minimise RAdj1 > RAdj2:: "; fast_sort_adj::Foo(tempStr, depth);

			//	Choose node at random, at each edge iteration:
			//	the performance similarity of this to bisection shows that the bisection method doesn't really work. Need to rewrite this to limit it to one set of nodes.
			std::cout << std::setw(35) << "Random Node and Edge :: "; fast_randedge::Foo(tempStr, depth);

			//	Bisect the graph as quickly as possible
			//	Once the graph is split into two, you no longer do a lot of fruitless searching in the second partition.
			//	this only works for (3,6) graphs currently, as follows: we split the graph into two halves, and sort the degree 3 nodes to minimise crossings; either 0, 1, 2 or 3 edges cross the boundary between the two halves
			//	if two edges cross the boundary, then we swap the node with a node from the other side that also has 2 crossings; similarly, if 3 edges cross, we swap that with a corresponding node from across the partition
			//	in total, the number of edges that cross is reduced to 1/4, down from 1/2. Can thus bisect the graph by removing 1/4 of the nodes (actually 1/4 - 1, as last edge cannot be part of any cycles).
 			//	So, graph looks like so:
			// 
			//			 XXXX | YYYY
			//				  |
			//		AA|BBBBBB | CCCCCC|DD
			//
			//	Nodes X/Y have 6 edges, nodes A/B/C/D have 3 edges
			//	Nodes A connect to nodes X only.
			//	Nodes D connect to nodes Y only.
			//	Nodes B connect two edges to X, one edge to Y.
			//	Nodes C connect two edges to Y, one edge to X.
			//	This can be improved further, suppose that we start at in A, then from across the partition it is necessary to path Y-B-X-A or Y-C-X-B-X-A and we could apply these constraints. 
			//	This leads into the next stage of optimisations.
			std::cout << std::setw(35) << "Targeted Bisection    :: "; fast_bisect::Foo(tempStr, depth);

		}
		//_________________________________________________________________________________________________________________________

	}
}


