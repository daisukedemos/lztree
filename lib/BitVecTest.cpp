#include <gtest/gtest.h>
#include <sstream>
#include "BitVec.hpp"

using namespace lztree;
using namespace std;

TEST(BitVec, trivial){
  BitVec bv;
  ASSERT_EQ(0, bv.num());
  ASSERT_EQ(0, bv.one_num());
  ASSERT_EQ(0, bv.Rank(0));
  ostringstream os;
  bv.Write(os);
  
  BitVec bv2;
  istringstream is(os.str());
  bv2.Read(is);
  ASSERT_EQ(0, bv2.num());
  ASSERT_EQ(0, bv2.one_num());
  ASSERT_EQ(0, bv2.Rank(0));
}

TEST(BitVec, random){
  BitVec bv;
  const int N = 10000;
  vector<uint32_t> bits;
  vector<uint32_t> ranks;
  uint32_t one_num = 0;
  for (int i = 0; i < N; ++i){
    uint32_t b = rand() % 2;
    bv.PushBack(b);
    ranks.push_back(one_num);
    bits.push_back(b);
    if (b == 1){
      ++one_num;
    }
  }
  ASSERT_EQ(N, bv.num());
  ASSERT_EQ(one_num, bv.one_num());
  ASSERT_EQ(one_num, bv.Rank(N));

  for (int i = 0; i < N; ++i){
    ASSERT_EQ(bits[i], bv.GetBit(i));
    ASSERT_EQ(ranks[i], bv.Rank(i));
  }

  ostringstream os;
  bv.Write(os);
  bv.Clear();
  ASSERT_EQ(0, bv.num());
  ASSERT_EQ(0, bv.one_num());
  
  BitVec bv2;
  istringstream is(os.str());
  bv2.Read(is);
  ASSERT_EQ(N, bv2.num());
  ASSERT_EQ(one_num, bv2.one_num());
  ASSERT_EQ(one_num, bv2.Rank(N));

  for (int i = 0; i < N; ++i){
    ASSERT_EQ(bits[i], bv2.GetBit(i));
    ASSERT_EQ(ranks[i], bv2.Rank(i));
  }

}


