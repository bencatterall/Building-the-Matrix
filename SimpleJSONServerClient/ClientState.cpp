#include "ClientState.hpp"

ClientState::ClientState(): timeout(10000) {
    this->bump();
}

ClientState::ClientState(Address address) : client_address(address), timeout(10000) {
    this->bump();
}

ClientState::~ClientState() {}

void ClientState::bump() {
    time_last_updated = std::chrono::system_clock::now();
}

std::chrono::time_point<std::chrono::system_clock> ClientState::getTimeLastUpdated() const {
    return time_last_updated;
}

bool ClientState::timedOut() const {
    return (std::chrono::system_clock::now() - time_last_updated) > timeout;
}
