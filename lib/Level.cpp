#include <iostream>
#include <map>
#include <cmath>
#include <set>
#include "level.hpp"
using namespace std;

namespace lztree{

Level::Level() : max_id_(0){
}

Level::Level(uint32_t max_id) : max_id_(max_id){
}

State Level::AddBinary(const State& x1, const State& x2){
  State ret = LookupRule(x1.id, x2.id+1);
  ret.is_binary = true;
  if (!ret.is_copy){
    Write(x1);
    Write(x2);
  }
  return ret;
}

State Level::AddUnary(const State& x){
  State ret = LookupRule(x.id, 0);

  ret.is_binary = false;
  if (!ret.is_copy){
    Write(x);
  }
  return ret;
}

State Level::LookupRule(uint32_t x1, uint32_t x2){
  uint64_t x = ((uint64_t)x1 << 32) + x2;
  rule_t::const_iterator it = rule_.find(x);
  State ret;
  ret.is_terminal = false;
  if (it != rule_.end()){
    ret.is_copy = true;
    ret.id = it->second;
  } else {
    ret.is_copy = false;
    ret.id = max_id_;
    rule_[x] = ret.id;
    ++max_id_;
  }
  return ret;
}

void Level::IncrMaxID(){
  ++max_id_;
}

uint32_t Level::Write(const State& x){
  uint32_t pos = copies_.num();
  copies_.PushBack(x.is_copy);
  if (x.is_copy){
    copied_ids_.PushBack(x.id);
  } else {
    pos = tree_.num();
    tree_.PushBack(x.is_binary);
  }
  return pos;
}

void Level::Write(ostream& os) const{
  copies_.Write(os);
  tree_.Write(os);
  copied_ids_.Write(os);
}

void Level::PrintForDebug() const {
  cout << "copies = " << copies_.num() / 8 << endl;
  cout << "tree   = " << tree_.num() / 8 << endl;
  cout << "copyid = " << copied_ids_.num() * copied_ids_.len() / 8 << endl;
  cout << "copies_     ";
  for (uint32_t i = 0; i < copies_.num(); ++i){
    if (copies_.GetBit(i)){
      cout << "1";
    } else {
      cout << "0";
    }
  }
  cout << endl;
  

  cout << "tree_       ";
  for (uint32_t i = 0; i < tree_.num(); ++i){
    if (tree_.GetBit(i)){
      cout << "1";
    } else {
      cout << "0";
    }
  }
  cout << endl;
  cout << "copied_ids_ num()=" << copied_ids_.num() << " len()=" << (uint32_t)copied_ids_.len() << endl;
  cout << "copied_ids_ ";
  
  map<uint32_t, uint32_t> counter;
  map<uint32_t, uint32_t> ids;
  for (uint64_t i = 0; i < copied_ids_.num(); ++i){
    uint32_t id = copied_ids_.Get(i);
    if (ids.find(id) != ids.end()){
      counter[i - ids[id] - 1]++;
      cout <<  id << ":" << i - ids[id] - 1 << " ";
    } else {
      counter[- 1]++;
      cout << id << ":-1 ";
    }
    ids[id] = i;
  }
  cout << endl;
  for (map<uint32_t, uint32_t>::const_iterator it = counter.begin();
       it != counter.end(); ++it){
    cout << it->first << ":" << it->second << " " << (float) it->second / copied_ids_.num() << endl;
  }
  cout << "ids.size()= " << ids.size() << " " << (float)ids.size() / copied_ids_.num() << endl;
}

}
