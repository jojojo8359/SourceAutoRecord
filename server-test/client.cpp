#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

static void usage();

int main(int argc, char* argv[]) {
	if(argc > 1 && *(argv[1]) == '-') {
		usage();
		exit(1);
	}

	int s0 = socket(AF_INET, SOCK_STREAM, 0);
	if(s0 < 0) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(1);
	}

	struct sockaddr_in server;
	int serverAddrLen;
	bzero(&server, sizeof(server));

	char* peerHost = "localhost";
	if(argc > 1)
		peerHost = argv[1];
	
	struct hostent *host = gethostbyname(peerHost);
	if(host == NULL) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(1);
	}

	server.sin_family = AF_INET;
	short peerPort = 1234;
	if(argc >= 3)
		peerPort = (short) atoi(argv[2]);
	server.sin_port = htons(peerPort);

	std::cout << "server address = " << (host->h_addr_list[0][0] & 0xff) << "." <<
										(host->h_addr_list[0][1] & 0xff) << "." <<
										(host->h_addr_list[0][2] & 0xff) << "." <<
										(host->h_addr_list[0][3] & 0xff) << ", port " <<
										static_cast<int>(peerPort) << std::endl;
	
	memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

	int res = connect(s0, (struct sockaddr*) &server, sizeof(server));
	if(res < 0) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(1);
	}

	std::cout << "Connected." << std::endl;

	char buffer[1024];
	res = read(s0, buffer, 1024);
	if(res < 0) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(1);
	}

	std::cout << "Received:" << "\n" << buffer;

	write(s0, "Thanks! Bye-bye...\r\n", 20);

	close(s0);
	return 0;
}

static void usage()
{
    std::cout << "A simple Internet client application.\n"
              << "Usage:\n"
              << "         client [IP_address_of_server [port_of_server]]\n"
              << "     where IP_address_of_server is either IP number of server\n"
              << "     or a symbolic Internet name, default is \"localhost\";\n"
              << "     port_of_server is a port number, default is 1234.\n"
              << "The client connects to a server which address is given in a\n"
              << "command line, receives a message from a server, sends the message\n"
              << "\"Thanks! Bye-bye...\", and terminates.\n";
}
