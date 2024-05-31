#include <iostream>
#include "inputparcer.hpp"
#include "func.hpp"

int main(int argc, char *argv[])
{
  InputParser input(argc, argv);
  if (input.cmdOptionExists("-h"))
  {
    help_func();
    return 0;
  }

  const std::string &mode = input.getCmdOption("-m");
  const std::string &filename = input.getCmdOption("-f");
  const std::string &pattern = input.getCmdOption("-v");

  if (!filename.empty() && !pattern.empty() && mode == "words")
  {
    std::cout << word_mode_bm(filename, pattern) << std::endl;
    return 0;
  }
  if (!filename.empty() && mode == "checksum")
  {
    std::cout << checksum_mode(filename) << std::endl;
    return 0;
  }

  help_func();
  return 0;
}