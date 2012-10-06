#include "FindPath.h"
#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

void test_2_by_2() {
  vector<vector<int > > a;
  int n = 2;
  for (int i = 0; i < n; ++i) { 
    vector<int> s;
    for (int j = 0; j < n; ++j)
      s.push_back(0);
    a.push_back(s);
  }
  a[0][0] = -2;
  a[1][1] = -1;
  FindPath foo(a, 1, true);
  assert(foo.Search() == 1);
}  

void test_3_by_3() {
  vector<vector<int > > a;
  int n = 3;
  for (int i = 0; i < n; ++i) { 
    vector<int> s;
    for (int j = 0; j < n; ++j)
      s.push_back(0);
    a.push_back(s);
  }
  a[0][0] = -2;
  a[0][1] = -1;
  a[1][1] = -1;
  a[2][2] = -1;
  a[1][2] = -2;
  FindPath foo(a, 1, true);
  assert(foo.Search() == 2);
}

void test_3_by_3_vertical() {
  vector<vector<int > > a;
  int n = 3;
  for (int i = 0; i < n; ++i) { 
    vector<int> s;
    for (int j = 0; j < n; ++j)
      s.push_back(0);
    a.push_back(s);
  }
  a[0][0] = -2;
  a[0][1] = -1;
  a[0][2] = -2;
  a[1][1] = -1;
  a[2][2] = -1;
  a[1][2] = -2;
  FindPath foo(a, 0, true);
  assert(foo.Search() == 1);
}

void test_4_by_4() {
  vector<vector<int > > a;
  int n = 4;
  for (int i = 0; i < n; ++i) { 
    vector<int> s;
    for (int j = 0; j < n; ++j)
      s.push_back(0);
    a.push_back(s);
  }
  a[0][1] = -2;
  a[0][2] = -1;
  a[1][0] = -1;
  a[1][2] = -1;
  a[1][3] = -2;
  a[2][0] = -1;
  a[2][1] = -2;
  a[2][2] = -1;
  a[3][0] = -1;
  a[3][3] = -1;
  FindPath foo(a, 1, true);
  assert(foo.Search() == 4);
}

void test_4_by_4_vertical() {
  vector<vector<int > > a;
  int n = 4;
  for (int i = 0; i < n; ++i) { 
    vector<int> s;
    for (int j = 0; j < n; ++j)
      s.push_back(0);
    a.push_back(s);
  }
  a[0][1] = -2;
  a[0][2] = -1;
  a[1][0] = -1;
  a[1][2] = -1;
  a[1][3] = -2;
  a[2][0] = -1;
  a[2][1] = -2;
  a[2][2] = -1;
  a[3][0] = -1;
  a[3][3] = -1;
  FindPath foo(a, 0, true);
  assert(foo.Search() == 2);
}

int main() {
  
  test_2_by_2();
  test_3_by_3();
  test_4_by_4();
  
  test_3_by_3_vertical();
  test_4_by_4_vertical();
  return -1;
}
