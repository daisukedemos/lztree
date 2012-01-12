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

#include <iostream>
#include <fstream>
#include "../lib/LZTree.hpp"
#include "cmdline.h"

using namespace std;

void ProcessFile(const string& fn, int maximum_size){
  ifstream ifs(fn.c_str());
  if (!ifs){
    throw string("cannot open ") + fn;
  }
  
  lztree::LZTree comp;
  int size = 0;
  vector<string> lines;
  vector<lztree::NonTerminal> nts;
  for (string line; getline(ifs, line); ){
    lines.push_back(line);
    for (size_t i = 0; i < line.size(); ++i){
      comp.Append(static_cast<uint8_t>(line[i]));
      ++size;
    }
    lztree::NonTerminal nt =comp.Flush();
    nts.push_back(nt);
    if ((size % 1048576) == 0) {
      cout << size / 1048576 << " MB\t" << endl;
    }
  }
  if (size == 0) return;

  ostringstream os;
  comp.Write(os);
  cout << "os.size()=" << os.str().size() << " " << (float)os.str().size() / size << endl;

  for (size_t i = 0; i < nts.size(); ++i){
    vector<uint32_t> output;
    comp.Decode(nts[i], output);

    const string& line = lines[i];
    if (output.size() != line.size()){
      cout << "i = " << i << endl;
      cout << "level = " << (int)nts[i].level << " id = " << nts[i].id << endl;
      cout << "size error decode=" << output.size() << " correct=" << line.size() << endl;
    }
  }

}

int main(int argc, char* argv[]){
  cmdline::parser p;
  p.add<string>("input", 'i', "input file to be compressed", true);
  p.add<int>("maxsize", 'm', "maximum size to be processed", false, 1000000000);
  p.set_program_name("gcomp");

  if (!p.parse(argc, argv)){
    cerr << p.error() << endl
         << p.usage() << endl;
    return -1;
  }

  try {
    ProcessFile(p.get<string>("input"), p.get<int>("maxsize"));
  } catch (const string& what){
    cerr << what << endl;
    return -1;
  }

  return 0;
}
