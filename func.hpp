#include <string>
#include <span>

void help_func();
size_t word_mode(const std::string filepathname, const std::string pattern);
uint32_t checksum_mode(const std::string filepathname);
uint32_t calc_checksum(std::span<const uint8_t> sp);
