#include <cstring>
#include <iomanip>
#include <iostream>

void Deposit(int& n) {
  int k;
  std::cin >> k;
  if (k > 0) {
    n += k;
  } else {
    std::cout << "error" << "\n";
  }
}

void Withdraw(int& n) {
  int k;
  std::cin >> k;
  if (k > 0 && k <= n) {
    n -= k;
  } else {
    std::cout << "error" << "\n";
  }
}

void Balance(int& n) {
  std::cout << n << "\n";
}

void Reset(int& n) {
  n = 0;
}

void Inc(int& n) {
  n++;
}

struct Command {
  const char* name;
  void (*func)(int&);
};

Command table[] = {
  {"deposit", Deposit},
  {"withdraw", Withdraw},
  {"balance", Balance},
  {"reset", Reset},
  {"inc", Inc}
};

int main() {
  int n = 0;
  char s[32];
  int c = sizeof(table) / sizeof(table[0]);

  while (std::cin >> std::setw(sizeof(s)) >> s && strcmp(s, "stop") != 0) {
    bool found = false;
    for (int i = 0; i < с; i++) {
      if (strcmp(table[i].name, s) == 0) {
        table[i].func(n);
        found = true;
        break;
      }
    }
    if (not(found)) {
      std::cout << "error" << "\n";
    }
  }
}