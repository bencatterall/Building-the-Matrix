#ifndef IDGEN_H
#define IDGEN_H

#include <mutex>

class IDGenerator
{
    unsigned long long counter;
    std::mutex lock;
public:
    IDGenerator();
    ~IDGenerator();
    unsigned long long getID();
};

#endif
