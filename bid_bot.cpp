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

pair<int, pair<int, int> > best;
int PICKS[7];
int BPICKS[7];
int rot;

bool vis[7][7];

int dr[] = {1, 1, 1, 0, -1, -1, -1, 0};
int dc[] = {-1, 0, 1, 1, 1, 0, -1, -1};

void dfs(int r, int c) {
  if(r < 0 || r >= 7 || c < 0 || c >= 7 || vis[r][c] ||
     !(C[r][c] == -2 || C[r][c] == 0)) return;
  vis[r][c] = true;
  for(int i = 0; i < 8; i++) {
    dfs(r + dr[i], c + dc[i]);
  }
}

int score_path() {
  int pos = 0;
  int res = 0;
  for(int j = 0; j < 7; j++) {
    int b = (j + 7 - rot) % 7;
    if(PICKS[b] == -1) continue;
    int v = ORD[PICKS[b]];
    C[v / 7][v % 7] = -2;

    int comps = 0;
    memset(vis, 0, sizeof(vis));
    for(int i = 0; i < 7; i++) {
      for(int j = 0; j < 7; j++) {
        if(!(C[i][j] == -2 || C[i][j] == 0) || vis[i][j]) continue;
        dfs(i, j);
        comps++;
      }
    }
    res += (4 - pos++) * comps;
    if(pos == 4) break;
  }

  for(int j = 0; j < 7; j++) {
    int b = (j + 7 - rot) % 7;
    if(PICKS[b] == -1) continue;
    int v = ORD[PICKS[b]];
    C[v / 7][v % 7] = -1;
  }
  return res;
}

void find_picks(int r, int c, int m, int sumcomp) {
  bool setpick = false;
  if(C[r][c] == 1) return;
  if(C[r][c] == -1) {
    int j;
    int om = m;
    for(j = (rot + A[r][c] / 7) % 7; j < 30; j += 7) {
      if(m & 1 << j) continue;
      m |= 1 << j;
      break;
    }
    if(m == om) return;

    if(j < 7) {
      setpick = true;
      PICKS[A[r][c] / 7] = A[r][c];
    }
  }

  pair<int, pair<int, int> > val(__builtin_popcount(m), make_pair(m, 0));
  if(val >= best) {
    if(setpick) { PICKS[A[r][c] / 7] = -1; C[r][c] = -1; }
    return;
  }
  
  if(c == 6) {
    val.second.second = score_path();
    if(val < best) {
      best = val;
      memcpy(BPICKS, PICKS, sizeof(PICKS));
    }
    if(setpick) { PICKS[A[r][c] / 7] = -1; C[r][c] = -1; }
    return;
  }

  for(int dr = -1; dr <= 1; dr++) {
    int nr = r + dr;
    if(nr < 0 || nr >= 7) continue;
    find_picks(nr, c + 1, m, sumcomp);
  }

  if(setpick) { PICKS[A[r][c] / 7] = -1; C[r][c] = -1; }
}

void transpose() {
  for(int i = 0; i < 7; i++) {
    for(int j = i; j < 7; j++) {
      swap(A[i][j], A[j][i]);
      swap(C[i][j], C[j][i]);
    }
  }
  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 7; j++) {
      ORD[A[i][j]] = i * 7 + j;
      if(C[i][j] >= 0) C[i][j] = 1 - C[i][j];
    }
  }
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

  memset(C, -1, sizeof(C));
  for(int round = 0; ; round++) {
    vector<int> offer = get_vector();
    sort(offer.begin(), offer.end());
    if(offer.empty()) break;

    bool opwant = false;
    int opneed = 0;
    int opchoice = -1;
    for(int i = 0; i < 2; i++) {
      transpose();
      rot = 7 - round % 7;
      best = make_pair(128, make_pair(0, 0));
      memset(PICKS, -1, sizeof(PICKS));
      BPICKS[round % 7] = offer[0];
      for(int r = 0; r < 7; r++) {
        find_picks(r, 0, 0, 0);
      }
      if(i == 0) {
        opwant = BPICKS[round % 7] != -1;
        opneed = best.first;
        opchoice = BPICKS[round % 7];
      }
    }

    bool mewant = BPICKS[round % 7] != -1;

    int bid = credits / best.first;
    if(!opwant) {
      bid = (int)(bid * 0.5);
    } else if(round < 3) {
      bid = 8;
    }
    if(opwant && opneed == 1) {
      bid = min((int)(0.2 + credits * .8), (int)(bid * 1.5));
      if(!mewant) BPICKS[round % 7] = opchoice;
    }
    if(!mewant && !opwant) {
      bid = 1;
      BPICKS[round % 7] = offer[rand() % offer.size()];
    }
    
    if(bid == 0) bid = 1;
    if(BPICKS[round % 7] == -1) bid = 0;
    if(bid > credits) bid = credits;
    cout << bid << endl;

/*
    int bid = credits / best.first;
    if(bid == 0) bid = 1;
    if(round < 4) {
      bid = 8;//5 + round * 2;
    } else if(!opwant && !mewant) {
      //bid = 1;
    } else if(opwant && !mewant) {
      if(opneed == 1) bid = (int)(1.5 * bid);
      if(opneed == 2) bid = bid;
      if(opneed == 3) bid = (int)(0.8 * bid);
      if(opneed > 3) bid = 1 + rand() % 3;
    } else if(!opwant && mewant) {
      if(best.first >= 2) {
        bid = 1 + bid / 2 + rand() % ((bid + 1) / 2);
      }
    }
    if(bid == 0) bid = 1;
    if(bid > credits) bid = credits;
    cout << bid << endl;
*/

    int win; cin >> win;
    if(win == -1) {
      /* Nobody won :( */
      continue;
    }
    if(win) {
      credits -= bid;
      int choice = BPICKS[round % 7];
      if(choice == -1) choice = opchoice;
      if(choice == -1) choice = offer[rand() % offer.size()];
      int r = ORD[choice] / 7;
      int c = ORD[choice] % 7;
      C[r][c] = 0;
      cout << choice << endl;
    } else {
      int choice; cin >> choice;
      int r = ORD[choice] / 7;
      int c = ORD[choice] % 7;
      C[r][c] = 1;
    }
  }
  return 0;
}
