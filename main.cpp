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
  Game() : board(GAME_WIDTH,
                 GAME_HEIGHT),
           words()
  {}

  Game(int size) : board(size,
                         size),
                   words()
  {}
  
  Game(std::vector<Word> &words) : board(GAME_WIDTH,
                                          GAME_HEIGHT),
                                    words(words)
  {}
                                    
  // Start playing
  void start() {
    for (int i = 0; i < words.size(); ++i) {
      board.addWord(words[i]);
    }
    board.render();
  }

  // Algorithm to make a game
  void generate(const char *dictFile) {
    CrossWordsAlgorithm alg(dictFile);
    alg.run(words); // should output a list of words - just printf for now
  }
};

int main(int argc, char **argv)
{
  Game game(GAME_SIZE);
  game.generate(DICTIONARY_FILE); // load the linux dictionary
  game.start();
  return 0;
}
