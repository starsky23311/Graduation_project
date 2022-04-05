#include "JKRobotControl/TCPServer.h"
#define endr "\n\r"
string TCPServer::Message;

TCPServer::TCPServer() {
  newsockfd = 0;
}

void* TCPServer::Task(void* arg) {
  int n;
  int* pnewsockfd = (int*)arg;
  char msg[MAXPACKETSIZE];
  pthread_detach(pthread_self());
  while (1) {
    n = recv(*pnewsockfd, msg, MAXPACKETSIZE, 0);
    if (n == 0) {
      close(*pnewsockfd);
      *pnewsockfd = 0;
      cout << "Client is disconnected\n\r";
      break;
    }
    msg[n] = 0;
    // send(newsockfd, msg, n, 0);
    Message = string(msg);
  }
  return 0;
}

int TCPServer::setup(int port) {
	int ret;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int on = 1;
	ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));	
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(port);
  ret = bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (ret < 0) cout << "bind : " << ret << endr;
  ret = listen(sockfd, 5);
	if (ret < 0) cout << "listen : " << ret << endr;
	return ret;
}

string TCPServer::receive(int* thread_alive) {
  string str;

  while (thread_alive == NULL ? 1 : *thread_alive) {
    socklen_t sosize  = sizeof(clientAddress);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientAddress, &sosize);
    str = inet_ntoa(clientAddress.sin_addr);
		cout << "Client is connected" << endr;
    pthread_create(&serverThread, NULL, &Task, (void*)(&newsockfd));
  }
  return str;
}

string TCPServer::getMessage() {
  return Message;
}

int TCPServer::Send(string msg) {
  if (newsockfd <= 0) return -1;
  return send(newsockfd, msg.c_str(), msg.length(), 0);
}

void TCPServer::clean() {
  Message = "";
  memset(msg, 0, MAXPACKETSIZE);
}

int TCPServer::detach() {
	int ret;
  ret = close(sockfd);
	if (ret < 0) {cout << "close " << ret << endr; return ret;}
  ret = close(newsockfd);
	if (ret < 0) {cout << "close " << ret << endr; return ret;}
	return ret;
}
