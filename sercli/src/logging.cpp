#include "logging.hpp"

#include <fstream>
#include <iomanip>
#include <boost/shared_ptr.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/attributes/function.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <atomic>
#include <thread>


using namespace boost::log;

namespace sp2p {
	namespace sercli {
		namespace logging {

            namespace logging = boost::log;
            namespace src = boost::log::sources;
            namespace sinks = boost::log::sinks;
            namespace keywords = boost::log::keywords;
            namespace expr = boost::log::expressions;
            namespace attrs = boost::log::attributes;

            typedef sinks::synchronous_sink< sinks::text_file_backend > sink_t;
            typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;

            std::atomic_bool is_initialized(false);


			BOOST_LOG_GLOBAL_LOGGER_INIT(sp2p_lg, src::severity_logger_mt<trivial::severity_level>) {
                src::severity_logger_mt<trivial::severity_level> lg;

                lg.add_attribute("LineID", attrs::counter<unsigned int>(1));
                lg.add_attribute("TimeStamp", attrs::local_clock());
                lg.add_attribute("ThreadID", attrs::make_function([]() -> std::thread::id { return std::this_thread::get_id(); }));

                return lg;
            }

            void init() {
                if(!is_initialized) {

                    logging::core::get()->set_filter
                        (
                         logging::trivial::severity >= logging::trivial::trace
                        );

                    logging::add_file_log 
                        (
                         keywords::file_name = "sp2p_%N.log",
                         keywords::rotation_size = 10 * 1024 * 1024,
                         keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
                         keywords::format = 
                         (
                          expr::stream  
                                << expr::attr<unsigned int>("LineID") << " "
                                << "[" << expr::attr<boost::posix_time::ptime>("TimeStamp") << "] "
                                << "(" << std::hex << std::setw(8) << std::setfill('0') << expr::attr<std::thread::id>("ThreadID") << ") "
                                << "<" << expr::attr<trivial::severity_level>("Severity") << ">: "
                                << expr::smessage
                         )
                        );

                    is_initialized = true;
                }
            }

        }
    }
}

