#include "data.hpp"

#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace sp2p {
	namespace sercli {

        namespace serialization {

            // BasicSink
            BasicSink::BasicSink(const std::string& dataFile)
            : os(dataFile), oa(os) { }

            BasicSink& BasicSink::operator<<(const Node& node) { 
                oa << node; 
                return *this;
            }

            BasicSink& BasicSink::operator<<(const Network& network) {
               oa << network; 
               return *this;
            }

            BasicSink& BasicSink::operator<<(const std::vector<types::NodeDescription>& nodes) {
                oa << nodes;
                return *this;
            }

            BasicSink& BasicSink::operator<<(const std::vector<types::NetworkDescription>& networks) {
                oa << networks;
                return *this;
            }

            BasicSink& BasicSink::operator<<(const std::string& text) {
                oa << text; 
                return *this;
            }

            BasicSink::~BasicSink() { }

            // BasicSource
            BasicSource::BasicSource(const std::string& dataFile) 
                : is(dataFile), ia(is) { }

            BasicSource& BasicSource::operator>>(Node& node) {
                ia >> node; 
                return *this;
            }


            BasicSource& BasicSource::operator>>(Network& network) {
                ia >> network; 
                return *this;
            }

            BasicSource& BasicSource::operator>>(std::vector<types::NodeDescription>& nodes) {
                ia >> nodes;
                return *this;
            }
            BasicSource& BasicSource::operator>>(std::vector<types::NetworkDescription>& networks) {
                ia >> networks;
                return *this;
            }
            BasicSource& BasicSource::operator>>(std::string& text) { 
                ia >> text; 
                return *this;
            }

            BasicSource::~BasicSource() { }

            // DataException
            DataException::DataException(std::string message) : message(std::move(message)) { }

            const char* DataException::what() const throw() {
                return message.c_str();
            }


        } /* namespace serialization */

        // DataManger
        DataManager::~DataManager() { }
        
        std::shared_ptr<serialization::AbstractSink> DataManager::getSink() {
            if(dataFile.size() == 0) throw serialization::DataException("no file given");

            return std::shared_ptr<serialization::AbstractSink>(new serialization::BasicSink(dataFile));
        }
        std::shared_ptr<serialization::AbstractSource> DataManager::getSource() {
            if(dataFile.size() == 0) throw serialization::DataException("no file given");

            return std::shared_ptr<serialization::AbstractSource>(new serialization::BasicSource(dataFile));
        }

        void DataManager::setDataFile(std::string dataFile) {
            this->dataFile = std::move(dataFile);
        }

    } /* namespace sercli */
} /* namespace sp2p */
