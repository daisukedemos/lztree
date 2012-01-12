#include <gtest/gtest.h>
#include "LZTree.hpp"

using namespace lztree;
using namespace std;

TEST(lztree, trivial){
  LZTree c;
  ASSERT_EQ(0, c.num());
  NonTerminal nt = c.Flush();
  ASSERT_EQ(-1, nt.level);
  ASSERT_EQ(0, nt.id);
  vector<uint32_t> ret;
  c.Decode(nt, ret);
  ASSERT_EQ(0, ret.size());
}

TEST(lztree, random){
  LZTree c;
  const uint32_t N = 10000;
  const uint32_t alpha_num = 1000;
  vector<uint32_t> vals;
  for (uint32_t i = 0; i < N; ++i){
    uint32_t val = rand() % alpha_num;
    c.Append(val);
    vals.push_back(val);
  }
  NonTerminal nt = c.Flush();
  ASSERT_EQ(N, c.num());

  vector<uint32_t> ret;
  c.Decode(nt, ret);
  ASSERT_EQ(N, ret.size());
  for (uint32_t i = 0; i < N; ++i){
    ASSERT_EQ(vals[i], ret[i]);
  }
}

TEST(lztree, many){
  LZTree c;
  c.Append(0);
  c.Append(1);
  c.Append(2);
  NonTerminal nt1 = c.Flush();
  c.Append(1);
  c.Append(0);
  c.Append(1);
  c.Append(0);
  NonTerminal nt2 = c.Flush();
  NonTerminal nt3 = c.Flush();
  c.Append(0);
  NonTerminal nt4 = c.Flush();

  c.PrintForDebug();

  vector<uint32_t> ret1;
  c.Decode(nt1, ret1);
  ASSERT_EQ(3, ret1.size());
  EXPECT_EQ(0, ret1[0]);
  EXPECT_EQ(1, ret1[1]);
  EXPECT_EQ(2, ret1[2]);

  vector<uint32_t> ret2;
  c.Decode(nt2, ret2);
  ASSERT_EQ(4, ret2.size());
  EXPECT_EQ(1, ret2[0]);
  EXPECT_EQ(0, ret2[1]);
  EXPECT_EQ(1, ret2[2]);
  EXPECT_EQ(0, ret2[3]);

  vector<uint32_t> ret3;
  c.Decode(nt3, ret3);
  ASSERT_EQ(0, ret3.size());

  vector<uint32_t> ret4;
  c.Decode(nt4, ret4);
  ASSERT_EQ(1, ret4.size());
  EXPECT_EQ(0, ret4[0]);
}

TEST(lztree, failed_test){
  LZTree c;
  vector<string> values;
  values.push_back("!");
  values.push_back("!!");
  values.push_back("!!!");
  values.push_back("!!!Fuck_You!!!");
  vector<NonTerminal> nts;
  for (size_t i = 0; i < values.size(); ++i){
    const string& s = values[i];
    for (size_t j = 0; j < s.size(); ++j){
      c.Append((uint8_t)s[j]);
    }
    NonTerminal nt = c.Flush();
    nts.push_back(nt);
  }

  c.PrintForDebug();

  for (size_t i = 0; i < nts.size(); ++i){
    vector<uint32_t> ret;
    c.Decode(nts[i], ret);
    const string& value = values[i];
    ASSERT_EQ(value.size(), ret.size());
    for (size_t j = 0; j < value.size(); ++j){
      EXPECT_EQ(value[j], ret[j]);
    }
  }
}

TEST(lztree, manyrandom){
  LZTree c;
  vector<NonTerminal> nts;
  vector<vector<uint32_t> > valss;
  const int ITER_NUM = 1000;
  for (int iter = 0; iter < ITER_NUM; ++iter){
    uint32_t N = rand() % 10000 + 1;
    uint32_t alpha_num = (rand() % 30) + 1;
    vector<uint32_t> vals;
    for (uint32_t i = 0; i < N; ++i){
      uint32_t val = rand() % alpha_num;
      c.Append(val);
      vals.push_back(val);
    }
    NonTerminal nt = c.Flush();
    nts.push_back(nt);
    valss.push_back(vals);
  }

  c.PrintForDebug();

  for (int iter = 0; iter < ITER_NUM; ++iter){
    vector<uint32_t> ret;
    c.Decode(nts[iter], ret);
    const vector<uint32_t>& vals = valss[iter];
    EXPECT_EQ(vals.size(), ret.size()) << " iter=" << iter;
  }

  for (int iter = 0; iter < ITER_NUM; ++iter){
    vector<uint32_t> ret;
    c.Decode(nts[iter], ret);
    const vector<uint32_t>& vals = valss[iter];
    ASSERT_EQ(vals.size(), ret.size());
    for (uint32_t i = 0; i < vals.size(); ++i){
      ASSERT_EQ(vals[i], ret[i]);
    }
  }
}
