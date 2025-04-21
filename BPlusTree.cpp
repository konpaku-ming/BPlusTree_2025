#include "BPlusTree.h"
using std::cin;
using std::cout;

bool operator==(const Data &x, const Data &y) {
  return strcmp(x.key, y.key) == 0 && x.value == y.value;
}

bool operator<(const Data &x, const Data &y) {
  const int cmp = strcmp(x.key, y.key);
  if (cmp < 0)return true;
  if (cmp > 0)return false;
  return x.value < y.value;
}

bool operator<=(const Data &x, const Data &y) {
  return x < y || x == y;
}

bool operator>(const Data &x, const Data &y) {
  const int cmp = strcmp(x.key, y.key);
  if (cmp > 0)return true;
  if (cmp < 0)return false;
  return x.value > y.value;
}

bool operator>=(const Data &x, const Data &y) {
  return x > y || x == y;
}
