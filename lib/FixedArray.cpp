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

#include <cassert>
#include "FixedArray.hpp"

using namespace std;

namespace lztree{

static const uint64_t BLOCKSIZE = 64;

FixedArray::FixedArray() : num_(0), len_(0){
}

FixedArray::~FixedArray(){
}

void FixedArray::Clear(){
  bits_.clear();
  num_ = 0;
  len_ = 0;
}

void FixedArray::PushBack(uint64_t val){
  uint8_t len = 0;
  while (val >> len){
    ++len;
  }
  if (len > len_){
    Resize(len);
  }
  if ((num_ + 1) * len_ > bits_.size() * BLOCKSIZE){
    bits_.push_back(0);
  }
  SetBits(bits_, num_, len_, val);
  ++num_;
}

uint64_t FixedArray::Get(uint32_t ind) const{
  return GetBits(bits_, ind, len_);
}

void FixedArray::SetBits(vector<uint64_t>& bits, uint32_t ind, uint8_t len, uint64_t val){
  if (len == 0) return;
  uint32_t block_pos = (uint64_t)ind * len / BLOCKSIZE;
  uint32_t block_offset = (ind * len) % BLOCKSIZE;
  assert(block_pos < bits.size());
  bits[block_pos] |= val << block_offset;
  if (block_offset + len > BLOCKSIZE){
    assert(block_pos+1 < bits.size());
    bits[block_pos + 1] |= val >> (BLOCKSIZE - block_offset);
  }
}

uint64_t FixedArray::GetBits(const vector<uint64_t>& bits, uint32_t ind, uint8_t len){
  if (len == 0) return 0;
  uint64_t block_pos = (ind * len) / BLOCKSIZE;
  uint64_t block_offset = (ind * len) % BLOCKSIZE;
  uint64_t ret = bits[block_pos] >> block_offset;
  assert(block_pos < bits.size());
  if (block_offset + len > BLOCKSIZE){
    assert(block_pos+1 < bits.size());
    ret |= bits[block_pos+1] << (BLOCKSIZE - block_offset);
  }
  ret &= ((1LLU << len) - 1);
  return ret;
}

void FixedArray::Resize(uint8_t len){
  vector<uint64_t> bits((len * num_ + BLOCKSIZE - 1) / BLOCKSIZE);
  for (uint32_t i = 0; i < num_; ++i){
    uint64_t val = GetBits(bits_, i, len_);
    SetBits(bits, i, len, val);
  }
  bits_.swap(bits);
  len_ = len;
}

void FixedArray::Write(ostream& os) const{
  os.write((const char*)&num_, sizeof(num_));
  os.write((const char*)&len_, sizeof(len_));
  os.write((const char*)&bits_[0], sizeof(bits_[0]) * bits_.size());
}

void FixedArray::Read(istream& is) {
  is.read((char*)&num_, sizeof(num_));
  is.read((char*)&len_, sizeof(len_));
  bits_.resize((len_ * num_ + BLOCKSIZE - 1) / BLOCKSIZE);
  is.read((char*)&bits_[0], sizeof(bits_[0]) * bits_.size());
}

}
