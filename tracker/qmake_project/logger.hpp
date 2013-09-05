#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/empty_deleter.hpp>

namespace sp2p {
namespace tracker {
namespace logger {


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

enum severity_level
{
    normal,
    warning,
    error
};


// Complete sink type
typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
void init_logging();

} // namespace logger
} // namespace tracker
} // namespace sp2p
#endif // LOGGER_HPP
