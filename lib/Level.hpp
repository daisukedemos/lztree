#ifndef LZ_TREE_LEVEL_HPP_
#define LZ_TREE_LEVEL_HPP_

#include <vector>
#include <iostream>
#include <tr1/unordered_map>
#include "State.hpp"
#include "BitVec.hpp"
#include "FixedArray.hpp"

namespace lztree{

struct Level{
public:
  Level();
  Level(uint32_t max_id);
  State AddBinary(const State& x1, const State& x2);
  State AddUnary(const State& x);
  void IncrMaxID();
  void Write(std::ostream& os) const;

  void PrintForDebug() const;

  State LookupRule(uint32_t x1, uint32_t x2);
  uint32_t Write(const State& x);

  BitVec copies_;
  BitVec tree_;
  FixedArray copied_ids_;
  typedef std::tr1::unordered_map<uint64_t, uint32_t> rule_t;
  rule_t rule_;
  uint32_t max_id_;
};

}

#endif // LZ_TREE_LEVEL_HPP_
