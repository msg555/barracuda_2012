#include <vector>


class FindPath {
 public:
  // -1 means your opponent, -2 means you, other numbers mean the square number
  // direction 1 mean horrizontal, 0 means vertical
  FindPath(std::vector<std::vector<int> > board, int direction, bool out); 
  // return the length of the shortest path
  int Search(); 
  // (x,y) already has been filled. Recursive search next
  void RSearch(int x, int y, std::vector<std::pair<int,int> >& c_move); 

 private:

  std::vector<std::vector<int> > board_, visited_;
  int min_square_num_;
  int direction_;
  std::vector<std::pair<int, int> > best_move_;
  int b_size_;
  bool out_;
  static const int dir_x[8];
  static const int dir_y[8]; 
};
