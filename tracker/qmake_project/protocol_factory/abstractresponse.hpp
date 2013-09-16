#ifndef ABSTRACTRESPONSE_HPP
#define ABSTRACTRESPONSE_HPP

#include <iostream>

namespace sp2p {
namespace tracker {
namespace protocol_factory {


/**
 * @brief The AbstractResponse class - interface for server response
 */
class AbstractResponse
{
public:
    /**
     * @brief parseIntoOstream - parses response into ostream
     * @param stream
     * @return
     */
    virtual bool parseIntoOstream(std::ostream *stream) = 0;

    /**
     * @brief getSize - getter for message size
     * @return - message size
     */
    virtual int getSize() const = 0;

    /**
     * @brief status - returns message status as a string
     * @return
     */
    virtual std::string status() = 0;
    virtual ~AbstractResponse() = default;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTRESPONSE_HPP
