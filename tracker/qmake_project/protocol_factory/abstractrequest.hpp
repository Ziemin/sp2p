#ifndef ABSTRACTREQUEST_HPP
#define ABSTRACTREQUEST_HPP

#include <iostream>

namespace sp2p {
namespace tracker {
namespace protocol_factory {

/**
 * @brief The AbstractRequest class - interface for client request
 */
class AbstractRequest
{
public:
    /**
     * @brief parseFromIstream - produces request from istream
     * @param stream
     * @return
     */
    virtual bool parseFromIstream(std::istream *stream) = 0;

    /**
     * @brief getSize - getter for message byte size
     * @return
     */
    virtual int getSize() const = 0;
    virtual ~AbstractRequest() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTREQUEST_HPP
