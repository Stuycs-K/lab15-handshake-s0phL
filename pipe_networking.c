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
  int from_client = 0;
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
  mkfifo(WKP, 0666);
  *to_client = open(WKP, O_WRONLY);
  if (*to_client == -1) {
    perror("Failed to open FIFO for writing");
    exit(1);
  }

  char message[100];
  sprintf(message, "%d", SYN);
  write(*to_client, message, sizeof(message));
  int from_client = open(WKP, O_RDONLY);
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char pid[100];
  sprintf(pid, "%d", getpid());
  mkfifo(pid, 0666);
  *to_server = open(pid, O_WRONLY);
  if (*to_server == -1) {
    perror("Failed to open FIFO for writing");
    exit(1);
  }
  char message[100];
  sprintf(message, "%d", SYN);
  write(*to_server, message, sizeof(message));

  int from_server = open(WKP, O_RDONLY);
  if (from_server == -1) {
    perror("Failed to open from server FIFO for reading");
    exit(1);
  }
  char response[100];
  read(from_server, response, sizeof(response));
  response; //++

  write(*to_server, response, sizeof(response));

  char pid[100];
  sprintf(pid, "%d", getpid());
  mkfifo(pid, 0666);
  *to_server = open(pid, O_WRONLY);
  char message[100];
  sprintf(message, "%d", SYN_ACK);
  write(*to_server, message, sizeof(message));
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
