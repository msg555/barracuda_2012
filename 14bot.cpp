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

bool vis[7][7];

int dr[] = {-1, 0, 1, 0, 1, 1, -1, -1};
int dc[] = {0, 1, 0, -1, 1, -1, 1, -1};

bool dfs(int r, int c, int col) {
  if(r < 0 || r >= 7 || c < 0 || c >= 7 ||
     vis[r][c] || C[r][c] != col) return false;
  vis[r][c] = true;
  for(int i = 0; i < 8; i++) {
    if(dfs(r + dr[i], c + dc[i], col)) {
      return true;
    }
  }
  return false;
}

int check_winner() {
  memset(vis, 0, sizeof(vis));
  for(int i = 0; i < 7; i++) {
    dfs(i, 0, 0);
    dfs(0, i, 1);
  }
  int res = 0;
  for(int i = 0; i < 7; i++) {
    if(C[i][6] == 0 && vis[i][6]) res |= 1;
    if(C[6][i] == 1 && vis[6][i]) res |= 2;
  }
  if(res == 3) cerr << "TWO WINNERS?" << endl;
  return res;
}

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
  /* Initialize game state. */
  cin >> idx;

  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 7; j++) {
      cin >> (idx ? A[j][i] : A[i][j]);
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
  for(int round = 0; credits > 0; round++) {
    vector<int> offer = get_vector();
    sort(offer.begin(), offer.end());
    if(offer.empty()) break;

    int bid = min(14, credits);
    cout << bid << endl;

    int win; cin >> win;
    if(win == -1) {
      /* Nobody won :( */
      fprintf(stderr, "Nobody won round :(\n");
      continue;
    }
    if(win) {
      credits -= bid;
      cout << offer[PICKS[round % 7] % offer.size()] << endl;
      int r = ORD[offer[0]] / 7;
      int c = ORD[offer[0]] % 7;
      C[r][c] = idx;
    } else {
      int choice; cin >> choice;
      int r = ORD[choice] / 7;
      int c = ORD[choice] % 7;
      C[r][c] = 1 - idx;
    }

    int r = check_winner();
    if(r) {
      r--;
      if(marked_win) {
        //cerr << "WIN AGAIN? " << idx << " " << r << endl;
      }
      //cerr << (idx == r ? "WIN" : "LOSE") << endl;
      marked_win = true;
      break;
    }
  }
  return 0;
}
