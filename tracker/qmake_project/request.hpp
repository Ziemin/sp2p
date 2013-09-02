#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "protocol_factory/abstractrequest.hpp"

namespace sp2p {
namespace tracker {

class Request : public protocol_factory::AbstractRequest
{
public:
    Request();
    bool parseFromIstream(const std::istream *stream);
    int getSize();
};

} // namespace tracker
} // namespace sp2p

#endif // REQUEST_HPP
