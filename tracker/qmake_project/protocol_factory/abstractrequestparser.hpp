#ifndef ABSTRACTREQUESTPARSER_HPP
#define ABSTRACTREQUESTPARSER_HPP

#include <boost/logic/tribool.hpp>

#include "abstractrequest.hpp"

namespace sp2p {
namespace tracker {
namespace protocol_factory {


/**
 * @brief The AbstractRequestParser class - interface for request parser
 */
class AbstractRequestParser
{
public:
    /**
     * @brief parse - parses given message
     * @param clientMessage - pointer to save parsed message
     * @param data - array of bytes read from client
     * @param length - length of the array
     * @return true if parsed correctly, false if request is corrupted, indefined if not all bytes read
     */
    virtual boost::tribool parse(AbstractRequest *clientMessage, const char data[], int length) = 0;

    /**
     * @brief getLen - getter for message length
     * @return message length
     */
    virtual int getLen() const = 0;

    /**
     * @brief setLen - setter for message length
     * @param value
     */
    virtual void setLen(int value) = 0;
    virtual ~AbstractRequestParser() = default;
private:
    int len;

};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTREQUESTPARSER_HPP
