#include "HttpServer.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>

HttpServer::HttpServer() : _port("8080"), _backlog(10) {

  struct addrinfo hints{};	
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, _port.c_str(), &hints, &res);
}

HttpServer::~HttpServer() {
  freeaddrinfo(res);
}

int HttpServer::initServer() {  

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if (sockfd == -1){
     std::cerr << "server: socket error: " << std::strerror(errno) << std::endl;
     return -1;
  }
  
  if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1){
    std::cerr << "server bind error: " << std::strerror(errno) << std::endl;
    close(sockfd);
    return -1;
  }

  if (listen(sockfd, _backlog) == -1){
    std::cerr << "server listen error: " << std::strerror(errno) << std::endl;
    if (sockfd != -1){
      close(sockfd);
    }
    return -1;
  }
  
  std::cout << "Server is running" << std::endl;
  return 0;
}


void HttpServer::runServer(){
  
  struct sockaddr_storage their_addr;
  socklen_t addr_size = sizeof(their_addr);

  while(1){
    addr_size = sizeof(their_addr);
    int newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    
    std::cout << "Connection accepted on sockfd = " << newfd << std::endl;

    if (newfd == -1) {
      std::cerr << "server accept error: " << std::strerror(errno) << std::endl;
      continue;
    }
    
    handleClient(newfd);

    close(newfd);
  }  
}

// For now the server echoes the client's message back
void HttpServer::handleClient(int newfd){
  
  std::vector<char> buff(1024);
  std::string message = "Welcome to my echo HTTP server!!!\n";
  buff.assign(message.begin(), message.end());  

  send(newfd, buff.data(), buff.size(), 0);
  buff.clear();

  while (1){
    
    std::vector<char> read_buff(1024);
    int response = recv(newfd, read_buff.data(), read_buff.size(), 0);

    if (response <= 0){
      break;
    }
    
    // handleRequest();

    send(newfd, read_buff.data(), response, 0);
  }
}
