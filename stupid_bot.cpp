#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "protocol_common.h"

#include "FindPath.h"

using namespace std;

static int idx;
static int credits = 98;
static int A[7][7];
static int C[7][7];
static int ORD[50];

static bool vis[7][7];

static int dr[] = {-1, 0, 1, 0, 1, 1, -1, -1};
static int dc[] = {0, 1, 0, -1, 1, -1, 1, -1};

static bool dfs(int r, int c, int col) {
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

static int check_winner() {
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

      vector<vector<int> > board(7, vector<int>(7, 0));
      for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 7; j++) {
          if(C[i][j] == -1) {
            board[i][j] = A[i][j];
          } else {
            board[i][j] = C[i][j] == idx ? -2 : -1;
          }
        }
      }

      pair<int, int> res(128, offer[0]);
      for(int i = 0; i < offer.size(); i++) {
        int v = ORD[offer[i]];
        board[v / 7][v % 7] = -2;
        FindPath path(board, 1 - idx, false);
        res = min(res, make_pair(path.Search(), offer[i]));
        board[v / 7][v % 7] = A[v / 7][v % 7];
      }

      cout << res.second << endl;
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
