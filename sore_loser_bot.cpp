#include <iostream>

#include "protocol_common.h"

using namespace std;

int A[7][7];

int main() {
  int idx; cin >> idx; 
  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 7; j++) {
      cin >> A[i][j];
    }
  }
  for(;;) {
    vector<int> offer = get_vector();
    if(offer.empty()) break;

    cout << 98 << endl;
    int win; cin >> win;
    if(win == 1) {
      cout << -1 << endl;
    }
  }
}
