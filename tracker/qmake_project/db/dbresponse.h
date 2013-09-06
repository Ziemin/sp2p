#ifndef BDRESPONSE_H
#define BDRESPONSE_H

namespace sp2p {
namespace tracker {
namespace db {

enum DB_Response {
    OK,
    INTERNAL_ERROR,
    NOT_UNIQUE,
    NOT_FOUND
};

} // namespace db
} // namespace tracker
} // namespace sp2p

#endif // BDRESPONSE_H

