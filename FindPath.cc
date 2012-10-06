/*
  Given a board with your moves and opponent's moves, find how many more squares are needed to connect. 
*/
#include <iostream>
#include <vector>

using namespace std;

class FindPath {
 public:
  // -1 means your opponent, -2 means you, other numbers mean the square number
  // direction 1 mean horrizontal, 0 means vertical
  FindPath(vector<vector<int> > board, int direction) {
    b_size_ = board.size();
    for (int i = 0; i < board.size(); ++i)
      for (int j = 0; j < board.size(); ++j)
        board_[i][j] = board[i][j];

    min_path_num_ = 50; 
    direction_ = direction;
  }

  // return the length of the shortest path
  int Search() {
    if (direction_ == 1) {
      for (int i = 0; i < b_size_; ++i) {
        if ( board_[i][0] >= 0)  {
          vector<pair<int, int> > c_move;
          int tmp = board_[i][0];
          board_[i][0] = -2;
          c_move.push_back(make_pair(i, 0));
          RSearch(i, 0, c_move);
          board_[i][0] = tmp; 
        }
      }
      // output
      cout << "shortest path num: " << min_path_num_ << endl;
      cout << "shortest path: " << endl;
      for (int i = 0; i < best_move_.size(); ++i) 
        cout << best_move_[i].first << " " << best_move_[i].second << " ";
      cout << endl;
    } 
    return min_path_num_;
  }

  // (x,y) already has been filled. Recursive search next
  void RSearch(int x, int y, vector<pair<int,int> >& c_move) {
    int c_len = c_move.size();
    if (c_len > min_path_num_)
      return;
    if (y == b_size_ - 1) {
      // find the path
      if (c_len < min_path_num_) {
        min_path_num_ = c_len;
        best_move_.clear();
        for (int i = 0; i < c_move.size(); ++i)
          best_move_[i] = c_move[i];
      }
      return;
    }
    // search other direction
    for (int i = 0; i < 8; ++i) {
      int nx = x + dir_x[i];
      int ny = y + dir_y[i];
      if (nx >= 0 && nx <= b_size_ && ny >=0 && ny <= b_size_ && board_[nx][ny] >= 0) {
        int tmp = board_[nx][ny];
        board_[nx][ny] = -2;
        c_move.push_back(make_pair(nx, ny));
        RSearch(nx, ny, c_move);
        board_[nx][ny] = tmp;
        c_move.pop_back();
      }
    }     
  }

 private:

  vector<vector<int> > board_;
  int min_path_num_;
  int direction_;
  vector<pair<int, int> > best_move_;
  int b_size_;
  static const int dir_x[8];
  static const int dir_y[8]; 

};

const int FindPath::dir_x[8] = {-1, -1, 0, 1, 1, 1,   0, -1};
const int FindPath::dir_y[8] = {0,   1, 1, 1, 0, -1, -1, -1};


