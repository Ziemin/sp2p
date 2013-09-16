#ifndef REQUESTPARSER_H
#define REQUESTPARSER_H

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include "types.hpp"
#include "protocol_factory/abstractrequest.hpp"
#include "protocol_factory/abstractrequestparser.hpp"

namespace sp2p {
namespace tracker {


/**
 * @brief The RequestParser class - implementation od AbstractRequestParser for sp2p protocol
 */
class RequestParser : public protocol_factory::AbstractRequestParser
{
public:
    RequestParser();
    RequestParser(std::int32_t len);
    boost::tribool parse(protocol_factory::AbstractRequest *clientMessage, const char data[], int length);
    int getLen() const;
    void setLen(int value);

private:
    std::int32_t len;

};

} // namespace tracker
} // namespace sp2p

#endif // REQUESTPARSER_H
