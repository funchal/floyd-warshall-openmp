Naive parallel Floyd-Warshall algorithm (all-pairs shortest path on dense weighted graphs) using OpenMP.

https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm

![Performance](https://raw.githubusercontent.com/funchal/floyd-warshall-openmp/master/performance.png)

Plot above is time (us) vs number of threads.
The multi-threaded version is up to 3.6x faster than single-threaded on 4 cores,
which is pretty good for a memory-bound algorithm.
However, it's still a `O(n^3)` algorithm.
