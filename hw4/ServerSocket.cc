/*
 * Copyright ©2022 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.

#include "./ServerSocket.h"

using std::to_string;
using std::string;

extern "C" {
  #include "libhw1/CSE333.h"
}

namespace hw4 {

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int* const listen_fd) {
  // Use "getaddrinfo," "socket," "bind," and "listen" to
  // create a listening socket on port port_.  Return the
  // listening socket through the output parameter "listen_fd"
  // and set the ServerSocket data member "listen_sock_fd_"

  // STEP 1:

  // Populate the "hints" addrinfo structure for getaddrinfo().
  // ("man addrinfo")

  // from ex11 sample
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  
  // makes sure that passed in ai_family is ipv4 or ipv6
  if (ai_family != AF_INET && ai_family != AF_INET6) {
    return false;
  }

  hints.ai_family = ai_family;
  hints.ai_socktype = SOCK_STREAM;  // stream
  hints.ai_flags = AI_PASSIVE;      // use wildcard "INADDR_ANY"
  hints.ai_flags |= AI_V4MAPPED;    // use v4-mapped v6 if no v6 found
  hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  // Use port_ as the string representation of our portnumber to
  // pass in to getaddrinfo().  getaddrinfo() returns a list of
  // address structures via the output parameter "result".
  struct addrinfo* result;
  int res = getaddrinfo(nullptr, to_string(port_).c_str(), &hints, &result);

  // Did addrinfo() fail?
  if (res != 0) {
    return false;
  }

  // Loop through the returned address structures until we are able
  // to create a socket and bind to one.  The address structures are
  // linked in a list through the "ai_next" field of result.
  int l_fd = -1; //listen_fd (but can't name this till end because its a return param)
  for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    l_fd = socket(rp->ai_family,
                  rp->ai_socktype,
                  rp->ai_protocol);
    if (l_fd == -1) {
      // Creating this socket failed.  So, loop to the next returned
      // result and try again.
      l_fd = -1;
      continue;
    }

    // Configure the socket; we're setting a socket "option."  In
    // particular, we set "SO_REUSEADDR", which tells the TCP stack
    // so make the port we bind to available again as soon as we
    // exit, rather than waiting for a few tens of seconds to recycle it.
    int optval = 1;
    setsockopt(l_fd, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof(optval));

    // Try binding the socket to the address and port number returned
    // by getaddrinfo().
    if (bind(l_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
      // Bind worked!  Return to the caller the address family.
      *listen_fd = rp->ai_family;
      sock_family_ = rp->ai_family;
      break;
    }

    // The bind failed.  Close the socket, then loop back around and
    // try the next address/port returned by getaddrinfo().
    close(l_fd);
    l_fd = -1;
  }

  // Free the structure returned by getaddrinfo().
  freeaddrinfo(result);

  // If we failed to bind, return failure.
  if (listen_fd <= 0) {
    return false;
  }

  // Success. Tell the OS that we want this to be a listening socket.
  if (listen(l_fd, SOMAXCONN) != 0) {
    close(l_fd);
    return false;
  }

  // Return to the client the listening file descriptor.
  listen_sock_fd_ = l_fd;
  *listen_fd = l_fd;
  return true;
}

bool ServerSocket::Accept(int* const accepted_fd,
                          std::string* const client_addr,
                          uint16_t* const client_port,
                          std::string* const client_dns_name,
                          std::string* const server_addr,
                          std::string* const server_dns_name) const {
  // Accept a new connection on the listening socket listen_sock_fd_.
  // (Block until a new connection arrives.)  Return the newly accepted
  // socket, as well as information about both ends of the new connection,
  // through the various output parameters.

  // STEP 2:
  // Wait for a client to arrive.
  // from ex11 sample
  int client_fd;
  struct sockaddr_storage caddr;
  socklen_t caddr_len = sizeof(caddr);
  struct sockaddr *addr = reinterpret_cast<struct sockaddr *>(&caddr);
  while (1) {
    client_fd = accept(listen_sock_fd_, addr, &caddr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;
      }
      return false;
    }
    break;
  }

  // update output params
  *accepted_fd = client_fd;

  // client params
  // from PrintOut in server_accept_rw_close.cc
  if (addr->sa_family == AF_INET) { // handle IPv4

    char astring[INET_ADDRSTRLEN];
    struct sockaddr_in* in4 = reinterpret_cast<struct sockaddr_in *>(addr);
    inet_ntop(AF_INET, &(in4->sin_addr), astring, INET_ADDRSTRLEN);

    *client_addr = string(astring);
    *client_port = ntohs(in4->sin_port);
  } else { // handle IPv6

    char addr_string[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *in6 = reinterpret_cast<struct sockaddr_in6 *>(addr);
    inet_ntop(AF_INET6, &(in6->sin6_addr), addr_string, INET6_ADDRSTRLEN);

    *client_addr = string(addr_string);
    *client_port = ntohs(in6->sin6_port);
  }

  // client DNS params
  // from PrintReverseDNS in server_accept_rw_close.cc
  char hostname[1024]; // ought to be big enough.
  if (getnameinfo(addr, caddr_len, hostname, 1024, NULL, 0, 0) != 0) {
    return false;
  }
  *client_dns_name = string(hostname);  // store in output param

  // Server params
  // from PrintServerSide in server_accept_rw_close.cc
  char hname[1024];
  hname[0] = '\0';
  if (sock_family_ == AF_INET) {
    // The server is using an IPv4 address.
    struct sockaddr_in srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET_ADDRSTRLEN];
    getsockname(client_fd,
                reinterpret_cast<struct sockaddr*>(&srvr),
                &srvrlen);
    inet_ntop(AF_INET, &srvr.sin_addr, addrbuf, INET_ADDRSTRLEN);
    // Get the server's dns name, or return it's IP address as
    // a substitute if the dns lookup fails.
    getnameinfo(reinterpret_cast<struct sockaddr*>(&srvr),
                srvrlen, hname, 1024, nullptr, 0, 0);

    *server_addr = string(addrbuf);
    *server_dns_name = string(hname);
  } else {
    // The server is using an IPv6 address.
    struct sockaddr_in6 srvr;
    socklen_t srvrlen = sizeof(srvr);
    char addrbuf[INET6_ADDRSTRLEN];
    getsockname(client_fd,
                reinterpret_cast<struct sockaddr*>(&srvr),
                &srvrlen);
    inet_ntop(AF_INET6, &srvr.sin6_addr, addrbuf, INET6_ADDRSTRLEN);
    std::cout << addrbuf;
    // Get the server's dns name, or return it's IP address as
    // a substitute if the dns lookup fails.
    getnameinfo(reinterpret_cast<struct sockaddr*>(&srvr),
                srvrlen, hname, 1024, nullptr, 0, 0);

    *server_addr = string(addrbuf);
    *server_dns_name = string(hname);
  }
  return true;
}

}  // namespace hw4
