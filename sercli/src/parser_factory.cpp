
#include "parser_factory.hpp"

namespace sp2p {
    namespace sercli {

        parser_ptr<NodeResponse> NodeParserFactory::getParser() {

            return parser_ptr<NodeResponse>(new NodeResponseParser());
        }
    }
}

