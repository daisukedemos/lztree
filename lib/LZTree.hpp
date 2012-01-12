/* 
 *  Copyright (c) 2012 Daisuke Okanohara
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 */

#ifndef LZ_TREE_LZTREE_HPP_
#define LZ_TREE_LZTREE_HPP_

#include <stdint.h>
#include "Queue.hpp"
#include "Level.hpp"
#include "State.hpp"
#include "NonTerminal.hpp"

namespace lztree{

class LZTree{
public:

  LZTree();
  ~LZTree();

  void Clear();
  void Append(uint32_t c);
  NonTerminal Flush();
  void Decode(const NonTerminal& nt, 
              std::vector<uint32_t>& ret) const;
  void Write(std::ostream& os) const;
  uint32_t num() const{
    return num_;
  }
  void PrintForDebug();

private:
  void DecodeInternal(int8_t level, uint32_t id,
                      std::vector<uint32_t>& ret) const;
  void AppendWithLevel(const State& c, uint8_t level);
  std::vector<Queue> queues_;
  std::vector<Level> levels_;
  uint32_t num_;
  uint32_t flush_num_;
};

}

#endif // LZ_TREE_LZTREE_HPP_
