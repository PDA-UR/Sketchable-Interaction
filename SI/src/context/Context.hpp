

#ifndef SI_CONTEXT_HPP
#define SI_CONTEXT_HPP


class Context
{
public:
    Context(int width, int height);
    ~Context();

    int width() const;

    void set_width(int width);

    int height() const;

    void set_height(int height);

private:
    int d_width, d_height;
protected:
};


#endif //SI_CONTEXT_HPP
