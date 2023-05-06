#ifndef _modules_network_hpp
#define _modules_network_hpp

#define NETWORK_ACCESS_COMMAND_ADDRESS 10
#define NETWORK_ACCESS_DATA_ADDRESS 20

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
#endif
