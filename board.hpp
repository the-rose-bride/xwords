#ifndef BOARD_H
#define BOARD_H

#include "defs.hpp"

#include <cstdio>
#include <cstring>

class Board
{
private:
  int height, width;
  char **board;

public:
  Board(int height, int width) : height(height),
                                 width(width)
  {
    // Allocate memory for new 2d map
    board = new char*[height];
    for (int i = 0; i < height; ++i) {
      board[i] = new char[width];
      for (int j = 0; j < width; ++j) {
        board[i][j] = BOARD_DEFAULT_EMPTY_CHAR;
      }
    }
  }

  void render()
  {
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        putchar(board[i][j]);
        putchar(' ');
      }
      putchar('\n');
    }
  }

  void addWord(const Word& word)
  {
    int x = word.start.first;
    int y = word.start.second;

    for (int i = 0; i < strlen(word.word); ++i)
    {
      board[y][x] = word.word[i];
      if (E_Across == word.dir) x++;
      else if (E_Down == word.dir) y++;
    }
  }
};

#endif // BOARD_H
