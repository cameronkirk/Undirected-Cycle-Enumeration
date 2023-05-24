# Undirected-Cycle-Enumeration
Incremental development of an algorithm for faster short-cycle enumeration on undirected graphs (LDPC Codes).

Adding files as I clean them up, will comment/expedite upon request - afaik, end result is substantially faster than anything published. 

|                              | Halford   |            |               |                 | Karimi           |                |                 |                   |
| ---------------------------- | --------- | ---------- | ------------- | --------------- | ---------------- | -------------- | --------------- | ----------------- |
|| Code A    | Code B     | Code C        | Code D          | Code A           | Code B         | Code C          | Code D            |
|                              | 816.3.174 | 816.55.178 | PEGReg252x504 | PEGirReg252x504 | PEGirReg504x1008 | PEGReg504x1008 | 8000.4000.3.483 | 10000.10000.3.631 |
|                              | 10        | 10         | 12            | 10              | 10               | 14             | 10              | 10                |
| Johnson (1975)               | 0.08 | 32.65 | 0.44 | 49.15 | 91.29 | 8.74 | 1.95  | 5.43   |
| Halford & Chugg (2005)       | 0.13 | 0.12  | 0.07 | 0.04  | 0.21  | 0.29 | 51.69 | 870.82 |
| Karimi (2013)                | 0.14 | 0.35  | 0.04 | 0.07  | 0.35  | 0.20 | 23.14 | 331.80 |
| Recursive Naive DFS          | 0.06 | 16.91 | 0.34 | 34.41 | 65.01 | 6.19 | 1.09  | 3.72   |
| Fast Largest to Smallest     | 0.01 | 2.12  | 0.05 | 1.06  | 2.16  | 0.98 | 0.20  | 0.84   |
| ---------------------------- | ---- | ----- | ---- | ----- | ----- | ---- | ----- | ------ |
| Recursive Naive DFS V        | 0.06 | 17.04 | 0.34 | 34.33 | 64.67 | 6.38 | 1.09  | 3.67   |
| Contig. Array NDFS           | 0.05 | 17.96 | 0.30 | 31.44 | 60.29 | 5.50 | 0.79  | 2.09   |
| Iterative Naive DFS          | 0.06 | 21.98 | 0.36 | 39.97 | 75.10 | 6.58 | 0.88  | 2.14   |
| Recursive Naive MULTI        | 0.03 | 3.05  | 0.07 | 5.72  | 10.32 | 1.01 | 0.40  | 1.17   |
| Recursive Naive DFS (no rem) | 0.06 | 16.94 | 0.32 | 34.80 | 65.78 | 6.20 | 1.12  | 3.68   |
| Recursive Naive DFS (unroll) | 0.06 | 19.29 | 0.31 | 43.37 | 87.31 | 6.06 | 1.04  | 3.46   |
| Recursive NDFS (fixed size)  | 0.00 | 0.00  | 0.00 | 0.00  | 0.00  | 0.00 | 0.00  | 3.43   |
| Contig. Naive DFS (no rem)   | 0.05 | 16.62 | 0.31 | 34.68 | 65.78 | 5.61 | 0.71  | 1.90   |
| Iterative Naive DFS (no rem) | 0.06 | 21.76 | 0.35 | 41.78 | 78.16 | 6.38 | 0.78  | 1.91   |
| Recursive Naive DFS V        | 0.06 | 16.83 | 0.35 | 35.94 | 65.49 | 6.38 | 1.10  | 3.71   |
| Recursive Naive DFS VR       | 0.06 | 17.67 | 0.34 | 6.31  | 12.11 | 6.34 | 1.17  | 3.75   |
| Recursive Naive DFS P        | 0.03 | 10.80 | 0.18 | 13.74 | 28.59 | 3.38 | 0.75  | 2.37   |
| Recursive Naive DFS PR       | 0.03 | 10.79 | 0.18 | 14.19 | 26.08 | 3.52 | 0.72  | 2.40   |
| Fast - Variable Nodes        | 0.01 | 2.35  | 0.06 | 2.99  | 6.25  | 1.11 | 0.23  | 0.92   |
| Fast - Var. Nodes Rev        | 0.01 | 2.35  | 0.06 | 1.04  | 2.16  | 1.08 | 0.24  | 0.92   |
| Fast - Parity Nodes          | 0.01 | 2.14  | 0.05 | 1.78  | 3.78  | 1.07 | 0.21  | 0.83   |
| Fast - Par. Nodes Rev        | 0.01 | 2.15  | 0.05 | 1.81  | 3.69  | 1.09 | 0.22  | 0.85   |
| Largest to Smallest          | 0.01 | 2.12  | 0.05 | 1.06  | 2.16  | 0.98 | 0.20  | 0.84   |
| Smallest to Largest          | 0.01 | 2.36  | 0.06 | 3.21  | 6.68  | 1.11 | 0.22  | 0.90   |
| S2L Deg2 Special Code        | 0.01 | 1.88  | 0.04 | 1.36  | 2.88  | 0.78 | 0.14  | 0.62   |
| L2S Updated each Edge        | 0.01 | 1.29  | 0.04 | 0.69  | 1.41  | 0.65 | 0.14  | 0.57   |
| Minimise RAdj1 > RAdj2       | 0.01 | 0.68  | 0.03 | 0.58  | 1.20  | 0.58 | 0.10  | 0.36   |
| Random Node and Edge         | 0.01 | 1.91  | 0.05 | 2.13  | 4.42  | 1.08 | 0.21  | 0.81   |
| Targeted Bisection           | 0.01 | 2.36  | 0.05 | 3.00  | 6.13  | 0.98 | 0.21  | 0.79   |
