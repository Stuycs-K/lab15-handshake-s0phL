#include "pipe_networking.h"
#include <time.h>
#include <signal.h>
// #include <fcntl.h>
// #include <unistd.h>

int main() {
  srand(time(NULL));

  int to_client;
  int from_client;

  signal(SIGPIPE, SIG_IGN);

  while (1) {
    from_client = server_handshake(&to_client);

    while (1) {
      int random_int = rand() % 101;
      
      if (write(to_client, &random_int, sizeof(random_int)) == -1) {
        close(to_client);
        close(from_client);
        break;
      }

      sleep(1);
    }
  }

  return 0;
}