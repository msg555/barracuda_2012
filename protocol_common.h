#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>

std::vector<int> get_vector() {
  size_t sz;
  std::vector<int> r;
  if(EOF == scanf("%zu", &sz)) {
    return r;
  }
  if(sz > 100) {
    fprintf(stderr, "Unexpectedly large vector size\n");
    exit(0);
  }
  for(int i = 0; i < sz; i++) {
    int x;
    scanf("%d", &x);
    r.push_back(x);
  }
  return r;
}
