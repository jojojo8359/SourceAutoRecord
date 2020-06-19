#include "Features/Feature.hpp"

#include "Command.hpp"

class Llanfair : public Feature {
private:
    int state;
    int sockfd;
    unsigned long long startms;

public:
    Llanfair();
    void ChangeState(int newState);
    int GetState();
    void StartClient(int port);
    void StopClient();
    void SendMsg(int code);
    void SendMsg(int code, int ticks, float ipt);
};

extern Llanfair* llanfair;

extern Command ll_connect;
extern Command ll_disconnect;
extern Command ll_state;
extern Command ll_time;
