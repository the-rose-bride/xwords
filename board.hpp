#ifndef BOARD_H
#define BOARD_H

#include "defs.hpp"

#include <cstdio>
#include <cstring>
#include <ctype.h>

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
    bool border = false;
    for (int i = 0; i < height; ++i) {
      if (border && i == 0) {
        printf("   ");
        for (int j = 0; j < width; ++j)
        {
          printf("%02d ", (j + 1));
        }
        printf("\n");
      }
      
      for (int j = 0; j < width; ++j) {
        if (border && j == 0)
        {
          printf("%02d ", (i + 1));
        }
        putchar(toupper(board[i][j]));
        printf("  ");
      }
      printf("\n");
    }
  }

  int addWord(const Word& word)
  {
    int x = word.start.first;
    int y = word.start.second;
    for (int i = 0; i < strlen(word.word); ++i)
    {
      if (x < 0 || y < 0 || x >= width || y >= height) return 1;
      board[y][x] = word.word[i];
      if (E_Across == word.dir) x++;
      else if (E_Down == word.dir) y++;
    }
    return 0;
  }
};

#endif // BOARD_H
