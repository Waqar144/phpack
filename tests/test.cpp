#include "../src/pack.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(Arg_v, Arg_v_singleByte)
{
    auto s = PhPacker::pack('v', 123);
    auto res = PhPacker::unpack('v', s);
    EXPECT_EQ(res, 123);
}

TEST(Arg_v_two, Arg_v_TwoBytes)
{
    std::string str = PhPacker::pack('v', 65535);
    int result = PhPacker::unpack('v', str);
    int expected = 65535;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(Arg_s, Arg_s_singleByte)
{
    auto s = PhPacker::pack('s', 123);
    auto res = PhPacker::unpack('s', s);
    EXPECT_EQ(res, 123);
}

TEST(Arg_s_two, Arg_s_TwoBytes)
{
    std::string str = PhPacker::pack('s', 655);
    int result = PhPacker::unpack('s', str);
    int expected = 655;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(Arg_S, Arg_S_singleByte)
{
    auto s = PhPacker::pack('S', 123);
    auto res = PhPacker::unpack('S', s);
    EXPECT_EQ(res, 123);
}

TEST(Arg_S_two, Arg_S_TwoBytes)
{
    std::string str = PhPacker::pack('S', 65535);
    int result = PhPacker::unpack('S', str);
    int expected = 65535;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(Arg_n, Arg_n_singleByte)
{
    auto s = PhPacker::pack('n', 123);
    auto res = PhPacker::unpack('n', s);
    EXPECT_EQ(res, 123);
}

TEST(Arg_n_two, Arg_n_TwoBytes)
{
    std::string str = PhPacker::pack('n', 65535);
    int result = PhPacker::unpack('n', str);
    int expected = 65535;
    GTEST_ASSERT_EQ(result, expected);
}

int main(int argc, char *argv[])
{
    PhPacker::init();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
