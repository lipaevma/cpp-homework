#include <iomanip>
#include <iostream>

struct MinMaxResult {
  int mn;
  int mx;
};

double SrTem(const int* a, const int& n_int) {
  const double n = n_int;
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += a[i];
  }
  double result = sum / n;
  return result;
}

MinMaxResult MinMax(const int* a, const int& n) {
  MinMaxResult result;
  result.mn = a[0];
  result.mx = a[0];
  for (int i = 0; i < n; i++) {
    if (a[i] < result.mn) {
      result.mn = a[i];
    }
    if (a[i] > result.mx) {
      result.mx = a[i];
    }
  }
  return result;
}

int CountFrost(const int* a, const int& n) {
  int frostdays = 0;
  for (int i = 0; i < n; i++) {
    if (a[i] < 0) {
      frostdays++;
    }
  }
  return frostdays;
}

int main() {
  int n;
  std::cin >> n;
  int* a = new int[n];
  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  MinMaxResult minmax = MinMax(a, n);
  std::cout << std::fixed << std::setprecision(1) << SrTem(a, n) << "\n";
  std::cout << minmax.mn << " " << minmax.mx << "\n";
  std::cout << CountFrost(a, n) << "\n";
  delete[] a;
}