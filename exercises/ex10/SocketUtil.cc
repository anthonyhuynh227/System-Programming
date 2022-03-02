#include "SocketUtil.h"

#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

bool LookupName(char* name,
                unsigned short port,
                struct sockaddr_storage* ret_addr,
                size_t* ret_addrlen) {
  struct addrinfo hints;
  struct addrinfo *result;
  int s;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /* Stream socket */

  // perform lookup
  s = getaddrinfo(name, NULL, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    return false;
  }

  if (result->ai_family == AF_INET) { //ipv4 addr
    struct sockaddr_in* ipv4addr = (struct sockaddr_in*) result->ai_addr;
    ipv4addr->sin_port = htons(port);
  } else if (result->ai_family == AF_INET6) { // ipv6 addr
    struct sockaddr_in6* ipv6addr = (struct sockaddr_in6 *) result->ai_addr;
    ipv6addr->sin6_port = htons(port);
  } else {
    cerr << "Could not get an IPv4 or IPv6 address!" << endl;
    return false;
  }
  // copy results into return address
  memcpy(ret_addr, result->ai_addr, result->ai_addrlen);
  *ret_addrlen = result->ai_addrlen;
  freeaddrinfo(result);
  return true;
}

bool Connect(const struct sockaddr_storage& addr,
             const size_t& addrlen,
             int* ret_fd) {
  int socket_fd = socket(addr.ss_family, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    cerr << "Failed to create socket!" << endl;
    return false;
  }

  // conncet to host
  if (connect(socket_fd, reinterpret_cast<const sockaddr *>(&addr),
      addrlen) == -1) {
    cerr << "Could not connect to host!" << endl;
    return false;
  }

  // was able to connect, update return file descriptor
  *ret_fd = socket_fd;
  return true;
}

int WrappedRead(int fd, unsigned char* buf, int readlen) {
  int result;
  while (true) {
    result = read(fd, buf, readlen);
    if (result == -1) {
      if ((errno == EAGAIN) || (errno == EINTR))
        continue;
    }
    break;
  }
  //cout << "performed read" << endl;
  return result;
}

int WrappedWrite(int fd, unsigned char* buf, int writelen) {
  int result;
  int writebuf;

  while (writebuf < writelen) {
    result = write(fd, buf + writebuf, writelen-writebuf);
    if (result == -1) {
      if ((errno == EAGAIN) || (errno == EINTR))
        continue;
      break;
    }
    if (result == 0) {
      break;
    }
    writebuf += result;
  }
  //cout << "performed write" << endl;
  return writebuf;
}
