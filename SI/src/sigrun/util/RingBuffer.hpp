

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

    [[nodiscard]] uint32_t size() const
    {
        return d_size;
    }

    [[nodiscard]] uint32_t max_size() const
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

    const std::vector<T>& buffer() const
    {
        return d_buffer;
    }

private:
    std::vector<T> d_buffer;
    uint32_t d_read_index;
    uint32_t d_write_index;
    uint32_t d_max_size;
    uint32_t d_size;
};


#endif //SITEST_RINGBUFFER_HPP
