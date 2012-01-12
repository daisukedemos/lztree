#include <gtest/gtest.h>
#include "FixedArray.hpp"

using namespace lztree;
using namespace std;

TEST(FixedArray, trivial){
  FixedArray array;
  EXPECT_EQ(0, array.num());
  EXPECT_EQ(0, array.len());

  for (uint64_t i = 0; i < 10; ++i){
    array.PushBack(i);
  }
  for (uint64_t i = 10; ; --i){
    array.PushBack(i);
    if (i == 0) break;
  }

  for (uint64_t i = 0; i < 10; ++i){
    EXPECT_EQ(i, array.Get(i));
  }

  for (uint64_t i = 0; i < 10; ++i){
    EXPECT_EQ(10 - i, array.Get(i+10));
  }
}

TEST(FixedArray, random){
  for (int iter = 0; iter < 10; ++iter){
    const uint64_t N = rand() % 10000;
    const uint64_t shift = rand() % 32;
    const uint64_t val_max = 1LLU << shift;
    FixedArray array;
    vector<uint64_t> vals;
    uint64_t max_val = 0;
    for (uint64_t i = 0; i < N; ++i){
      uint64_t v = rand() % val_max;
      if (max_val < v){
        max_val = v;
      }
      array.PushBack(v);
      vals.push_back(v);
    }

    uint64_t len = 0;
    for (; len < 64 && max_val >> len; ++len){
    }

    ASSERT_EQ(N, array.num());
    ASSERT_EQ(len, array.len());
    
    for (uint64_t i = 0; i < N; ++i){
      ASSERT_EQ(vals[i], array.Get(i));
    }

    ostringstream os;
    array.Write(os);
    
    FixedArray array2;
    istringstream is(os.str());
    array2.Read(is);

    ASSERT_EQ(N, array2.num());
    ASSERT_EQ(len, array2.len());
    
    for (uint64_t i = 0; i < N; ++i){
      ASSERT_EQ(vals[i], array2.Get(i));
    }
  }
}


