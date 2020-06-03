#include "Features/Feature.hpp"

#include "Command.hpp"

class Llanfair : public Feature {
private:
    int state;

public:
    Llanfair();
    void ChangeState(int newState);
    int GetState();
};

extern Llanfair* llanfair;

extern Command sar_ll_connect;
extern Command sar_ll_disconnect;
extern Command sar_ll_state;
