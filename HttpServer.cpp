#include "HttpServer.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sstream>

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

void HttpServer::handleClient(int newfd){
  
  std::vector<char> buff(1024);
  std::string message = "Welcome to my echo HTTP server!!!\n";
  buff.assign(message.begin(), message.end());  

  send(newfd, buff.data(), buff.size(), 0);
  buff.clear();

  while (1){
    
    std::vector<char> read_buff(1024);
    int client_response = recv(newfd, read_buff.data(), read_buff.size(), 0);

    if (client_response <= 0){
      break;
    }
    
    std::string request_response = handleRequest(read_buff, client_response);

    send(newfd, request_response.c_str(), request_response.length(), 0);
  }
}

std::string HttpServer::handleRequest(std::vector<char> read_buff, int client_response){
  
  std::string line(read_buff.data(), client_response);
  std::stringstream ss(line);
  std::string method, path, protocol, extra;

  std::string response;

  if (ss >> method >> path >> protocol) {
    
    if (ss >> extra){ 
      std::cout << "Error: HTTP request format composed of:  method filepath http version" << std::endl;
      return response = "Error: HTTP request format composed of:  method filepath http version\n";
    }
    // Here is the main part where the handling of the request takes place

    // Must check:
    // 1. Method validity (GET, POST, etc.)
    // 2. If path exists and its correctly formated (regex?)
    // 3. HTTP version validity
    else {
      std::cout << "Method: " << method << "\n";
      std::cout << "Path: " << path << "\n";
      std::cout << "Protocol: " << protocol << "\n";


      // Could build a larger string with everything that's wrong with the request format
      if (!checkMethod(method)){
        std::cout << "Invalid HTTP method '" << method << << "'\n";
      }

      if (!checkPath(path)){
        std::cout << "Invalid path format for '" << path << "' correct: /path/.../...\n"; 
      }

      if (!checkProtocol(protocol)){
        std::cout << "Invalid protocol '" << protocol << "' correct: HTTP/version (1.0, 1.1)\n";
      }
    }
  } 
  else {     
    std::cerr << "Error: Invalid HTTP request format" << std::endl;
    return response =  "Error: Invalid HTTP request format\n";
  }
  return response = "File contents\n";
}

bool HttpServer::checkMethod(std::string method){
  return (method == "GET" || method = "POST");
}

bool HttpServer::checkPath(std::string path){
  return true;
}

bool HttpServer::checkProtocol(std::string protocol){
  return (protocol == "HTTP/1.1" || "HTTP/1.0");
}






