#include "IDGenerator.hpp"

IDGenerator::IDGenerator() : counter(0) {}

IDGenerator::getID() {
    lock.lock();
    unsigned long long new_id = counter;
    counter++;
    lock.unlock();
    return new_id;
}
