/*
   Given a board with your moves and opponent's moves, find how many more squares are needed to connect. 
   */
#include <iostream>
#include "FindPath.h"
#include <vector>
#define MAX_PATH_LENGTH 50
using namespace std;

// -1 means your opponent, -2 means you, other numbers mean the square number
// direction 1 mean horrizontal, 0 means vertical
FindPath::FindPath(vector<vector<int> > board, int direction, bool out) {
  b_size_ = board.size();
  for (int i = 0; i < board.size(); ++i) {
    vector<int> a, b;
    for (int j = 0; j < board.size(); ++j) {
      a.push_back(board[i][j]);
      b.push_back(0);
    }
    board_.push_back(a);
    visited_.push_back(b);
  }
  min_square_num_ = MAX_PATH_LENGTH;
  direction_ = direction;
  out_ = out;
}

// return the minimum number of suqares to add to have a shortest path from one side to 
// another side. Will return -1 if couldn't find any path
int FindPath::Search() {
    for (int i = 0; i < b_size_; ++i) {
      int x, y;
      if (direction_ == 1) {
        // horrizontal
        x = i;
        y = 0;
      } else if (direction_ == 0) {
        // vertical
        x = 0;
        y = i;
      }
      if ( board_[x][y] >= 0 || board_[x][y] == -2)  {
        vector<pair<int, int> > c_move;
        visited_[x][y] = 1;
        c_move.push_back(make_pair(x, y));
        RSearch(x, y, c_move);
        visited_[x][y] = 0; 
      }
  }
  // output
  if (out_) {
    cout << "minimum suquares to add: " << min_square_num_ << endl;
    cout << "shortest path: " << endl;
    for (int i = 0; i < best_move_.size(); ++i) 
      cout << "(" << best_move_[i].first << "," << best_move_[i].second << ") ";
    cout << endl;
  }
  if (min_square_num_ == MAX_PATH_LENGTH)
    return -1;
  else
    return min_square_num_;
}

// (x,y) already has been filled. Recursive search next
void FindPath::RSearch(int x, int y, vector<pair<int,int> >& c_move) {
  // debug
  //cout << "x: " << x << " y: " << y << endl;
  int c_len = 0; 
  for (int i = 0; i < c_move.size(); ++i) {
    if (board_[c_move[i].first][c_move[i].second] != -2)
      c_len++;
  }
  if (c_len > min_square_num_)
    return;
  if ((direction_ == 1 && y == b_size_ - 1) || (direction_ == 0 && x == b_size_ - 1)) {
    // find the path, ie, reaching the other side
    if (c_len < min_square_num_) {
      min_square_num_ = c_len;
      best_move_.clear();
      for (int i = 0; i < c_move.size(); ++i)
        best_move_.push_back(c_move[i]);
    }
    return;
  }
  
  // search other direction
  for (int i = 0; i < 8; ++i) {
    int nx = x + dir_x[i];
    int ny = y + dir_y[i];
    if (nx >= 0 && nx < b_size_ && ny >=0 && ny < b_size_) { 
      if ((board_[nx][ny] >= 0 || board_[nx][ny] == -2) && visited_[nx][ny] == 0) {
        visited_[nx][ny] = 1;
        c_move.push_back(make_pair(nx, ny));
        RSearch(nx, ny, c_move);
        visited_[nx][ny] = 0;
        c_move.pop_back();
      }
    }
  }     
}

const int FindPath::dir_x[8] = {-1, -1, 0, 1, 1, 1,   0, -1};
const int FindPath::dir_y[8] = {0,   1, 1, 1, 0, -1, -1, -1};


