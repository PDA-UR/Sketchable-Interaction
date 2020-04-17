
#ifndef SI_SIGRUN_LinkCandidate
#define SI_SIGRUN_LinkCandidate

#include <sigrun/SIObject.hpp>

class LinkCandidate: public SIObject
{ SIGRUN
public:
    LinkCandidate(const std::string& _sender, const std::string& _sender_attrib, const std::string& _recv, const std::string& _recv_attrib):
            sender(_sender),
            sender_attrib(_sender_attrib),
            recv(_recv),
            recv_attrib(_recv_attrib) {}

    const bool operator ==(const LinkCandidate& other) const
    {
        return sender == other.sender && sender_attrib == other.sender_attrib && recv == other.recv && recv_attrib == other.recv_attrib;
    }

    const bool operator !=(const LinkCandidate& other) const
    {
        return !(*this == other);
    }

    std::string sender;
    std::string sender_attrib;
    std::string recv;
    std::string recv_attrib;
};

#endif // SI_SIGRUN_LinkCandidate