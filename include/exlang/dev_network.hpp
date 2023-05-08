#ifndef EXLANG_DEV_NETWORK_HPP
#define EXLANG_DEV_NETWORK_HPP

namespace exlang {
static constexpr int NETWORK_ACCESS_COMMAND_ADDRESS = 10;
static constexpr int NETWORK_ACCESS_DATA_ADDRESS = 20;

#ifndef MICRO_DEVICE
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

struct _network_handle {
	int socket_fd;
	int port;
	long bytes_in;
	long bytes_out;
};

#endif
}; // namespace exlang
#endif
