#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace boost::log;

namespace sp2p {
	namespace sercli {
		namespace logging {

            typedef src::severity_logger_mt<trivial::severity_level> Logger;

			BOOST_LOG_GLOBAL_LOGGER(sp2p_lg, src::severity_logger_mt<trivial::severity_level>);

			void init();

		}
	}
}

#endif
