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

#ifndef LZ_TREE_FIXED_ARRAY_HPP_
#define LZ_TREE_FIXED_ARRAY_HPP_

#include <vector>
#include <iostream>

namespace lztree{

class FixedArray{
public:
  FixedArray();
  ~FixedArray();

  void Clear();
  void PushBack(uint64_t val);
  void Write(std::ostream& os) const;
  void Read(std::istream& is);
  uint64_t Get(uint32_t ind) const;

  uint32_t num() const{
    return num_;
  }

  uint8_t len() const{
    return len_;
  }

private:
  static void SetBits(std::vector<uint64_t>& bits, uint32_t ind, uint8_t len, uint64_t val);
  static uint64_t GetBits(const std::vector<uint64_t>& bits, uint32_t ind, uint8_t len);
  void Resize(uint8_t len);
  std::vector<uint64_t> bits_;
  uint32_t num_;
  uint8_t len_;
};

}

#endif // LZ_TREE_FIXED_ARRAY_HPP_
