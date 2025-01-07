#include "pipe_networking.h"
#include <time.h>
#include <signal.h>

int main() {
  srand(time(NULL));

  int to_client;
  int from_client;

  signal(SIGPIPE, SIG_IGN);

  while (1) {
    from_client = server_setup();
	pid_t p = fork();
	if (p < 0) {
	  perror("fork fail");
	  exit(1);
	}
	else if (p == 0) {
	  to_client = server_handshake_half(&to_client, from_client);

      while (1) {
        int random_int = rand() % 101;
      
        if (write(to_client, &random_int, sizeof(random_int)) == -1) {
          close(to_client);
          close(from_client);
          exit(0);
        }

        sleep(1);
      }
	}
	else {
	  close(from_client);
	}
  }

  return 0;
}