#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include "types.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractrequestparser.hpp"

namespace sp2p {
namespace tracker {


/// Parser for incoming requests.
class RequestParser : public protocol_factory::AbstractRequestParser
{
public:
    RequestParser();
    RequestParser(int len);
    boost::tribool parse(protocol_factory::AbstractRequest *clientMessage, const char data[], int length);
    int getLen() const;
    void setLen(int value);

private:
    int len;

};

} // namespace tracker
} // namespace sp2p

#endif // REQUESTPARSER_H
