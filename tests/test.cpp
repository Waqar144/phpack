#include "../src/pack.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(PhPacker, Arg_v_singleByte)
{
    auto s = PhPacker::pack('v', 123);
    auto res = PhPacker::unpack<unsigned short>('v', s);
    EXPECT_EQ(res, 123);
}

TEST(PhPacker, Arg_v_TwoBytes)
{
    std::string str = PhPacker::pack('v', 65535);
    int result = PhPacker::unpack<unsigned short>('v', str);
    int expected = 65535;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_s_singleByte)
{
    auto s = PhPacker::pack('s', 123);
    auto res = PhPacker::unpack<short>('s', s);
    EXPECT_EQ(res, 123);
}

TEST(PhPacker, Arg_s_TwoBytes)
{
    std::string str = PhPacker::pack('s', 655);
    auto result = PhPacker::unpack<short>('s', str);
    int expected = 655;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_S_singleByte)
{
    auto s = PhPacker::pack('S', 123);
    auto res = PhPacker::unpack<unsigned short>('S', s);
    EXPECT_EQ(res, 123);
}

TEST(PhPacker, Arg_S_TwoBytes)
{
    std::string str = PhPacker::pack('S', 65535);
    int result = PhPacker::unpack<unsigned short>('S', str);
    int expected = 65535;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_n_singleByte)
{
    auto s = PhPacker::pack('n', 123);
    auto res = PhPacker::unpack<unsigned short>('n', s);
    EXPECT_EQ(res, 123);
}

TEST(PhPacker, Arg_n_TwoBytes)
{
    std::string str = PhPacker::pack('n', 65535);
    int result = PhPacker::unpack<unsigned short>('n', str);
    int expected = 65535;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_i_first)
{
    auto s = PhPacker::pack('i', 123);
    auto res = PhPacker::unpack<int>('i', s);
    EXPECT_EQ(res, 123);
}

TEST(PhPacker, Arg_i_Two)
{
    std::string str = PhPacker::pack('i', 65535123);
    int result = PhPacker::unpack<int>('i', str);
    int expected = 65535123;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_I_first)
{
    auto s = PhPacker::pack('I', 123);
    auto res = PhPacker::unpack<unsigned int>('I', s);
    EXPECT_EQ(res, 123);
}

TEST(PhPacker, Arg_I_Two)
{
    std::string str = PhPacker::pack('I', 655351234);
    int result = PhPacker::unpack<unsigned int>('I', str);
    int expected = 655351234;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_l_first)
{
    auto s = PhPacker::pack('l', 123);
    auto res = PhPacker::unpack<long>('l', s);
    EXPECT_EQ(res, 123);
}

TEST(PhPacker, Arg_l_Two)
{
    std::string str = PhPacker::pack('l', 655351234);
    int result = PhPacker::unpack<long>('l', str);
    int expected = 655351234;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_L)
{
    std::string str = PhPacker::pack('L', 655351234);
    int result = PhPacker::unpack<unsigned long>('L', str);
    int expected = 655351234;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack('L', 65);
    result = PhPacker::unpack<unsigned long>('L', str);
    expected = 65;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_N)
{
    std::string str = PhPacker::pack('N', 655351234);
    int result = PhPacker::unpack<unsigned long>('N', str);
    int expected = 655351234;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack('N', 65);
    result = PhPacker::unpack<unsigned long>('N', str);
    expected = 65;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_V)
{
    std::string str = PhPacker::pack('V', 655351234);
    int result = PhPacker::unpack<unsigned long>('V', str);
    int expected = 655351234;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack('V', 65);
    result = PhPacker::unpack<unsigned long>('V', str);
    expected = 65;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_q)
{
    std::string str = PhPacker::pack('q', 65535123424);
    long long result = PhPacker::unpack<long long>('q', str);
    long long expected = 65535123424;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack('q', 65);
    result = PhPacker::unpack<long long>('q', str);
    expected = 65;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_Q)
{
    std::string str = PhPacker::pack('Q', 65535123424);
    unsigned long result = PhPacker::unpack<unsigned long long>('Q', str);
    unsigned long expected = 65535123424;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack('Q', 65);
    result = PhPacker::unpack<unsigned long long>('Q', str);
    expected = 65;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_J)
{
    std::string str = PhPacker::pack('J', 65535123424);
    unsigned long result = PhPacker::unpack<unsigned long long>('J', str);
    unsigned long expected = 65535123424;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack('J', 65);
    result = PhPacker::unpack<unsigned long long>('J', str);
    expected = 65;
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_P)
{
    std::string str = PhPacker::pack('P', 65535123424);
    unsigned long result = PhPacker::unpack<unsigned long long>('P', str);
    unsigned long expected = 65535123424;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack('P', 65);
    result = PhPacker::unpack<unsigned long long>('P', str);
    expected = 65;
    GTEST_ASSERT_EQ(result, expected);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
