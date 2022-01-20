

#ifndef SITEST_IPHYSICALENVIRONMENT_HPP
#define SITEST_IPHYSICALENVIRONMENT_HPP

class IPhysicalEnvironment {
public:
    virtual void start(int argc, char** argv) = 0;
    virtual void stop() = 0;
};

#endif //SITEST_IPHYSICALENVIRONMENT_HPP
