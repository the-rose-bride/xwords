#ifndef DEFS_H
#define DEFS_H

#include <utility>

// Configurable hash defines
#define BOARD_DEFAULT_EMPTY_CHAR '.'

#define GAME_SIZE 30

#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 15

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
