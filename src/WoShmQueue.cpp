
#include "WoShmQueue.hpp"
#include "WoShmAllocator.hpp"

WoShmQueue::WoShmQueue() :
    allocator_(new WoShmAllocator)
{
}

WoShmQueue::~WoShmQueue()
{
    delete allocator_;
}

WoMessage WoShmQueue::pop()
{
    return WoMessage();
}
