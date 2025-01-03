#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;
  int to_server;
  int from_server;

  int f = fork();
  if (f < 0) {
    perror("fork fail");
    exit(1);
  }
  else if (f == 0) {
    from_server = client_handshake(&to_server);
    write(from_server, "\n", 1);

  }
  else {
    from_client = server_handshake(&to_client);
    server_setup();
    char line[100];
    read(from_client, line, sizeof(line));

  }


}
