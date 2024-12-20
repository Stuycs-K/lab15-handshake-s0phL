#include <stdlib.h>
#include <time.h>
#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  mkfifo(WKP, 0666);
  int from_client = open(WKP, O_RDONLY);
  remove(WKP);
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  char client_pid[100];
  int from_client = open(WKP, O_RDONLY);
  read(from_client, client_pid, sizeof(client_pid));

  *to_client = open(client_pid, O_WRONLY);

  srand(time(NULL));
  int random_int = rand();
  write(*to_client, &random_int, sizeof(random_int));

  int ack;
  read(from_client, &ack, sizeof(ack));

  if (ack == random_int + 1) {
      return from_client;
  } else {
      printf("Handshake failed: Invalid ACK\n");
      return -1;
  }
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char client_pid[100];
  sprintf(client_pid, "%d", getpid());
  mkfifo(client_pid, 0666);

  *to_server = open(client_pid, O_WRONLY);
  write(*to_server, client_pid, sizeof(client_pid));

  int server_random_int;
  int from_server = open(client_pid, O_RDONLY);
  read(from_server, &server_random_int, sizeof(server_random_int));

  int ack = server_random_int + 1;
  write(*to_server, &ack, sizeof(ack));

  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  char client_pid[100];
  read(from_client, client_pid, sizeof(client_pid));

  int to_client = open(client_pid, O_WRONLY);
  const char *message = "Hello!";
  write(to_client, message, strlen(message) + 1);

  int ack;
  read(from_client, &ack, sizeof(ack));

  if (ack == 1) {
      printf("Handshake complete, communication established.\n");
      return to_client;
  } else {
      printf("Handshake failed: Invalid ACK\n");
      return -1;
  }
}
