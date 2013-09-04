#include "parser.hpp"
#include "globals.hpp"

namespace sp2p {
    namespace sercli {


        void NodeResponseParser::reset() {

            sb.consume(sb.size());
            message_size = -1;
        }

        parse_result NodeResponseParser::parse(NodeResponse& message, const char data[], int length) {

            parse_result  result;
            std::ostream os(&sb);

            os.write(data, length);
            if(sb.size() > global::max_buffer_size) {
                reset();
                return parse_result::BAD;
            }

            if(message_size == -1 && sb.size() >= sizeof(std::uint32_t)) {
                std::istream is(&sb);
                std::uint32_t len;
				is.read(reinterpret_cast<char*>(&len), sizeof(len));
                message_size = (int) len;
            }

            if(message_size <= (int) sb.size() && message_size != -1) {

                std::istream is(&sb);
                if(message.ParseFromIstream(&is)) {
                    result = parse_result::GOOD;
                    message_size = -1;
                }
                else {
                    reset();
                    result = parse_result::BAD;
                }

            }
            else result = parse_result::INDETERMINATE;

            return result;
        }

    } /* namespace sercli */
} /* namespace sp2p */
