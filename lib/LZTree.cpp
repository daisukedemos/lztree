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

#include "LZTree.hpp"

using namespace std;

namespace lztree{

LZTree::LZTree() : num_(0), flush_num_(0) {
}

LZTree::~LZTree(){
}

void LZTree::Clear(){
  queues_.clear();
  levels_.clear();
  num_ = 0;
  flush_num_ = 0;
}

void LZTree::Append(uint32_t c){
  AppendWithLevel(State(true, true, false, c), 0);
  ++num_;
}

void LZTree::AppendWithLevel(const State& x, uint8_t level){
  if (level >= queues_.size()){
    queues_.push_back(Queue());
    levels_.push_back(Level(flush_num_));
    for (uint32_t i = 0; i < flush_num_; ++i){
      levels_[level].Write(State(false, false, false, 0));
    }
  }
  Queue& q = queues_[level];
  Level& l = levels_[level];
  q.Push(x);
  if (q.num() == Queue::QUEUE_MAX_LEN){
    if (q.IsFrontPair()){
      State x1 = q.Pop();
      State x2 = q.Pop();
      State bid = l.AddBinary(x1, x2);
      AppendWithLevel(bid, level+1);
    } else {
      State x1 = q.Pop();
      State x2 = q.Pop();
      State x3 = q.Pop();
      State uid = l.AddUnary(x1);
      State bid = l.AddBinary(x2, x3);
      AppendWithLevel(uid, level+1);
      AppendWithLevel(bid, level+1);
    }
  }
}
 
NonTerminal LZTree::Flush(){
  for (size_t i = 0; i < queues_.size(); ++i){
    while (queues_[i].num() >= 2){
      State x1 = queues_[i].Pop();
      State x2 = queues_[i].Pop();
      State bid = levels_[i].AddBinary(x1, x2);
      AppendWithLevel(bid, i+1);
    }
    if (queues_[i].num() == 1 && 
        queues_.size() > i+1 && 
        queues_[i+1].num() >= 1){
      State x = queues_[i].Pop();
      State uid = levels_[i].AddUnary(x);
      AppendWithLevel(uid, i+1);
    } else if (queues_[i].num() == 1){
      State x = queues_[i].Pop();
      if (!x.is_terminal && x.is_copy){
        return NonTerminal(i, x.id);
      }
      uint32_t id = levels_[i].Write(x);
      levels_[i].IncrMaxID();
      for (uint8_t j = i+1; j < queues_.size(); ++j){
        levels_[j].Write(State(false, false, false, 0));
        levels_[j].IncrMaxID();
      }
      ++flush_num_;
      return NonTerminal(i, id);
    }
  }
  return NonTerminal();
}

void LZTree::Decode(const NonTerminal& nt, vector<uint32_t>& ret) const{
  int8_t level = nt.level;
  uint32_t id = nt.id;
  if (level == -1) return;
  const Level& l = levels_[level];
  if (level == 0){
    ret.push_back(l.copied_ids_.Get(id));
    return;
  }

  uint32_t is_binary = l.tree_.GetBit(id);
  uint32_t next_id = l.tree_.Rank(id) + id;
  if (is_binary){
    DecodeInternal(level-1, next_id, ret);
    DecodeInternal(level-1, next_id+1, ret);
  } else {
    DecodeInternal(level-1, next_id, ret);
  }
}

void LZTree::DecodeInternal(int8_t level, uint32_t id,
                            vector<uint32_t>& ret) const{
  const Level& l = levels_[level];
  if (level == 0){
    ret.push_back(l.copied_ids_.Get(id));
    return;
  }

  uint32_t tree_pos = 0;
  if (l.copies_.GetBit(id)){
    uint32_t r = l.copies_.Rank(id);
    tree_pos = l.copied_ids_.Get(r);
  } else {
    tree_pos = id - l.copies_.Rank(id);
  }
  //cout << "tree_pos=" << tree_pos << endl;
  uint32_t is_binary = l.tree_.GetBit(tree_pos);
  uint32_t next_id = l.tree_.Rank(tree_pos) + tree_pos;
  if (is_binary){
    DecodeInternal(level-1, next_id, ret);
    DecodeInternal(level-1, next_id+1, ret);
  } else {
    DecodeInternal(level-1, next_id, ret);
  }
  
}

void LZTree::Write(ostream& os) const {
  os.write((const char*)&num_, sizeof(num_));
  uint8_t level_num = levels_.size();
  os.write((const char*)&level_num, sizeof(level_num) * 1);
  for (size_t i = 0; i < levels_.size(); ++i){
    levels_[i].Write(os);
  }
}

void LZTree::PrintForDebug(){
  for (size_t i = 0; i < levels_.size(); ++i){
    cout << "level=" << i << endl;
    levels_[i].PrintForDebug();
  }
}

}
