#include <vector>
#include <utility>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
    execv(args[0], args);
    perror("execv");
    exit(1);
  }

  close(pipe_a[0]);
  close(pipe_b[1]);
  return make_pair(fdopen(pipe_a[1], "w"), fdopen(pipe_b[0], "r"));
}

int main(int argc, char** argv) {
  vector<pair<FILE*, FILE*> > pipes;
  for(size_t i = 1; i < argc; i++) {
    char* args[2];
    args[0] = argv[i];
    args[1] = NULL;
    pipes.push_back(exec_pipe(args));
  }
  int x = 555;
  for(int i = 0; i < pipes.size(); i++) {
    fprintf(pipes[i].first, "%d\n", x);
    fflush(pipes[i].first);
    fscanf(pipes[i].second, "%d", &x);

    printf("GOT %d\n", x);
    fflush(stdout);
  }
  return 0;
}
