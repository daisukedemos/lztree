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

#ifndef LZ_TREE_STATE_HPP_
#define LZ_TREE_STATE_HPP_

#include <stdint.h>

namespace lztree{

struct State{
  State(){}
  State(bool is_terminal, bool is_copy, bool is_binary, uint32_t id) : 
    is_terminal(is_terminal), is_copy(is_copy), is_binary(is_binary), id(id) {}
  bool is_terminal;
  bool is_copy;
  bool is_binary;
  uint32_t id;
};

}

#endif // STATE_HPP_
