#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <boost/asio/buffer.hpp>
#include "protocol_factory/abstractresponse.hpp"

namespace sp2p {
namespace tracker {

class Response : public protocol_factory::AbstractResponse
{
public:
    Response();
    bool parseIntoOstream(std::ostream *stream) const;
    int getSize();
};

} // namespace tracker
} // namespace sp2p

#endif // RESPONSE_HPP
