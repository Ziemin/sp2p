#ifndef PARSER_HPP
#define PARSER_HPP

#include <type_traits>
#include <cstdint>
#include <memory>

#include <boost/asio.hpp>

#include "nodemessage.hpp"

namespace sp2p {
    namespace sercli {

        enum class parse_result {
            GOOD, BAD, INDETERMINATE
        };

        template <typename Mess, typename Enable = void>
            class Parser;

        template <typename Mess>
            class Parser <Mess, typename std::enable_if<std::is_base_of<Message, Mess>::value>::type> {

                public:
					~Parser() = default;

                    virtual void reset() = 0;

                    virtual parse_result parse(Mess& message, const char data[], int length) = 0;

            };


        class NodeResponseParser : public Parser<NodeResponse> {

            public:

				~NodeResponseParser() = default;
                void reset();
                parse_result parse(NodeResponse&, const char data[], int length);

            private:

                int message_size = -1;
                boost::asio::streambuf sb;

        };

        template <typename Mess>
            using parser_ptr = std::shared_ptr<Parser<Mess>>;

    } /* namespace sercli */
} /* namespace sp2p */

#endif /* PARSER_HPP */
