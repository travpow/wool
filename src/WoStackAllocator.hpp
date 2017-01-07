

template<typename T, size_t N>
class WoStackAllocator
{
public:
    WoStackAllocator()
    {
        for (int i = 0; i < N; i++)
        {
            free_[i] = &buffer_[i];
        }

        sp_ = N - 1;
    };

    T* allocate()
    {
        if (sp_)
        {
            T* alloc = free_[sp_];
            sp_--;

            *alloc = T();

            return alloc;
        }
        else
        {
            return nullptr;
        }
    }

    void deallocate(T* mem)
    {
        if (!mem)
            return;

        free_[sp_++];
    }

private:
    T buffer_[N];
    T* free_[N];
    T* sp_;
};
