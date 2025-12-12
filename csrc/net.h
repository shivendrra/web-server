#ifndef _NET_H_
#define _NET_H_

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <sys/socket.h>
#endif

extern "C" {
  void* get_in_addr(struct sockaddr *sa);
  int get_listener_socket(char *port);
}

#endif