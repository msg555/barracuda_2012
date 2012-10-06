#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#include "xmlrpc_methods.hpp"

using namespace std;

pair<FILE*, FILE*> exec_pipe(char *const args[]) {
  int pipe_a[2];
  int pipe_b[2];
  pipe(pipe_a);
  pipe(pipe_b);

  if(fork() == 0) {
    dup2(pipe_a[0], 0);
    dup2(pipe_b[1], 1);
    close(pipe_a[0]); close(pipe_a[1]);
    close(pipe_b[0]); close(pipe_b[1]);
    //close(2);
    execv(args[0], args);
    perror("execv");
    exit(1);
  }

  close(pipe_a[0]);
  close(pipe_b[1]);
  return make_pair(fdopen(pipe_a[1], "w"), fdopen(pipe_b[0], "r"));
}

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

int main(int argc, char** argv) {
  srand(time(NULL));

  int wins[2] = {0, 0};
  int ties = 0;
  for(int idx = 0; ; idx = 1 - idx) {
    for(int i = 0; i < 49; i++) {
      ORD[i] = i;
    }
    random_shuffle(ORD, ORD + 49);
    for(int i = 0; i < 49; i++) {
      A[ORD[i] / 7][ORD[i] % 7] = i;
    }
    memset(C, -1, sizeof(C));

    int credits[2];
    credits[0] = 98;
    credits[1] = 98;

    pair<FILE*, FILE*> p[2];
    for(int i = 0; i < 2; i++) {
      char* args[2];
      args[0] = argv[(i ^ idx) + 1];
      args[1] = NULL;
      p[i] = exec_pipe(args);

      fprintf(p[i].first, "%d\n", i);
      for(int j = 0; j < 7; j++) {
        for(int k = 0; k < 7; k++) {
          if(k) fprintf(p[i].first, " ");
          fprintf(p[i].first, "%d", A[j][k]);
        }
        fprintf(p[i].first, "\n");
      }
      fflush(p[i].first);
    }

    int r;
    int tie_breaker = 0;
    int total_rounds = 0;
    bool is_tie = false;
    for(int i = 0; total_rounds < 1024 &&
                    !(r = check_winner()); i = (i + 1) % 7, total_rounds++) {
      if(credits[0] + credits[1] == 0) {
        is_tie = true;
        break;
      }

      vector<int> order;
      for(int j = 0; j < 7; j++) {
        int v = ORD[i * 7 + j];
        if(C[v / 7][v % 7] == -1) {
          order.push_back(i * 7 + j);
        }
      }
      if(order.empty()) continue;

      for(int j = 0; j < 2; j++) {
        fprintf(p[j].first, "%zu\n", order.size());
        for(int k = 0; k < order.size(); k++) {
          if(k) fprintf(p[j].first, " ");
          fprintf(p[j].first, "%d", order[k]);
        }
        fprintf(p[j].first, "\n");
        fflush(p[j].first);
      }

      int bids[2];
      for(int j = 0; j < 2; j++) {
        fscanf(p[j].second, "%d", bids + j);
        if(bids[j] < 0 || bids[j] > credits[j]) {
          fprintf(stderr, "Bot %d gave bad bid (%d/%d)\n", j ^ idx, bids[j],
                  credits[j]);
          return 1;
        }
      }
      if(bids[0] == 0 && bids[1] == 0) {
        fprintf(p[0].first, "-1\n");
        fprintf(p[1].first, "-1\n");
        continue;
      }

      int winner = bids[0] > bids[1] ||
                   (bids[0] == bids[1] && (++tie_breaker & 1)) ? 0 : 1;
      credits[winner] -= bids[winner];

      int choice;
      fprintf(p[winner].first, "1\n");
      fflush(p[winner].first);
      fscanf(p[winner].second, "%d", &choice);

      if(find(order.begin(), order.end(), choice) == order.end()) {
        fprintf(stderr, "Bot %d gave bad choice\n", winner ^ idx);
        return 1;
      }

      fprintf(p[1 - winner].first, "0\n%d\n", choice);

      C[ORD[choice] / 7][ORD[choice] % 7] = winner;
    }
    if(is_tie) {
      ties++;
    } else {
      wins[(r - 1) ^ idx]++;
    }

    printf("\rWINS %5d %5d %5d", wins[0], wins[1], ties);
    fflush(stdout);

    for(int i = 0; i < 2; i++) {
      fclose(p[i].first);
      fclose(p[i].second);
    }
  }
  return 0;
}

