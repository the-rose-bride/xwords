#ifndef DEFS_H
#define DEFS_H

#include <utility>

// Configurable hash defines
#define BOARD_DEFAULT_EMPTY_CHAR '.'

#define GAME_WIDTH 14
#define GAME_HEIGHT 14
#define GAME_SIZE 14

#define DICTIONARY_FILE "/usr/share/dict/words"

// Class definitions
enum WordDir { E_Across, E_Down };

typedef std::pair<int, int> Offset;
typedef std::pair<int,int> Isect;

struct Word {
  WordDir dir;
  const char* word;
  Offset start;
  Word(WordDir dir,const char *word,Offset start) :
    dir(dir),
    word(word),
    start(start)
  {}
};

#endif // DEFS_H
