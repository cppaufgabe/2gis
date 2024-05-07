// tests.cpp
#include "../func.cpp"
#include <gtest/gtest.h>

TEST(ChecksumTest, rem1)
{
    std::vector<uint8_t> v = {255};
    std::span<uint8_t> sp(v.data(), v.size());
    std::cout << "its tests" << std::endl;
    ASSERT_EQ(255, calc_checksum(sp));
}

TEST(ChecksumTest, rem2)
{
    std::vector<uint8_t> v = {255, 255};
    std::span<uint8_t> sp(v.data(), v.size());
    std::cout << "its tests" << std::endl;
    ASSERT_EQ(65535, calc_checksum(sp));
}
TEST(ChecksumTest, rem3)
{
    std::vector<uint8_t> v = {255, 255,255};
    std::span<uint8_t> sp(v.data(), v.size());
    std::cout << "its tests" << std::endl;
    ASSERT_EQ(16777215, calc_checksum(sp));
}

TEST(ChecksumTest, 4byte)
{
    std::vector<uint8_t> v = {255, 255,255,255};
    std::span<uint8_t> sp(v.data(), v.size());
    std::cout << "its tests" << std::endl;
    ASSERT_EQ(4294967295, calc_checksum(sp));
}

TEST(ChecksumTest, 8byte)
{
    std::vector<uint8_t> v = {255,255,255,255,255,255,255,255};
    std::span<uint8_t> sp(v.data(), v.size());
    std::cout << "its tests" << std::endl;
    ASSERT_EQ(4294967294, calc_checksum(sp));
}