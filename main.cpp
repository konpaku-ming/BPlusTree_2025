#include <iostream>
#include "BPlusTree.h";
using std::cin;
using std::string;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  BPT bpt;
  //freopen("test.in", "r",stdin);
  //freopen("test.out", "w",stdout);
  int n;
  cin >> n;
  for (int i = 0; i < n; i++) {
    string cmd;
    cin >> cmd;
    if (cmd == "insert") {
      Data dt;
      cin >> dt.key;
      cin >> dt.value;
      bpt.Insert(dt);
    }
    if (cmd == "find") {
      char key[70];
      cin >> key;
      bpt.Find(key);
    }
    if (cmd == "delete") {
      Data dt;
      cin >> dt.key;
      cin >> dt.value;
      bpt.Remove(dt);
    }
  }
  return 0;
}
