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

#ifndef LZ_TREE_QUQUE_HPP_
#define LZ_TREE_QUQUE_HPP_

#include <algorithm>
#include <stdint.h>
#include "State.hpp"

namespace lztree{

class Queue{

public:
  enum {
    QUEUE_MAX_LEN = 4
  };
  
  Queue();
  ~Queue();
  void Push(const State& x);
  State Pop();
  bool IsFrontPair() const;
  uint32_t num() const{
    return num_;
  }

private:
  uint32_t Lookup(uint32_t offset) const;
  bool IsRepetition(uint32_t offset) const;
  bool IsMinimal(uint32_t offset) const;
  bool IsMaximal(uint32_t offset) const;
  static bool IsIncreasing(uint32_t w, uint32_t x, uint32_t y, uint32_t z);
  uint8_t LCADepth(uint32_t x, uint32_t y) const;
  
  State buf_[QUEUE_MAX_LEN];
  uint32_t begin_pos_;
  uint32_t end_pos_;
  uint32_t num_;
};

}

#endif // TREE_LZ_QUEUE_HPP_
