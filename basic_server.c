#include "pipe_networking.h"
#include <time.h>

int main() {
  srand(time(NULL));

  int to_client;
  int from_client = server_handshake(&to_client);

  while (1) {
    int random_int = rand() % 101;
    write(to_client, &random_int, sizeof(random_int));
    sleep(1);
  }

  return 0;
}
