

#ifndef SITEST_IROS2ENVIRONMENT_HPP
#define SITEST_IROS2ENVIRONMENT_HPP

class IROS2Environment {
public:
    virtual void start(int argc, char** argv) = 0;
    virtual void stop() = 0;
};

#endif //SITEST_IROS2ENVIRONMENT_HPP
