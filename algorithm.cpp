#include "algorithm.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>

typedef std::vector<Isect> IsectList;

// Helper functions

int read_valid_words_from_file_into_vector(const char *fileName, std::vector<const char*>& wordVector)
{
  FILE *file = fopen(fileName, "r");
  if (!file) {
    fprintf(stderr, "Unable to open %s\n", fileName);
    return 1;
  }

  char *line = NULL;
  size_t size = 0;
  int ret = 0;
  while ((ret = getline(&line, &size, file)) > 0)
  {
    strtok(line, "\n"); // trim newline

    int i, len = strlen(line);
    for (i = 0; i < len; ++i) {
      char c = tolower(line[i]);
      if (c < 'a' || c > 'z') break;
    }
    if (i < len) continue;

    wordVector.push_back(strdup(line));
  }

  fclose(file);
  free(line); // get line allocates memory
  return 0; // success
}

void get_intersections_between_two_words(const char *word1,
                                         const char *word2,
                                         IsectList &isects)
{
  for (int i = 0; i < strlen(word1); ++i) {
    char c1 = word1[i];
    for (int j = 0; j < strlen(word2); ++j) {
      char c2 = word2[j];
      if (c1 == c2) {
        isects.push_back(Isect(i,j));
      }
    }
  }
}

const char *get_random_word(std::vector<const char *> &words)
{
  return words[rand() % words.size()];
  // can check for duplicates if we want
}

WordDir otherWordDir(WordDir dir)
{
  if (dir == E_Across) return E_Down;
  return E_Across;
}

// The algorithm

void CrossWordsAlgorithm::run(std::vector<Word> &entries)
{
  // Read the dictionary in 
  std::vector<const char *> words;
  read_valid_words_from_file_into_vector(dictFile, words);
  printf("Read %zu words from %s\n", words.size(), dictFile);

  // select two random words
  const char *word1 = get_random_word(words);
  const char *word2 = get_random_word(words);
  
  // get a list of possible intersections
  
  IsectList isects;
  get_intersections_between_two_words(word1, word2, isects);

  if (0 == isects.size()) {
    fprintf(stderr, "No intersections between %s and %s\n", word1, word2);
    return;
  }
  
  // add those two words to the list
  Isect isect = isects[0];
  entries.push_back(Word(E_Across, word1, Offset(0, isect.second)));
  entries.push_back(Word(E_Down, word2, Offset(isect.first, 0)));

  // try a third word
  const char *word3 = get_random_word(words);

  // find all intersections on the board
  std::vector<std::pair<Word,IsectList>> board_intersections;

  for (int i = 0; i < entries.size(); i++)
  {
    Word word = entries[i];
    IsectList isects;
    get_intersections_between_two_words(word3, word.word, isects);
    board_intersections.push_back(std::pair<Word,IsectList>(word,isects));
  }

  // add the first intersection
  std::pair<Word,IsectList> board_isect = board_intersections[0];
  isect = board_isect.second[0];
  Word word_on_board = board_isect.first;

  WordDir dir = otherWordDir(word_on_board.dir);
  Offset offset;

  if (E_Across == dir)
  {
    offset.first = word_on_board.start.first - isect.first;
    offset.second = word_on_board.start.second + isect.second;
  }
  else
  {
    // we want to place a word going Down
    // this word's X value will be the other word's x + the other words offset
    offset.first = word_on_board.start.first + isect.second;
    // this word's Y value will be the other word's y - our offset
    offset.second = word_on_board.start.second - isect.first;
  }
  
  entries.push_back(Word(dir, word3, offset));
}
