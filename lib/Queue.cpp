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

#include <algorithm>
#include "Queue.hpp"

namespace lztree{

using namespace std;

Queue::Queue() : begin_pos_(0), end_pos_(0),
                 num_(0) {
  for (size_t i = 0; i < QUEUE_MAX_LEN; ++i){
    buf_[i].id = 0;
  }
}

Queue::~Queue(){
}

void Queue::Push(const State& x){
  buf_[end_pos_ % QUEUE_MAX_LEN] = x;
  ++end_pos_;
  ++num_;
}

State Queue::Pop(){
  State x = buf_[begin_pos_ % QUEUE_MAX_LEN];
  ++begin_pos_;
  --num_;
  return x;
}

bool Queue::IsFrontPair() const{
  if (IsRepetition(0)) return true;
  if (IsRepetition(1)) return false;
  if (IsRepetition(2)) return true;
  if (IsMinimal(0) || IsMaximal(0)) return true;
  if (IsMinimal(1) || IsMaximal(1)) return false;
  return true;
}

uint32_t Queue::Lookup(uint32_t offset) const{
  return buf_[(begin_pos_ + offset) % QUEUE_MAX_LEN].id;
}

bool Queue::IsRepetition(uint32_t offset) const{
  if (Lookup(offset) == Lookup(offset+1)) return true;
  return false;
}

bool Queue::IsMinimal(uint32_t offset) const{
  uint32_t w0 = Lookup(offset-1);
  uint32_t w1 = Lookup(offset);
  uint32_t w2 = Lookup(offset+1);
  if (w0 > w1 && w1 < w2){
    return true;
  } else {
    return false;
  }
}

bool Queue::IsMaximal(uint32_t offset) const{
  uint32_t w0 = Lookup(offset-1);
  uint32_t w1 = Lookup(offset);
  uint32_t w2 = Lookup(offset+1);
  uint32_t w3 = Lookup(offset+2);
  if (!IsIncreasing(w0, w1, w2, w3) && !IsIncreasing(w3, w2, w1, w0)){
    return false;
    }
  if (LCADepth(w1, w2) > LCADepth(w0, w1) &&
      LCADepth(w1, w2) > LCADepth(w2, w3)){
    return true;
  } else {
    return false;
  }
}

bool Queue::IsIncreasing(uint32_t w, uint32_t x, uint32_t y, uint32_t z){
  return (w < x && x < y && y < z);
}

uint8_t Queue::LCADepth(uint32_t x, uint32_t y) const{
  if (x == y) return 32LLU;
  uint32_t dif = x ^ y;
  uint32_t depth = 0;
  for (; ; ++depth){
    if ((dif >> depth) & 1LLU == 1){
      break;
    }
  }
  return depth;
}

}
