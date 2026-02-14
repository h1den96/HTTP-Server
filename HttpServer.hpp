#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

class HttpServer {
private:
  int sockfd;
  struct addrinfo *res;

public:
  HttpServer();
  ~HttpServer();
  int initServer();
};
