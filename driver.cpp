#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#include "xmlrpc_methods.hpp"

using namespace std;

const int DEFAULT_PORT = 5150;

char* args[16] = {NULL};
FILE* fin = NULL;
FILE* fout = NULL;

pair<FILE*, FILE*> exec_pipe(char *const args[]) {
  int pipe_a[2];
  int pipe_b[2];
  pipe(pipe_a);
  pipe(pipe_b);

  pid_t npid = fork();
  if(npid == 0) {
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

void PingMethod::execute(const xmlrpc_c::paramList& paramList,
                         xmlrpc_c::value* const retval) {
  *retval = xmlrpc_c::value_boolean(true);
}

void InitGameMethod::execute(const xmlrpc_c::paramList& paramList,
                             xmlrpc_c::value* const retval) {
  if(fout) {
    fprintf(fout, "0\n");
    fflush(fout);
    fclose(fin); fclose(fout);
  }
  pair<FILE*, FILE*> pipe = exec_pipe(args);
  fout = pipe.first;
  fin = pipe.second;

  map<string, xmlrpc_c::value> gstate = paramList.getStruct(0);

  fprintf(fout, "%d\n", (int)xmlrpc_c::value_int(gstate["idx"]));

  vector<xmlrpc_c::value> unprocessed_board =
      xmlrpc_c::value_array(gstate["board"]).vectorValueValue();
  for (vector<xmlrpc_c::value>::iterator i = unprocessed_board.begin();
       i != unprocessed_board.end(); ++i) {
    bool first = false;
    const vector<xmlrpc_c::value>& row =
        xmlrpc_c::value_array(*i).vectorValueValue();
    for (vector<xmlrpc_c::value>::const_iterator j = row.begin();
         j != row.end(); ++j) {
      if(!first) fprintf(fout, " ");
      fprintf(fout, "%d", (int)xmlrpc_c::value_int(*j));
      first = false;
    }
    fprintf(fout, "\n");
  }
  fflush(fout);

  *retval = xmlrpc_c::value_boolean(true);
}

void GetBidMethod::execute(const xmlrpc_c::paramList& paramList,
                           xmlrpc_c::value* const retval) {
  if(!fin || !fout) return;

  vector<xmlrpc_c::value> offer = paramList.getArray(0);
  map<string, xmlrpc_c::value> gstate = paramList.getStruct(1);

  bool first = true;
  fprintf(fout, "%zu\n", offer.size());
  for (vector<xmlrpc_c::value>::iterator i = offer.begin();
       i != offer.end(); ++i) {
    if(!first) fprintf(fout, " ");
    fprintf(fout, "%d", (int)xmlrpc_c::value_int(*i));
    first = false;
  }
  fprintf(fout, "\n");
  fflush(fout);

  int bid;
  fscanf(fin, "%d", &bid);
  *retval = xmlrpc_c::value_int(bid);
}

void MakeChoiceMethod::execute(const xmlrpc_c::paramList& paramList,
                               xmlrpc_c::value* const retval) {
  if(!fin || !fout) return;

  vector<xmlrpc_c::value> offer = paramList.getArray(0);

  fprintf(fout, "1\n");
  fflush(fout);

  int choice;
  fscanf(fin, "%d", &choice);
  
  bool ok = false;
  for (vector<xmlrpc_c::value>::iterator i = offer.begin();
       i != offer.end(); ++i) {
    ok |= (int)xmlrpc_c::value_int(*i) == choice;
  }
  if(!ok) {
    fprintf(stderr, "Warning: invalid make choice result\n");
    fflush(stderr);
  }

  *retval = xmlrpc_c::value_int(choice);
}

void MoveResultMethod::execute(const xmlrpc_c::paramList& paramList,
                               xmlrpc_c::value* const retval) {
  if(!fin || !fout) return;

  map<string, xmlrpc_c::value> result = paramList.getStruct(0);

  string result_string = xmlrpc_c::value_string(result["result"]);
  if (result_string == "you_chose") {
    /* Nothing to do. */
  } else if (result_string == "opponent_chose") {
    fprintf(fout, "0\n%d\n", (int)xmlrpc_c::value_int(result["choice"]));
    fflush(fout);
  } else {
    fprintf(fout, "-1\n");
    fflush(fout);
  }

  *retval = xmlrpc_c::value_boolean(true);
}

void GameResultMethod::execute(const xmlrpc_c::paramList& paramList,
                               xmlrpc_c::value* const retval) {
  if(!fin || !fout) return;

  map<string, xmlrpc_c::value> result = paramList.getStruct(0);

  string winner = xmlrpc_c::value_string(result["winner"]);
  printf("GAME RESULT %s\n", winner.c_str());
  fflush(stdout);

  fprintf(fout, "0\n");
  fflush(fout);
  fclose(fin); fclose(fout);
  fin = fout = NULL;
}

int main(int argc, char* argv[]) {
  struct sigaction act;
  act.sa_handler=SIG_IGN;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGPIPE, &act, NULL); 

  args[0] = argv[1];
  args[1] = NULL;

  int port = DEFAULT_PORT;
/*
  if (argc > 1) {
      port = atoi(argv[1]);
  }
*/

  xmlrpc_c::registry registry;
  xmlrpc_c::methodPtr const ping_method(new PingMethod());
  xmlrpc_c::methodPtr const init_game_method(new InitGameMethod);
  xmlrpc_c::methodPtr const get_bid_method(new GetBidMethod);
  xmlrpc_c::methodPtr const make_choice_method(new MakeChoiceMethod);
  xmlrpc_c::methodPtr const move_result_method(new MoveResultMethod);
  xmlrpc_c::methodPtr const game_result_method(new GameResultMethod);

  registry.addMethod("ping", ping_method);
  registry.addMethod("init_game", init_game_method);
  registry.addMethod("get_bid", get_bid_method);
  registry.addMethod("make_choice", make_choice_method);
  registry.addMethod("move_result", move_result_method);
  registry.addMethod("game_result", game_result_method);

  xmlrpc_c::serverAbyss server(xmlrpc_c::serverAbyss::constrOpt().registryP(&registry).portNumber(port));

  std::cout << "T-1000: Starting on " << port << ". Kill all robots." << std::endl;

  for(;;) {
    server.runOnce();
  }

  return 0;
}
