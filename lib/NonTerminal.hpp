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

#ifndef LZTREE_NONTERMINAL_HPP_
#define LZTREE_NONTERMINAL_HPP_

#include <stdint.h>

namespace lztree{

struct NonTerminal{
  NonTerminal() : level(-1), id(0) {}
  NonTerminal(int8_t level, uint32_t id) : 
    level(level), id(id) {}
  int8_t level;
  uint32_t id;
};

}

#endif // LZTREE_NONTERMINAL_HPP_
