#ifndef ABSTRACTREQUESTPARSER_HPP
#define ABSTRACTREQUESTPARSER_HPP

#include <boost/logic/tribool.hpp>

#include "abstractrequest.hpp"

namespace sp2p {
namespace tracker {
namespace protocol_factory {

class AbstractRequestParser
{
public:
    virtual boost::tribool parse(AbstractRequest *clientMessage, const char data[], int length) = 0;
    virtual int getLen() const = 0;
    virtual void setLen(int value) = 0;
    virtual ~AbstractRequestParser() = default;
private:
    int len;

};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTREQUESTPARSER_HPP
