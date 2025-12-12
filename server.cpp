// g++ -o server.exe server.cpp csrc/net.cpp csrc/hashtable.cpp csrc/cache.cpp csrc/llist.cpp csrc/mime.cpp csrc/mime_table.cpp csrc/file.cpp -Icsrc -lws2_32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "ws2_32.lib")
  #define close closesocket
#else
  #include <unistd.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <arpa/inet.h>
#endif

#include "csrc/net.h"
#include "csrc/file.h"
#include "csrc/mime.h"
#include "csrc/cache.h"

#define PORT "3490"
#define SERVER_FILES "./csrc/serverfiles"
#define SERVER_ROOT "./csrc/serverroot"
#define CACHE_MAX_SIZE 10
#define CACHE_HASHSIZE 128

void send_response(int fd, char *header, char *content_type, void *body, int content_length) {
  const int max_response_size = 262144;
  char response[max_response_size];
  int response_length = snprintf(response, max_response_size,
    "%s\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %d\r\n"
    "Connection: close\r\n"
    "\r\n", header, content_type, content_length);
  memcpy(response + response_length, body, content_length);
  send(fd, response, response_length + content_length, 0);
}

void send_404(int fd) {
  char filepath[4096];
  snprintf(filepath, sizeof(filepath), "%s/404.html", SERVER_FILES);
  file_data *filedata = file_load(filepath);
  if (filedata == NULL) {
    char *response = (char*)"HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
    send(fd, response, strlen(response), 0);
    return;
  }
  send_response(fd, (char*)"HTTP/1.1 404 NOT FOUND", (char*)"text/html", filedata->data, filedata->size);
  file_free(filedata);
}

void get_file(int fd, cache *cache, char *request_path) {
  char filepath[4096];
  if (strcmp(request_path, "/") == 0) snprintf(filepath, sizeof(filepath), "%s/index.html", SERVER_ROOT);
  else snprintf(filepath, sizeof(filepath), "%s%s", SERVER_ROOT, request_path);
  cache_entry *entry = cache_get(cache, filepath);
  if (entry != NULL) {
    send_response(fd, (char*)"HTTP/1.1 200 OK", entry->content_type, entry->content, entry->content_length);
    printf("Cache hit: %s\n", filepath);
    return;
  }
  file_data *filedata = file_load(filepath);
  if (filedata == NULL) {
    send_404(fd);
    return;
  }
  char *mime_type = mime_type_get(filepath);
  cache_put(cache, filepath, mime_type, filedata->data, filedata->size);
  send_response(fd, (char*)"HTTP/1.1 200 OK", mime_type, filedata->data, filedata->size);
  printf("Loaded file: %s\n", filepath);
  file_free(filedata);
}

void handle_request(int fd, cache *cache) {
  const int request_buffer_size = 65536;
  char request[request_buffer_size];
  int bytes_recvd = recv(fd, request, request_buffer_size - 1, 0);
  if (bytes_recvd < 0) {
    perror("recv");
    return;
  }
  request[bytes_recvd] = '\0';
  char method[8], path[8192], protocol[128];
  sscanf(request, "%s %s %s", method, path, protocol);
  printf("Request: %s %s %s\n", method, path, protocol);
  if (strcmp(method, "GET") == 0) get_file(fd, cache, path);
  else {
    char *response = (char*)"HTTP/1.1 501 NOT IMPLEMENTED\r\nContent-Type: text/plain\r\n\r\nMethod not implemented";
    send(fd, response, strlen(response), 0);
  }
}

int main(void) {
  int newfd, listenfd;
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  char s[INET6_ADDRSTRLEN];
  cache *cache = cache_create(CACHE_MAX_SIZE, CACHE_HASHSIZE);
  if (cache == NULL) {
    fprintf(stderr, "Failed to create cache\n");
    return 1;
  }
  listenfd = get_listener_socket((char*)PORT);
  if (listenfd < 0) {
    fprintf(stderr, "webserver: fatal error getting listening socket\n");
    return 1;
  }
  printf("Server listening on port %s\n", PORT);
  while (1) {
    sin_size = sizeof their_addr;
    newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
    if (newfd == -1) {
      perror("accept");
      continue;
    }
    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("Connection from %s\n", s);
    handle_request(newfd, cache);
    close(newfd);
  }
  free_cache(cache);
  return 0;
}