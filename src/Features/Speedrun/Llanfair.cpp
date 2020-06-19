#include "Llanfair.hpp"

#include "Modules/Console.hpp"

#include "Command.hpp"

#include "SpeedrunTimer.hpp"

#include <chrono>
#include <sstream>

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

using namespace std::chrono;

Llanfair* llanfair;

Llanfair::Llanfair()
    : state(0),
	sockfd(0),
	startms(0ULL)
{
    this->hasLoaded = true;
}
void Llanfair::ChangeState(int newState)
{
    this->state = newState;
}
int Llanfair::GetState()
{
    return this->state;
}

void Llanfair::StartClient(int port) {
	std::string hostname = "localhost";
	std::string nbhd;
	
	// const char* chostname = hostname.c_str();

	struct sockaddr_in serv_addr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0) {
		console->Warning("Error opening client socket!\n");
	}
	else {
		server = gethostbyname(hostname.c_str());

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(port);

		if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
			console->Warning("Error connecting to server!\n");
		}
		else {
			std::ostringstream ss;
			ss << "Connected to server on port " << port << "!\n";
			console->Print(ss.str().c_str());
			llanfair->ChangeState(1);
		}
	}
}

void Llanfair::StopClient() {
	sockfd = -1;
	llanfair->ChangeState(0);
}

void Llanfair::SendMsg(int code) {
	if(sockfd == -1) {
		std::ostringstream ss;
		ss << "Server is not running! (code " << code << ")\n";
		console->Warning(ss.str().c_str());
	}
	char buffer[256];
	bzero(buffer, 256);

	std::string sbuffer;

	unsigned long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if(code == 1) {
		startms = ms;
		std::ostringstream ss;
		ss << ms;
		console->Print("Saved start time: %s\n", ss.str().c_str());
	}
	std::ostringstream oss;
	oss << code << ":" << ms;

	sbuffer.append(oss.str());

	strncpy(buffer, sbuffer.c_str(), 254);
	buffer[sbuffer.length()] = '\n';
	buffer[sbuffer.length() + 1] = '\0';

	int n;
	n = send(sockfd, buffer, strlen(buffer) + 1, MSG_NOSIGNAL);
	if(n < 0) {
		console->Warning("Error writing to server!\n");
	}
}

void Llanfair::SendMsg(int code, int ticks, float ipt) {
	if(sockfd == -1) {
		std::ostringstream ss;
		ss << "Server is not running! (code " << code << ")\n";
		console->Warning(ss.str().c_str());
	}
	char buffer[256];
	bzero(buffer, 256);

	std::string sbuffer;

	// console->Print("Sending split to timer at %i ticks\n", ticks);

	std::ostringstream oss;
	oss << code << ":" << startms + (unsigned long long) (ticks * ipt * 1000);

	sbuffer.append(oss.str());

	strncpy(buffer, sbuffer.c_str(), 254);
	buffer[sbuffer.length()] = '\n';
	buffer[sbuffer.length() + 1] = '\0';

	int n;
	if((!speedrun->IsPaused() && code == 6) || (speedrun->IsPaused() && code == 7) || code != -1) {
		n = send(sockfd, buffer, strlen(buffer) + 1, MSG_NOSIGNAL);
		
	}
	else {
		console->Print("Not sending split\n");
	}
	if(n < 0) {
		console->Warning("Error writing to server!\n");
	}
}


CON_COMMAND(ll_connect, "Connect to the Llanfair timer server.\nUsage: ll_connect <port>") {
	if(args.ArgC() != 2) {
		return console->Print("Please enter a port!\n");
	}

	llanfair->StartClient(atoi(args[1]));
}

CON_COMMAND(ll_disconnect, "Disconnects from the Llanfair timer server.\n") {
	llanfair->StopClient();
	console->Print("Disconnected from Llanfair!\n");
}

CON_COMMAND(ll_state, "Get the state of the connection to the Llanfair timer server.\n") {
	int state = llanfair->GetState();
	if(state == 0) {
		console->Print("Disconnected.\n");
	}
	else if(state == 1) {
		console->Print("Connected.\n");
	}
	else {
		console->Print("Unknown state.\n");
	}
}

CON_COMMAND(ll_time, "Get current system time.\n") {
	unsigned long long ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	std::ostringstream oss;
	oss << ms;
	console->Print(oss.str().c_str());
}
