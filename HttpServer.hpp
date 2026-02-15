#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

class HttpServer {
private:
  int sockfd;
  struct addrinfo *res;
  const int _backlog;
  const std::string _port;
public:
  HttpServer();
  ~HttpServer();
  int initServer();
  void runServer();
  // void handleClient();
};
