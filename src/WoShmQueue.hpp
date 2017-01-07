
#ifndef WOSHMQUEUE_HPP
#define WOSHMQUEUE_HPP

#include "WoMessage.hpp"

class WoShmAllocator;

class WoShmQueue
{
public: 
    WoShmQueue();
    ~WoShmQueue();

    WoMessage pop();

private:
    WoShmAllocator* allocator_;
};

#endif
