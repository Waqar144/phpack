#include "../include/pack.h"

#include "gtest/gtest.h"
#include <iostream>
#include <limits>

TEST(PhPacker, Arg_v)
{
    uint16_t val = 123;
    auto s = PhPacker::pack<uint16_t>('v', val);
    uint16_t res = std::any_cast<uint16_t>(PhPacker::unpack('v', s));
    EXPECT_EQ(res, 123);

    std::string str = PhPacker::pack<uint16_t>('v', std::numeric_limits<uint16_t>::max());
    uint16_t result = std::any_cast<uint16_t>(PhPacker::unpack('v', str));
    uint16_t expected = std::numeric_limits<uint16_t>::max();
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack<uint16_t>('v', std::numeric_limits<uint16_t>::min());
    result = std::any_cast<uint16_t>(PhPacker::unpack('v', str));
    expected = std::numeric_limits<uint16_t>::min();
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_s)
{
    int16_t val = 123;
    auto s = PhPacker::pack<short>('s', val);
    int16_t res = std::any_cast<short>(PhPacker::unpack('s', s));
    EXPECT_EQ(res, 123);

    std::string str = PhPacker::pack<short>('s', int16_t{655});
    int16_t result = std::any_cast<short>(PhPacker::unpack('s', str));
    int16_t expected = 655;
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack<int16_t>('s', std::numeric_limits<int16_t>::max());
    result = std::any_cast<int16_t>(PhPacker::unpack('s', str));
    expected = std::numeric_limits<int16_t>::max();
    GTEST_ASSERT_EQ(result, expected);

    str = PhPacker::pack<int16_t>('s', std::numeric_limits<int16_t>::min());
    result = std::any_cast<int16_t>(PhPacker::unpack('s', str));
    expected = std::numeric_limits<int16_t>::min();
    GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_S)
{
   auto s = PhPacker::pack('S', uint16_t{123});
   uint16_t res = std::any_cast<uint16_t>(PhPacker::unpack('S', s));
   EXPECT_EQ(res, 123);

   std::string str = PhPacker::pack('S', uint16_t{65535});
   uint16_t result = std::any_cast<uint16_t>(PhPacker::unpack('S', str));
   uint16_t expected = 65535;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint16_t>('S', std::numeric_limits<uint16_t>::max());
   result = std::any_cast<uint16_t>(PhPacker::unpack('S', str));
   expected = std::numeric_limits<uint16_t>::max();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint16_t>('S', std::numeric_limits<uint16_t>::min());
   result = std::any_cast<uint16_t>(PhPacker::unpack('S', str));
   expected = std::numeric_limits<uint16_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_n)
{
   auto s = PhPacker::pack('n', uint16_t{123});
   uint16_t res = std::any_cast<uint16_t>(PhPacker::unpack('n', s));
   EXPECT_EQ(res, 123);

   std::string str = PhPacker::pack('n', uint16_t{65535});
   uint16_t result = std::any_cast<uint16_t>(PhPacker::unpack('n', str));
   uint16_t expected = 65535;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint16_t>('n', std::numeric_limits<uint16_t>::max());
   result = std::any_cast<uint16_t>(PhPacker::unpack('n', str));
   expected = std::numeric_limits<uint16_t>::max();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint16_t>('n', std::numeric_limits<uint16_t>::min());
   result = std::any_cast<uint16_t>(PhPacker::unpack('n', str));
   expected = std::numeric_limits<uint16_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_i)
{
   auto s = PhPacker::pack('i', 123);
   int res = std::any_cast<int>(PhPacker::unpack('i', s));
   EXPECT_EQ(res, 123);

   std::string str = PhPacker::pack('i', 65535123);
   int result = std::any_cast<int>(PhPacker::unpack('i', str));
   int expected = 65535123;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<int32_t>('i', std::numeric_limits<int32_t>::max());
   result = std::any_cast<int32_t>(PhPacker::unpack('i', str));
   expected = std::numeric_limits<int32_t>::max();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<int32_t>('i', std::numeric_limits<int32_t>::min());
   result = std::any_cast<int32_t>(PhPacker::unpack('i', str));
   expected = std::numeric_limits<int32_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_I)
{
   auto s = PhPacker::pack('I', 123u);
   uint32_t res = std::any_cast<uint32_t>(PhPacker::unpack('I', s));
   EXPECT_EQ(res, 123);

   std::string str = PhPacker::pack('I', 655351234u);
   uint32_t result = std::any_cast<uint32_t>(PhPacker::unpack('I', str));
   uint32_t expected = 655351234u;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('I', std::numeric_limits<uint32_t>::max());
   result = std::any_cast<uint32_t>(PhPacker::unpack('I', str));
   expected = std::numeric_limits<uint32_t>::max();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('I', std::numeric_limits<uint32_t>::min());
   result = std::any_cast<uint32_t>(PhPacker::unpack('I', str));
   expected = std::numeric_limits<uint32_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

//// always 32 - bit - machine byte order
TEST(PhPacker, Arg_l_first)
{
   auto s = PhPacker::pack<int32_t>('l', 123);
   int32_t res = std::any_cast<int32_t>(PhPacker::unpack('l', s));
   EXPECT_EQ(res, 123);

   std::string str = PhPacker::pack<int32_t>('l', 655351234l);
   int32_t result = std::any_cast<int32_t>(PhPacker::unpack('l', str));
   int32_t expected = 655351234l;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<int32_t>('l', std::numeric_limits<int32_t>::max());
   result = std::any_cast<int32_t>(PhPacker::unpack('l', str));
   expected = std::numeric_limits<int32_t>::max();
   GTEST_ASSERT_EQ(result, expected);


   str = PhPacker::pack<int32_t>('l', std::numeric_limits<int32_t>::min());
   result = std::any_cast<int32_t>(PhPacker::unpack('l', str));
   expected = std::numeric_limits<int32_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_L)
{
   std::string str = PhPacker::pack<uint32_t>('L', 655351234);
   size_t result = std::any_cast<uint32_t>(PhPacker::unpack('L', str));
   size_t expected = 655351234;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('L', 65);
   result = std::any_cast<uint32_t>(PhPacker::unpack('L', str));
   expected = 65;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('L', std::numeric_limits<uint32_t>::max());
   result = std::any_cast<uint32_t>(PhPacker::unpack('L', str));
   expected = std::numeric_limits<uint32_t>::max();
   GTEST_ASSERT_EQ(result, expected);


   str = PhPacker::pack<uint32_t>('L', std::numeric_limits<uint32_t>::min());
   result = std::any_cast<uint32_t>(PhPacker::unpack('L', str));
   expected = std::numeric_limits<uint32_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_N)
{
   std::string str = PhPacker::pack<uint32_t>('N', 655351234);
   unsigned long result = std::any_cast<uint32_t>(PhPacker::unpack('N', str));
   unsigned long expected = 655351234;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('N', 65);
   result = std::any_cast<uint32_t>(PhPacker::unpack('N', str));
   expected = 65;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('N', std::numeric_limits<uint32_t>::max());
   result = std::any_cast<uint32_t>(PhPacker::unpack('N', str));
   expected = std::numeric_limits<uint32_t>::max();
   GTEST_ASSERT_EQ(result, expected);


   str = PhPacker::pack<uint32_t>('N', std::numeric_limits<uint32_t>::min());
   result = std::any_cast<uint32_t>(PhPacker::unpack('N', str));
   expected = std::numeric_limits<uint32_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_V)
{
   std::string str = PhPacker::pack<uint32_t>('V', 655351234);
   unsigned long result = std::any_cast<uint32_t>(PhPacker::unpack('V', str));
   unsigned long expected = 655351234;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('V', 65);
   result = std::any_cast<uint32_t>(PhPacker::unpack('V', str));
   expected = 65;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint32_t>('V', std::numeric_limits<uint32_t>::max());
   result = std::any_cast<uint32_t>(PhPacker::unpack('V', str));
   expected = std::numeric_limits<uint32_t>::max();
   GTEST_ASSERT_EQ(result, expected);


   str = PhPacker::pack<uint32_t>('V', std::numeric_limits<uint32_t>::min());
   result = std::any_cast<uint32_t>(PhPacker::unpack('V', str));
   expected = std::numeric_limits<uint32_t>::min();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_q)
{
   std::string str = PhPacker::pack<int64_t>('q', int64_t{65535123424ll});
   int64_t result = std::any_cast<int64_t>(PhPacker::unpack('q', str));
   int64_t expected = 65535123424ll;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<int64_t>('q', 65l);
   result = std::any_cast<int64_t>(PhPacker::unpack('q', str));
   expected = 65;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<int64_t>('q', std::numeric_limits<int64_t>::min());
   result = std::any_cast<int64_t>(PhPacker::unpack('q', str));
   expected = std::numeric_limits<int64_t>::min();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<int64_t>('q', std::numeric_limits<int64_t>::max());
   result = std::any_cast<int64_t>(PhPacker::unpack('q', str));
   expected = std::numeric_limits<int64_t>::max();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_Q)
{
   std::string str = PhPacker::pack<uint64_t>('Q', 65535123424ull);
   uint64_t result = std::any_cast<uint64_t>(PhPacker::unpack('Q', str));
   uint64_t expected = 65535123424ull;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('Q', 65ull);
   result = std::any_cast<uint64_t>(PhPacker::unpack('Q', str));
   expected = 65;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('Q', std::numeric_limits<uint64_t>::min());
   result = std::any_cast<uint64_t>(PhPacker::unpack('Q', str));
   expected = std::numeric_limits<uint64_t>::min();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('Q', std::numeric_limits<uint64_t>::max());
   result = std::any_cast<uint64_t>(PhPacker::unpack('Q', str));
   expected = std::numeric_limits<uint64_t>::max();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_J)
{
   std::string str = PhPacker::pack<uint64_t>('J', 65535123424ull);
   uint64_t result = std::any_cast<uint64_t>(PhPacker::unpack('J', str));
   uint64_t expected = 65535123424ull;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('J', 65ull);
   result = std::any_cast<uint64_t>(PhPacker::unpack('J', str));
   expected = 65;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('J', std::numeric_limits<uint64_t>::min());
   result = std::any_cast<uint64_t>(PhPacker::unpack('J', str));
   expected = std::numeric_limits<uint64_t>::min();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('J', std::numeric_limits<uint64_t>::max());
   result = std::any_cast<uint64_t>(PhPacker::unpack('J', str));
   expected = std::numeric_limits<uint64_t>::max();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_P)
{
   std::string str = PhPacker::pack<uint64_t>('P', 65535123424ull);
   uint64_t result = std::any_cast<uint64_t>(PhPacker::unpack('P', str));
   uint64_t expected = 65535123424ull;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('P', 65ull);
   result = std::any_cast<uint64_t>(PhPacker::unpack('P', str));
   expected = 65ull;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('P', std::numeric_limits<uint64_t>::min());
   result = std::any_cast<uint64_t>(PhPacker::unpack('P', str));
   expected = std::numeric_limits<uint64_t>::min();
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<uint64_t>('P', std::numeric_limits<uint64_t>::max());
   result = std::any_cast<uint64_t>(PhPacker::unpack('P', str));
   expected = std::numeric_limits<uint64_t>::max();
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_f)
{
   std::string str = PhPacker::pack<float>('f', 1.234f);
   float result = std::any_cast<float>(PhPacker::unpack('f', str));
   float expected = 1.234f;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<float>('f', 65232.123f);
   result = std::any_cast<float>(PhPacker::unpack('f', str));
   expected = 65232.123f;
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_G)
{
   std::string str = PhPacker::pack<float>('G', 1.234f);
   float result = std::any_cast<float>(PhPacker::unpack('G', str));
   float expected = 1.234f;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<float>('G', 65232.123f);
   result = std::any_cast<float>(PhPacker::unpack('G', str));
   expected = 65232.123f;
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_g)
{
   std::string str = PhPacker::pack<float>('g', 1.234f);
   float result = std::any_cast<float>(PhPacker::unpack('g', str));
   float expected = 1.234f;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<float>('g', 65232.123f);
   result = std::any_cast<float>(PhPacker::unpack('g', str));
   expected = 65232.123f;
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_d)
{
   std::string str = PhPacker::pack<double>('d', 1.234);
   double result = std::any_cast<double>(PhPacker::unpack('d', str));
   double expected = 1.234;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<double>('d', 65232.123);
   result = std::any_cast<double>(PhPacker::unpack('d', str));
   expected = 65232.123;
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_e)
{
   std::string str = PhPacker::pack<double>('e', 1.234);
   double result = std::any_cast<double>(PhPacker::unpack('e', str));
   double expected = 1.234;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<double>('e', 652322.123);
   result = std::any_cast<double>(PhPacker::unpack('e', str));
   expected = 652322.123;
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_E)
{
   std::string str = PhPacker::pack<double>('E', 123.234);
   double result = std::any_cast<double>(PhPacker::unpack('E', str));
   double expected = 123.234;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<double>('E', 65232213123.123);
   result = std::any_cast<double>(PhPacker::unpack('E', str));
   expected = 65232213123.123;
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_c)
{
   std::string str = PhPacker::pack<signed char>('c', 12);
   signed char result = std::any_cast<signed char>(PhPacker::unpack('c', str));
   signed char expected = 12;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<signed char>('c', -77);
   result = std::any_cast<signed char>(PhPacker::unpack('c', str));
   expected = -77;
   GTEST_ASSERT_EQ(result, expected);
}

TEST(PhPacker, Arg_C)
{
   std::string str = PhPacker::pack<unsigned char>('C', 123);
   unsigned char result = std::any_cast<unsigned char>(PhPacker::unpack('C', str));
   unsigned char expected = 123;
   GTEST_ASSERT_EQ(result, expected);

   str = PhPacker::pack<unsigned char>('C', 200);
   result = std::any_cast<unsigned char>(PhPacker::unpack('C', str));
   expected = 200;
   GTEST_ASSERT_EQ(result, expected);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
