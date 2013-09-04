#ifndef PARSER_FACTORY_HPP
#define PARSER_FACTORY_HPP

#include "parser.hpp"
#include "nodemessage.hpp"

namespace sp2p {
    namespace sercli {

        template<typename Message>
            class ParserFactory {

                public:

                    ~ParserFactory() = default;
                    virtual parser_ptr<Message> getParser() = 0;
            };

        class NodeParserFactory : ParserFactory<NodeResponse> {

            public:
                parser_ptr<NodeResponse> getParser();
        };
    }
}

#endif
