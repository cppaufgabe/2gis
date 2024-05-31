#include <iostream>
#include <iostream>
#include <thread>
#include <bit>
#include "filemmap.hpp"
#include "func.hpp"
#include "boyermoore.hpp"
#include <span>
#include "string.h"

void help_func()
{
  std::cout << "Usage:" << std::endl;
  std::cout << "1) test -f filename -m words -v pattern" << std::endl;
  std::cout << "prints the number of words \"pattern\" in the file \"filename\" " << std::endl;
  std::cout << std::endl;
  std::cout << "2) test -f filename -m checksum" << std::endl;
  std::cout << "prints a 32-bit checksum calculated by the algorithm checksum = word1 + word2 + ... + wordN," << std::endl;
  std::cout << "where word1..wordN – 32-bit words representing the contents of the \"filename\" " << std::endl;
  std::cout << std::endl;
  std::cout << "-h display this help and exit";
  std::cout << std::endl;
}

size_t word_mode(const std::string filepathname, const std::string pattern)
{

  MappedFile mfile(filepathname);
  std::string_view sv(mfile.data().data(), mfile.data().size());
  size_t pos = 0;
  size_t count = 0;
  while (true)
  {
    pos = sv.find(pattern, pos);
    if (pos != std::string_view::npos)
    {
      pos++;
      count++;
    }
    else
      break;
  }

  return count;
}

size_t word_mode_bm(const std::string filepathname, const std::string pattern)
{

  MappedFile mfile(filepathname);
  //std::string_view sv(mfile.data().data(), mfile.data().size());
  size_t pos = 0;
  size_t count = 0;
    
  BoyerMoore bm(pattern);

  while (true)
  {
    pos = bm.search(mfile.data(), pos);
    if (pos < mfile.data().size())
    {
      pos++;
      count++;
    }
    else
      break;
  }

  return count;
}


uint32_t checksum_mode(const std::string filepathname)
{
  MappedFile mfile(filepathname);
  std::span<const uint8_t> sp = std::bit_cast<std::span<const uint8_t>>(mfile.data());
  return calc_checksum(sp);
}

uint32_t calc_checksum(std::span<const uint8_t> sp) {
  size_t quot = (size_t)sp.size() / 4;
  size_t rem = sp.size() % 4;
  auto it = sp.begin();

  size_t bytelength = 4;
  uint32_t quot_sum = 0;
  for (size_t i = 0; i < quot * 4; i += 4)
  {
    uint32_t word = (static_cast<uint32_t>(sp[i])) |
                    (static_cast<uint32_t>(sp[i + 1]) << 8) |
                    (static_cast<uint32_t>(sp[i + 2]) << 16) |
                    (static_cast<uint32_t>(sp[i + 3]) << 24);
    quot_sum += word;
  }
  size_t index = quot * 4;
  uint32_t rem_sum = 0;

  if (rem == 1)
  {
    rem_sum = (static_cast<uint32_t>(sp[index]));
  }
  if (rem == 2)
  {
    rem_sum = (static_cast<uint32_t>(sp[index])) |
              (static_cast<uint32_t>(sp[index + 1]) << 8);
  }
  if (rem == 3)
  {
    rem_sum = (static_cast<uint32_t>(sp[index])) |
              (static_cast<uint32_t>(sp[index + 1]) << 8) |
              (static_cast<uint32_t>(sp[index + 2]) << 16);
  }

  return quot_sum + rem_sum;
}