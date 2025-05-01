#include "algorithm.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <ctime>

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
    if (len < MIN_WORD_LENGTH or len > MAX_WORD_LENGTH) continue;
    for (i = 0; i < len; ++i) {
      char c = (line[i]);
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

WordDir oppositeWordDir(WordDir dir)
{
  if (dir == E_Across) return E_Down;
  return E_Across;
}

Offset get_offset_from_word_given_intersect(Word word, Isect isect)
{
  WordDir dir = oppositeWordDir(word.dir);
  Offset offset;
  
  if (E_Across == dir)
  {
    offset.first = word.start.first - isect.first;
    offset.second = word.start.second + isect.second;
  }
  else
  {
    // we want to place a word going Down
    // this word's X value will be the other word's x + the other words offset
    offset.first = word.start.first + isect.second;
    // this word's Y value will be the other word's y - our offset
    offset.second = word.start.second - isect.first;
  }

  return offset;
}

bool is_word_and_offset_valid(const char *word, WordDir dir, Offset offset)
{
  bool valid;
  
  if (E_Across == dir)
  {
    valid = (offset.first >= 0 && (offset.first + strlen(word) < GAME_SIZE)
             && offset.second >= 0);
  }
  else
  {
    valid = (offset.first >= 0
             && offset.second >= 0 && (offset.second + strlen(word) < GAME_SIZE) );
  }

  return valid;
}

typedef std::pair<Word,IsectList> WordAndIsects;

void get_all_intersections_for_new_word(const char *word,
                                        std::vector<Word> &entries,
                                        std::vector<WordAndIsects> &board_intersections)
{
  for (int i = 0; i < entries.size(); i++)
  {
    Word entry = entries[i];
    IsectList isects;
    get_intersections_between_two_words(word, entry.word, isects);
    board_intersections.push_back(WordAndIsects(entry,isects));
  }
}

void correctlyOrderWords(Word& word1, Word& word2)
{
  bool doSwap = false;
  if (word1.dir == word2.dir)
  {
    // word 1 should always be lower or higher
    if (   word1.start.first > word2.start.first
        || word1.start.second > word2.start.second)
    {
      doSwap = true;
    }
  }
  else
  {
    // word1 should always be across
    if (word1.dir != E_Across)
    {
      doSwap = true;
    }
  }

  if (doSwap)
  {
    Word temp = word1;
    word1 = word2;
    word2 = temp;
  }
}

// true - can't co-exist
bool check_invalid_intersect_between_two_words(Word word1, Word word2)
{
  // if they cross, word1 is always across
  // if they don't word1 is lower/lefter
  correctlyOrderWords(word1, word2);

  if (word1.dir == word2.dir)
  {
    if (E_Across == word1.dir) // both across
    {
      // in-line or next to is a problem
      bool inline_or_adjacent = (word2.start.second - word1.start.second <= 1);
      bool x_overlap = (word2.start.first <= (word1.start.first + strlen(word1.word)));
      if (inline_or_adjacent && x_overlap) return true;
    }
    else // both down
    {
      // in-line or next to is a problem
      bool inline_or_adjacent = (word2.start.first - word1.start.first <= 1);
      bool y_overlap = (word2.start.second <= (word1.start.second + strlen(word1.word)));
      if (inline_or_adjacent && y_overlap) return true;
    }
    return false;
  }

  // this is the board isect
  Isect isect(word2.start.first, word1.start.second);

  // word1 is across
  // the index into word1 is given by the diff word2_x - word1_x
  int across_index = word2.start.first - word1.start.first;

  if (across_index == -1 || across_index == strlen(word1.word)) return true;
  if (across_index < 0 || across_index > strlen(word1.word)) return false;
  
  int down_index = word1.start.second - word2.start.second;

  if (down_index == -1 || down_index == strlen(word2.word)) return true;
  if (down_index < 0 || down_index > strlen(word2.word)) return false;

  // if there is an actual intersect check the character
  if (word1.word[across_index] != word2.word[down_index])
  {
    // fprintf(stderr, "Collision between %s and %s - %c != %c\n",
    //         word1.word,
    //         word2.word,
    //         word1.word[across_index],
    //         word2.word[down_index]);
    return true;
  }

  return false;
}

bool get_collision(std::vector<Word> &entries, Word newWord)
{
  for (int i = 0; i < entries.size(); ++i)
  {
    Word check = entries[i];

    if (check_invalid_intersect_between_two_words(newWord, check))
    {
      return true; // collision
    }
  }
  
  return false; // no collision 
}

Word try_to_place_word(const char *word, std::vector<Word> &entries)
{
  Word noWord(E_Across, "", Offset(0,0));
  
  // find all intersections on the board
  std::vector<WordAndIsects> board_intersections;
  get_all_intersections_for_new_word(word, entries, board_intersections);
  if (0 == board_intersections.size())
  {
    //fprintf(stderr, "No intersection to add %s\n", word);
    return noWord;
  }
  
  // Loop over first the words, and then the possible intersects to add a third
  // word to he board.
  for (int i = 0; i < board_intersections.size(); ++i)
  {
    WordAndIsects board_isect = board_intersections[i];
    Word word_on_board = board_isect.first;
    
    for (int j = 0; j < board_isect.second.size(); ++j)
    {
      Isect isect = board_isect.second[j];
      WordDir dir = oppositeWordDir(word_on_board.dir);
      Offset offset = get_offset_from_word_given_intersect(word_on_board, isect);

      Word retWord(dir, word, offset);
      
      bool collision = get_collision(entries, retWord);
      
      // check that this is valid before adding
      if (!is_word_and_offset_valid(word, dir, offset)
        || get_collision(entries, retWord))
      {
        // fprintf(stderr,
        //         "%s is not valid to add at %d,%d\n",
        //         word,
        //         offset.first,
        //         offset.second);
        continue;
      }

      return retWord;
    }
  }

  return noWord;
}

// The algorithm
void CrossWordsAlgorithm::run(std::vector<Word> &entries, int seed)
{
  srand(seed ? seed : time(NULL)); // set random seed
  
  // Read the dictionary in 
  std::vector<const char *> words;
  read_valid_words_from_file_into_vector(dictFile, words);
  printf("Read %zu words from %s\n", words.size(), dictFile);

  // This method is nicer than cramming into the corner
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
  printf("Placing %s and %s\n", word1, word2);
  Isect first_isect = isects[0];
  entries.push_back(Word(E_Across, word1, Offset(0, first_isect.second)));
  entries.push_back(Word(E_Down, word2, Offset(first_isect.first, 0)));

  // or just put the first random word in the corner
  // const char *firstWord = get_random_word(words);
  // entries.push_back(Word(E_Across, firstWord, Offset(0, 0)));

  // try 100 more times
  int fail = 0;
  while (fail++ < 100)
  {
    if (fail % 1000 == 0) fprintf(stderr, "%d tries since last success\n", fail);
    const char *word3 = get_random_word(words);
    Word place = try_to_place_word(word3, entries);
    if (strlen(place.word)) {
      printf("Added %s\n", word3);
      entries.push_back(place);
      fail = 0;
      //break; // success
    }
  }
}
