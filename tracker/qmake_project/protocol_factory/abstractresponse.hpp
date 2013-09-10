#ifndef ABSTRACTRESPONSE_HPP
#define ABSTRACTRESPONSE_HPP

#include <iostream>

namespace sp2p {
namespace tracker {
namespace protocol_factory {

class AbstractResponse
{
public:
    virtual bool parseIntoOstream(std::ostream *stream) = 0;
    virtual int getSize() const = 0;
    virtual std::string status() = 0;
    virtual ~AbstractResponse() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTRESPONSE_HPP
