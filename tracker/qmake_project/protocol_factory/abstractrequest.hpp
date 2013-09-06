#ifndef ABSTRACTREQUEST_HPP
#define ABSTRACTREQUEST_HPP

#include <iostream>

namespace sp2p {
namespace tracker {
namespace protocol_factory {


class AbstractRequest
{
public:
    virtual bool parseFromIstream(std::istream *stream) = 0;
    virtual int getSize() const = 0;
    virtual ~AbstractRequest() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTREQUEST_HPP
