#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <vector>
#include <utility>

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
  int seed = 0;

  if (argc >= 2)
  {
    int new_seed = atoi(argv[1]);
    if (new_seed) seed = new_seed;
    printf("Running with seed=%d\n", seed);
  }
  
  Game game(GAME_SIZE);
  game.generate(DICTIONARY_FILE, seed); // load the linux dictionary
  game.start();
  return 0;
}
