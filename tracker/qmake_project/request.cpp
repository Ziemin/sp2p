#include "request.hpp"


namespace sp2p {
namespace tracker {

Request::Request()
{
}

bool Request::parseFromIstream(const std::istream *stream) {
    return true;
}

int Request::getSize() {
    return 0;
}

} // namespace tracker
} // namespace sp2p
