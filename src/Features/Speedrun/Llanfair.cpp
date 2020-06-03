#include "Llanfair.hpp"

#include "Modules/Console.hpp"

#include "Command.hpp"

#include <chrono>
#include <sstream>

using namespace std::chrono;

Llanfair* llanfair;

Llanfair::Llanfair()
    : state(0)
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


CON_COMMAND(ll_connect, "Connect to the Llanfair timer server.\n") {
	llanfair->ChangeState(1);
	console->Print("Connected to Llanfair!\n");
}

CON_COMMAND(ll_disconnect, "Disconnects from the Llanfair timer server.\n") {
	llanfair->ChangeState(0);
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
