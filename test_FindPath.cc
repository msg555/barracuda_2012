#include "find_path.cc"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

void test_2_by_2() {
  vector<vector<int > > a;
  vector<int> s;
  s.push_back(0); s.push_back(0);
  a.push_back(s);
  a.push_back(s);
  FindPath foo(a, -1);
  assert(foo.Search() == 2);
}  

int main() {
  test_2_by_2();
  return -1;
}
