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
  
  //int from_client = open(WKP, O_RDONLY);
  read(from_client, pid, sizeof(pid);

  int *to_client = open(WKP, O_WRONLY);
  int random_int = rand();
  write(*to_client, &random_int, sizeof(random_int));

  int ACK;
  read(from_client, &ACK, sizeof(ACK);

  if (ACK == random_int + 1) {
      return from_client;
  } else {
      printf("Handshake failed: Invalid ACK\n");
      return -1;
  }



  
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
  write(*to_server, pid, sizeof(pid));

  int server_random_int;
  int from_server = open(pid, O_RDONLY);
  read(from_server, server_random_int, sizeof(server_random_int));

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
  int to_client = 0;
  read(from_client, client_pid, sizeof(client_pid));
  to_client = open(client_pid, O_WRONLY);  // Open client's PP for writing

  const char *message = "Hello!";
  write(to_client, message, strlen(message) + 1);
  
  int ack;
  read(from_client, &ack, sizeof(ack)); 
  
  if (ack == 1) {
      printf("Handshake complete, communication established.\n");
  } else {
      printf("Handshake failed: Invalid ACK\n");
      return -1;
  }
  
  return to_client;
}
