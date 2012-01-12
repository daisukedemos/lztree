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


#ifndef LZ_TREE_BITVEC_HPP_
#define LZ_TREE_BITVEC_HPP_

#include <vector>
#include <iostream>
#include <stdint.h>

namespace lztree{

class BitVec{
  static const uint64_t BLOCKSIZE = 64;
public:
  BitVec();
  void Clear();
  void PushBack(uint64_t bit);
  uint32_t GetBit(uint32_t pos) const;
  uint32_t Rank(uint32_t pos) const;
  uint32_t num() const{
    return num_;
  }
  uint32_t one_num() const {
    return one_num_;
  }
  void Write(std::ostream& os) const;
  void Read(std::istream& is);
  
private:
  void AddBlock();
  inline static uint64_t PopCount(uint64_t x){
    x = x - ((x & 0xAAAAAAAAAAAAAAAALLU) >> 1);
    x = (x & 0x3333333333333333LLU) + ((x >> 2) & 0x3333333333333333LLU);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FLLU;
    return x * 0x0101010101010101LLU >> 56;
  }

  std::vector<uint64_t> bits_;
  std::vector<uint32_t> lblocks_;
  std::vector<uint8_t> sblocks_;
  uint32_t num_;
  uint32_t one_num_;
};

}

#endif // LZ_TREE_BITVEC_HPP_
