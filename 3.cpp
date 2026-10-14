#include <cstring>
#include <iomanip>
#include <iostream>

const int cRows = 5;
const int cCols = 10;

bool Bord(int x, int y) {
  return (x >= 0 && x < cCols && y >= 0 && y < cRows);
}

bool Clear(char (&board)[cRows][cCols]) {
  for (int y = 0; y < cRows; y++) {
    for (int x = 0; x < cCols; x++) {
      board[y][x] = ' ';
    }
  }
  return true;
}

bool Put(char (&board)[cRows][cCols]) {
  int x, y;
  char c;
  std::cin >> x >> y >> c;
  if (not(Bord(x, y))) {
    return false;
  }
  board[y][x] = c;
  return true;
}

bool Text(char (&board)[cRows][cCols]) {
  int x, y;
  char word[cCols + 1];
  std::cin >> x >> y >> std::setw(cCols + 1) >> word;
  int len = strlen(word);
  if (not(Bord(x, y)) || not(Bord(x + len - 1, y))) {
    return false;
  }
  for (int i = 0; i < len; i++) {
    board[y][x + i] = word[i];
  }
  return true;
}

bool Print(char (&board)[cRows][cCols]) {
  for (int y = 0; y < cRows; y++) {
    for (int x = 0; x < cCols; x++) {
      std::cout << board[y][x];
    }
    std::cout << "\n";
  }
  return true;
}

struct Command {
  const char* name;
  bool (*func)(char (&)[cRows][cCols]);
};

Command table[] = {
  {"clear", Clear},
  {"put", Put},
  {"text", Text},
  {"print", Print}
};

int main() {
  char board[cRows][cCols];
  Clear(board);

  char s[32];
  while (std::cin >> std::setw(sizeof(s)) >> s && strcmp(s, "quit") != 0) {
    for (int i = 0; i < 4; i++) {
      if (strcmp(table[i].name, s) == 0) {
        if (not(table[i].func(board))) {
          std::cout << "error" << "\n";
        }
        break;
      }
    }
  }
}