#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "./SocketUtil.h"
using std::cout;
using std::endl;

// Open a local file, open a socket to a remote process,
// and send the file to other process.
int main(int argc, char** argv) {
  int buf_size = 512;
  unsigned char buf[buf_size];
  // validating args
  if (argc != 4) {
    cout << "Please provide 3 arguments: hostname, port number, local file." << endl;
    return EXIT_FAILURE;
  }
  unsigned short port;
  if (sscanf(argv[2], "%hu", &port) != 1) {
    cout << "Please provide a valid port number." << endl;
    return EXIT_FAILURE;
  }
  if (port < 1024 || port > 65535) {
    cout << "Please use a port number between 1024 and 65535." << endl;
  }
  int fd = open(argv[3], O_RDONLY);
  if (fd  == -1) {
    cout << "Could not open file in read only format!" << endl;
    return EXIT_FAILURE;
  }
  // connecting to host
  struct sockaddr_storage ret_addr;
  size_t ret_addrlen;
  if (!LookupName(argv[1], port, &ret_addr, &ret_addrlen)) {
    return EXIT_FAILURE;
  }
  int socket_fd;
  if (!Connect(ret_addr, ret_addrlen, &socket_fd)) {
    return EXIT_FAILURE;
  }
  // read file
  int read_result, write_result;
  while (true) {
    read_result = WrappedRead(fd, buf, buf_size);
    if (read_result <= 0) {
      break;
    }
    // write to socket
    write_result = WrappedWrite(socket_fd, buf, read_result);
    if (read_result != write_result) {
      break;
    }
    close(fd);
    close(socket_fd);
    return EXIT_SUCCESS;
  }
}
