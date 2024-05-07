#include "filemmap.hpp"
#include <iostream>
#include <thread>
#include "process.hpp"

void process_parallel(std::vector<std::span<const char>> chunks) {
    // Process the chunks in separate thread each
    std::vector<std::jthread> runners(chunks.size());
    std::vector<DB> dbs(chunks.size());
    for (size_t i=0; i<chunks.size();++i){
        runners[i] = std::jthread(
    [&, idx=i]{dbs[idx]=process_input(chunks[idx]);});
        }
        runners.clear(); //join threads

}
