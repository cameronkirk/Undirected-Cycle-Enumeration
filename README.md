# Undirected-Cycle-Enumeration
Incremental development of an algorithm for faster short-cycle enumeration on undirected graphs (LDPC Codes).

Adding files as I clean them up, will comment/expedite upon request - afaik, end result is substantially faster than anything published. 

|                              | Halford   |            |               |                 | Karimi           |                |                 |                   |
| ---------------------------- | --------- | ---------- | ------------- | --------------- | ---------------- | -------------- | --------------- | ----------------- |
|&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s&nbsp-s| Code A    | Code B     | Code C        | Code D          | Code A           | Code B         | Code C          | Code D            |
|                              | 816.3.174 | 816.55.178 | PEGReg252x504 | PEGirReg252x504 | PEGirReg504x1008 | PEGReg504x1008 | 8000.4000.3.483 | 10000.10000.3.631 |
|                              | 10        | 10         | 12            | 10              | 10               | 14             | 10              | 10                |
| <sub><sup>Recursive Naive DFS</sub></sup>          | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Contig. Array NDFS           | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Iterative Naive DFS          | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Recursive Naive MULTI        | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Recursive Naive DFS (no rem) | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Recursive Naive DFS (unroll) | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Recursive NDFS (fixed size)  | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Contig. Naive DFS (no rem)   | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |
| Iterative Naive DFS (no rem) | 0         | 0          | 0             | 0               | 0                | 0              | 0               | 0                 |

| |Halford| | | |Karimi| | | |
|----:|----:|----:|----:|----:|----:|----:|----:|----:|
|&nbsp-s &nbsp-s &nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s &nbsp-s &nbsp-s &nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s&nbsp-s &nbsp-s|Code A|Code B|Code C|Code D|Code A|Code B|Code C|Code D|
| |816.3.174|816.55.178|PEGReg252x504|PEGirReg252x504|PEGirReg504x1008|PEGReg504x1008|8000.4000.3.483|10000.10000.3.631|
| |10|10|12|10|10|14|10|10|
|Recursive Naive DFS|0|0|0|0|0|0|0|0|
|Contig. Array NDFS |0|0|0|0|0|0|0|0|
|Iterative Naive DFS|0|0|0|0|0|0|0|0|
|Recursive Naive MULTI|0|0|0|0|0|0|0|0|
|Recursive Naive DFS (no rem)|0|0|0|0|0|0|0|0|
|Recursive Naive DFS (unroll)|0|0|0|0|0|0|0|0|
|Recursive NDFS (fixed size)|0|0|0|0|0|0|0|0|
|Contig. Naive DFS (no rem)|0|0|0|0|0|0|0|0|
|Iterative Naive DFS (no rem)|0|0|0|0|0|0|0|0|
