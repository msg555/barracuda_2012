#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include "protocol_common.h"

using namespace std;

int idx;
int credits = 98;
int A[7][7];
int C[7][7];
int ORD[50];

int PICKS[7];

bool find_picks(int r, int c, int m) {
  PICKS[A[r][c] / 7] = A[r][c] % 7;
  if(c == 6) return true;
  for(int dr = -1; dr <= 1; dr++) {
    int nr = r + dr;
    if(nr < 0 || nr >= 7) continue;

    int b = A[nr][c + 1] / 7;
    if(m & 1 << b) continue;

    if(find_picks(nr, c + 1, m | 1 << b)) return true;
  }
  return false;
}

int main() {
  cin >> idx;
  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 7; j++) {
      cin >> (idx ? A[j][i] : A[i][j]);
    }
  }
  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 7; j++) {
      ORD[A[i][j]] = i * 7 + j;
    }
  }


  bool found_picks = false;
  for(int i = 0; i < 7; i++) PICKS[i] = 0;
  for(int i = 0; i < 7; i++) {
    if(find_picks(i, 0, 1 << (A[i][0] / 7))) {
      found_picks = true;
      break;
    }
  }

  bool marked_win = false;
  memset(C, -1, sizeof(C));
  for(int round = 0; ; round++) {
    vector<int> offer = get_vector();
    sort(offer.begin(), offer.end());
    if(offer.empty()) break;

    int bid = min(14, credits);
    if(PICKS[round % 7] == -1) bid = 0;
    cout << bid << endl;

    int win; cin >> win;
    if(win == -1) {
      /* Nobody won :( */
      continue;
    }
    if(win) {
      credits -= bid;
      int choice = offer[PICKS[round % 7] % offer.size()];
      int r = ORD[choice] / 7;
      int c = ORD[choice] % 7;
      C[r][c] = 0;
      cout << choice << endl;
    } else {
      int choice; cin >> choice;
      int r = ORD[choice] / 7;
      int c = ORD[choice] % 7;
      C[r][c] = 1 - idx;
    }
  }
  return 0;
}
