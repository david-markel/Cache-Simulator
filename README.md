# Cache-Simulator

Simulates how cache works in computer architecture by reading in hypothetical memory accesses and configurations and generating a cache table and statistics. 

First and foremost, this was an assignemnt for a computer architecture course, and so I do not claim this to be an original idea. The program takes in configurations and hypothetical memory addresses and generates a cache table with statistics on hits and misses. This is meant so simulate how cache works in computer architecture.

To run, download the memory.txt and.config files, then compile and run cache-simulator.cpp.

When ran, the terminal will prompt you to enter the name of a .txt file that holds hexadecimal memory addresses. Each one essentially is a request to cache for this address. The cache table initially starts as empty, so to recieve any hits, the .txt file should have multiple accesses to the same hexadecimal value. The program uses a file called trace.config that has number of sets, set size, and line size. These configurations are different ways that cache can be organized, and I would not reccomend changing them unless one understands exactly how it will effect cache accesses. In fact, it is probably best that anyone who uses this just use the default trace.txt and trace.config files I have provided to understand how it works, though it will work with any acceptably formatted configurations and addresses.

<img width="591" alt="cache demonstration" src="https://user-images.githubusercontent.com/60165504/159138806-19f6d7a4-df6a-40ed-8fc4-bb104b459db7.PNG">
