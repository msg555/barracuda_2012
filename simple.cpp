#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>

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

int main() {
  /* Initialize game state. */
  cin >> idx;

  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 7; j++) {
      cin >> A[i][j];
      ORD[A[i][j]] = i * 7 + j;
    }
  }

  bool marked_win = false;
  memset(C, -1, sizeof(C));
  for(; credits > 0; ) {
    vector<int> offer = get_vector();
    if(offer.empty()) break;

    int bid = min(4, credits);
    cout << bid << endl;

    int win; cin >> win;
    if(win == -1) {
      /* Nobody won :( */
      fprintf(stderr, "Nobody won round :(\n");
      continue;
    }
    if(win) {
      credits -= bid;
      cout << offer[0] << endl;
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
