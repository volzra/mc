#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// #include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "version.h"
#include "net/type.h"
#include "net/pkt.h"

void dump(void *buf, int n) {
  char *_buf = buf;
  int a = n;
  while (a-- > 0)
    printf("%x ", (*_buf++) & 0xff);

  putc('\n', stdout);

  a = n;
  _buf = buf;
  while (a-- > 0)
    printf("%c", (*_buf++));
  putc('\n', stdout);
}

void *create_bytearray(size_t len) {
  return malloc(len);
}

void destroy_bytearray(void *ptr) {
  free(ptr);
}

int main(int argc, char *argv[]) {

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  int ret;

  if (fd < 0) {
    perror("socket()");
    exit(1);
  }

  struct sockaddr_in sin = {
      .sin_family = AF_INET,
      .sin_port = htons(25565),
  };

  inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
  ret = connect(fd, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));

  if (ret != 0) {
    perror("connect()");
    exit(1);
  }

  serverinfo_t si = {0};
  memset(&si, 0, sizeof(si));
  si.si_conninfo.addr = "127.0.0.1";
  si.si_conninfo.port = 25565;
  si.si_conninfo.proto = MC_1_6_2;
  si.si_conninfo.sockfd = fd;

  userinfo_t ui = {
    .ui_name = "PLAYER",
  };

  int len;
  char buf[256];
  len = send_handshake(&si, 2);
  // len = send_slp(&si);
  // len = send_ping(&si, 0x4142434445464748);
  len = send_login(&si, &ui);

  read_response(&si, &ui, NULL);
  return 0;
}
