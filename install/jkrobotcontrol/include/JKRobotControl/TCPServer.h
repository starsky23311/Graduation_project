#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

using namespace std;

#define MAXPACKETSIZE 4096

class TCPServer {
 public:
  int sockfd, newsockfd, n, pid;
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  pthread_t serverThread;
  char msg[MAXPACKETSIZE];
  static string Message;

  TCPServer();
  int setup(int port);
  string receive(int* thread_alive);
  string getMessage();
  int Send(string msg);
  int detach();
  void clean();

 private:
  static void* Task(void* argv);
};

#endif
