

#ifndef SITEST_RINGBUFFER_HPP
#define SITEST_RINGBUFFER_HPP

#include <vector>
#include <algorithm>

template <typename T>
class RingBuffer
{
public:
    explicit RingBuffer(int size):
            d_buffer(size),
            d_read_index(0),
            d_write_index(size - 1),
            d_max_size(size),
            d_size(0)
    {

    }

    ~RingBuffer() = default;

    void push_back(const T& data)
    {
        d_size = (d_size == max_size()) ? max_size() : d_size + 1;

        d_buffer[d_write_index++] = data;

        if(d_write_index >= d_buffer.size())
            d_write_index = 0;
    }

    const T& get()
    {
        const T& val = d_buffer[d_read_index++];

        if(d_read_index >= d_buffer.size())
            d_read_index = 0;

        return val;
    }

    bool find(const T& data) const
    {
        return std::find(d_buffer.begin(), d_buffer.end(), data) != d_buffer.end();
    }

    void clear()
    {
        d_size = 0;
        d_buffer.clear();
    }

    [[nodiscard]] bool empty() const
    {
        return !d_size;
    }

    [[nodiscard]] int size() const
    {
        return d_size;
    }

    [[nodiscard]] int max_size() const
    {
        return d_max_size;
    }

    bool operator& (const T& value) const
    {
        return find(value);
    }

    void operator << (const T& value)
    {
        push_back(value);
    }

private:
    std::vector<T> d_buffer;
    int d_read_index;
    int d_write_index;
    int d_max_size;
    int d_size;
};


#endif //SITEST_RINGBUFFER_HPP
