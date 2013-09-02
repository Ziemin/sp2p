#ifndef PARSER_FACTORY_HPP
#define PARSER_FACTORY_HPP

#include "parser.hpp"

namespace sp2p {
    namespace sercli {

        template<typename Message>
            class ParserFactory {

                private:

                    ParserFactory();

                public:

                    virtual ParserFactory& getInstance() const;
                    virtual parser_ptr<Message> getParser();
            };
    }
}

#endif
