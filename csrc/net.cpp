#include <stdio.h>
#include <string.h>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  #define close closesocket
#else
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <arpa/inet.h>
#endif

#include "net.h"

#define BACKLOG 10

void* get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) return &(((struct sockaddr_in*)sa)->sin_addr);
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int get_listener_socket(char *port) {
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int yes = 1, rv;
  
#ifdef _WIN32
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    fprintf(stderr, "WSAStartup failed\n");
    return -1;
  }
#endif

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return -1;
  }
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int)) == -1) {
      perror("setsockopt");
      close(sockfd);
      freeaddrinfo(servinfo);
      return -2;
    }
    if (bind(sockfd, p->ai_addr, (int)p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    break;
  }
  freeaddrinfo(servinfo);
  if (p == NULL) {
    fprintf(stderr, "webserver: failed to find local address\n");
    return -3;
  }
  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    close(sockfd);
    return -4;
  }
  return sockfd;
}