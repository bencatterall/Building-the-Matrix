#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H

#include "Address.hpp"
#include <chrono>

class ClientState {
private:
    Address client_address;
    std::chrono::time_point<std::chrono::system_clock> time_last_updated;
    std::chrono::seconds timeout;
public:
    ClientState();
    ClientState(Address address);
    ~ClientState();
    void bump();
    std::chrono::time_point<std::chrono::system_clock> getTimeLastUpdated() const;
    bool timedOut() const;
};

#endif