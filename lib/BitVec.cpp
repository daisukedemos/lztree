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

#include "BitVec.hpp"

using namespace std;

namespace lztree{

static const uint64_t LBLOCKSIZE = 256;
static const uint64_t BLOCKSIZE = 64;

BitVec::BitVec() : num_(0), one_num_(0){
}

void BitVec::Clear(){
  bits_.clear();
  lblocks_.clear();
  sblocks_.clear();
  num_ = 0;
  one_num_ = 0;
}

void BitVec::PushBack(uint64_t bit){
  if ((num_ % BLOCKSIZE) == 0){
    AddBlock();
  }
  if (bit > 0){
    ++one_num_;
    bits_.back() |= (1LLU << (num_ % BLOCKSIZE));
  }
  ++num_;
}

uint32_t BitVec::GetBit(uint32_t pos) const{
  return (bits_[pos / BLOCKSIZE] >> (pos % BLOCKSIZE)) & 1LLU;
}

uint32_t BitVec::Rank(uint32_t pos) const{
  if (pos == num_) return one_num_;
  return lblocks_[pos / LBLOCKSIZE] 
    + sblocks_[pos / BLOCKSIZE]
    + PopCount(bits_[pos / BLOCKSIZE] & ((1LLU << (pos % BLOCKSIZE)) - 1));
}

void BitVec::AddBlock(){
  if (num_ % LBLOCKSIZE == 0){
    lblocks_.push_back(one_num_);
  }
  sblocks_.push_back(one_num_ - lblocks_[num_ / LBLOCKSIZE]);
  bits_.push_back(0LLU);
}

void BitVec::Write(ostream& os) const{
  os.write((const char*)&num_, sizeof(num_));
  os.write((const char*)&one_num_, sizeof(one_num_));
  os.write((const char*)&bits_[0], sizeof(bits_[0]) * bits_.size());
  os.write((const char*)&lblocks_[0], sizeof(lblocks_[0]) * lblocks_.size());
  os.write((const char*)&sblocks_[0], sizeof(sblocks_[0]) * sblocks_.size());
}

void BitVec::Read(istream& is) {
  is.read((char*)&num_, sizeof(num_));
  is.read((char*)&one_num_, sizeof(one_num_));
  bits_.resize((num_ + BLOCKSIZE - 1) / BLOCKSIZE);
  is.read((char*)&bits_[0], sizeof(bits_[0]) * bits_.size());
  lblocks_.resize((num_ + LBLOCKSIZE - 1) / LBLOCKSIZE);
  is.read((char*)&lblocks_[0], sizeof(lblocks_[0]) * lblocks_.size());
  sblocks_.resize((num_ + BLOCKSIZE - 1) / BLOCKSIZE);
  is.read((char*)&sblocks_[0], sizeof(sblocks_[0]) * sblocks_.size());
}




}
