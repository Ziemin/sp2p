#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <memory>

#include "connection.hpp"
#include "message.hpp"
#include "nodemessage.hpp"
#include "logging.hpp"

namespace sp2p {
    namespace sercli {


        /**
         * Class whose purpose is to control application logic during communication process.
         * It is meant to be used inside Connection class
         */
        template <typename Request, typename Response> 
            class Handler {
                static_assert(std::is_base_of<Message, Request>::value, "Request is not type of Message");
                static_assert(std::is_base_of<Message, Response>::value, "Response is not type of Message");

                public:
                    virtual ~Handler() = default;

                    virtual void handleResponse(Connection<Request, Response>& connection, Response& response) = 0;
                    virtual void handleSendError(Connection<Request, Response>& connection, Request& request) = 0;
                    virtual void handleReadError(Connection<Request, Response>& connection) = 0;
                    virtual void handleParseError(Connection<Request, Response>& connection) = 0;

            };


        /**
         * Template specialization class to be used when exchanging information with Nodes
         */
        template <typename Request, typename Response>
            using handler_ptr = std::shared_ptr<Handler<Request, Response>>;

            class Sp2pHandler : public Handler<NodeRequest, NodeResponse> {

                public:
                    ~Sp2pHandler() = default;

                    void handleResponse(Connection<NodeRequest, NodeResponse>& connection,
                            NodeResponse& response);

                    virtual void handleSendError(Connection<NodeRequest, NodeResponse>& connection, 
                            NodeRequest& request);

                    virtual void handleReadError(Connection<NodeRequest, NodeResponse>& connection);

                    virtual void handleParseError(Connection<NodeRequest, NodeResponse>& connection);

                private:

                    logging::Logger lg = logging::sp2p_lg::get();

            };

    } /* namespace sercli */
} /* namespace sp2p */

#endif /* HANDLER_HPP */
