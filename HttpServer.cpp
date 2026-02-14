#include <HttpServer.hpp>

const std::string PORT_NUMBER = "8080";

HttpServer::HttpServer(){

  struct addrinfo hints{};	
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, PORT_NUMBER.c_str(), &hints, &res);
}

HttpServer::~HttpServer(){
  freeaddrinfo(res);

}

int HttpServer::initServer(){  

  this->sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)

  if (this->sockfd == -1){
     std::cerr << "server: socket error: " << std::strerror(errno) << std::endl;
     return -1;
  }
  
  if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    std::cerr << "server bind error: " << std::strerror(errno) << std::endl;
    close(sockfd);
    return -1;
  }

  return 0;
}
