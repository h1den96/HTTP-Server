#include <iostream>
#include "HttpServer.hpp"
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
*/
int main (int argc, char** argv){

  HttpServer server;
  server.initServer();  

  return 0;
}
