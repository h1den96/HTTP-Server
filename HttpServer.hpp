#include <iostream>
#include <vector>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>

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
  void handleClient(int);
  std::string handleRequest(std::vector<char>, int);
};
