#include <stdint.h>
#include <unordered_map>
#include <string>

struct Record
{
    uint64_t cnt;
    double sum;

    float min;
    float max;
};

using DB = std::unordered_map<std::string, Record>;

DB process_input(std::span<const char> data);