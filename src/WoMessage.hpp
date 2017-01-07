
#ifndef WOMESSAGE_HPP
#define WOMESSAGE_HPP

#include <cstddef>

class WoMessage
{
public:
    WoMessage(const char* data, size_t size);
    ~WoMessage();

    const char* data() const
    {
        return data_;
    }
    size_t size() const
    {
        return size_;
    }

private:
    const char* data_;
    size_t size_;
};

#endif
