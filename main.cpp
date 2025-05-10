#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <vector>
#include <utility>
#include <unistd.h>

#include "defs.hpp"
#include "board.hpp"
#include "algorithm.hpp" 

class Game
{
private:
  Board board;
  std::vector<Word> words;

public:
  // Constructors
  Game() : board(GAME_SIZE,
                 GAME_SIZE),
           words()
  {}

  Game(int size) : board(size,
                         size),
                   words()
  {}
  
  Game(std::vector<Word> &words) : board(GAME_SIZE,
                                         GAME_SIZE),
                                    words(words)
  {}
                                    
  // Start playing
  void start() {
    for (int i = 0; i < words.size(); ++i) {
      if (0 != board.addWord(words[i]))
      {
        fprintf(stderr,
                "Failed to add %s to board - out of bounds.\n",
                words[i].word);
      }
    }
    board.render();
  }

  // Algorithm to make a game
  void generate(const char *dictFile, int seed) {
    CrossWordsAlgorithm alg(dictFile);
    alg.run(words, seed); // should output a list of words - just printf for now
  }
};

int main(int argc, char **argv)
{
  // config overrides
  int seed = 0;
  int size = GAME_SIZE;
  const char *dict_file = DICTIONARY_FILE;
  
  while (1)
  {
    const char *shortopts = "s:S:d:";

    int c = getopt(argc, argv, shortopts);
    if (-1 == c) break;

    int read_int = 0;
    
    switch (c)
    {
    case 's':
      read_int = atoi(optarg);
      if (read_int) size = read_int;
      printf("Running with size %dx%d\n", size, size);
      break;
    case 'S':
      read_int = atoi(optarg);
      if (read_int) seed = read_int;
      printf("Running with seed=%d\n", seed);
      break;      

    case 'd':
      if (access(optarg, F_OK) == 0)
        dict_file = optarg;
      else
        printf("Could not find file %s\n", optarg);
      printf("Using dict: %s\n", dict_file ? "dict_file" : "DEFAULT");
      break;
    }
  }

  Game game(size);
  game.generate(dict_file, seed);
  game.start();
  return 0;
}
