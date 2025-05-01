#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "defs.hpp"

#include <vector>

class CrossWordsAlgorithm
{
private:
  const char *dictFile; // set by ctor
public:
  CrossWordsAlgorithm(const char *dictFile) : dictFile(dictFile){}
  void run(std::vector<Word> &entries, int seed);
};

#endif // ALGORITHM_H
