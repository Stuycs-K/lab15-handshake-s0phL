#include "pipe_networking.h"

int main() {
  int to_server;
  int from_server = client_handshake(&to_server);

  while (1){
    int server_number;
    if (read(from_server, &server_number, sizeof(server_number)) == 0) {
      close(to_server);
      close(from_server);
      exit(1);
    }
    printf("%d\n", server_number);
  }
}

