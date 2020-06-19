#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include <chrono>
#include <sstream>

#include <regex>
#include <algorithm>

using namespace std;

int main() {
	string hostname, nbhd;
	int pnum;

	cout << "Enter server hostname: ";
	cin >> hostname;

	cout << "\nEnter server port number: ";
	cin >> pnum;

	const char* chostname = hostname.c_str();

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];

	portno = pnum;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0) {
		perror("Error opening socket");
		exit(0);
	}

	server = gethostbyname(chostname);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Error connecting to server");
		exit(1);
	}

	while(1) {
		cout << endl;

		// bzero(buffer, 256);

		//* Get message from server
		// int offset = 0;
		// int len = 16;

		// while(offset < len) {
		// 	int rcv_bytes = recv(sockfd, &(buffer[offset]), (len - offset), 0);
		// 	// errors

		// 	if(rcv_bytes < 0) {
		// 		perror("Error reading from socket");
		// 		exit(1);
		// 	}

		// 	offset += rcv_bytes;
		// }

		//* Old method of getting from server
		// string inbuffer;
		// char c;
		// while('\n' != (c = read(sockfd, buffer, 1))) {
		// 	inbuffer.append(string(c));
		// }
		
		// cout << "Received:" << buffer;
		// if(buffer[0] == '~') {
		// 	close(0);
		// 	exit(0);
		// }

		printf("> ");
		bzero(buffer, 256);
		string sbuffer;
		cin >> sbuffer;

		unsigned long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::ostringstream oss;
		oss << ms;
		// oss.str().c_str();

		std::regex newlines_re("\n+");

		sbuffer = regex_replace(sbuffer, newlines_re, "");
		sbuffer.append(":" + oss.str());

		//* Attempt to fix space splitting with cin
		// char* bptr = buffer;
		// size_t bsz = 255;
		// ::getline(&(bptr), &(bsz), stdin);

		strncpy(buffer, sbuffer.c_str(), 254);
		buffer[sbuffer.length()] = '\n';
		buffer[sbuffer.length() + 1] = '\0';

		//* Something
		// int c = getchar();
		// char* cp = fgets(buffer, 255, stdin);
		// printf("%p %p\n", cp, buffer);

		cout << buffer;
		if(buffer[0] == '\n') {
			close(0);
			exit(0);
		}

		// n = write(sockfd, buffer, strlen(buffer) + 1);
		n = send(sockfd, buffer, strlen(buffer) + 1, MSG_NOSIGNAL);
		// cout << "Wrote message to server\n";
		// cout << "n = " << n;
		printf("n = %d\n", (int) n);
		if(n < 0) {
			perror("Error writing to socket");
			exit(1);
		}
	}
}


