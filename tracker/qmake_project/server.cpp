#include "server.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <iostream>

#include "constants.h"

namespace sp2p {
namespace tracker {

Server::Server(Factory_ptr factory, const std::string& address,
               const std::string& port, std::size_t thread_pool_size)
    : thread_pool_size_(thread_pool_size),
      acceptor_(io_service_),
      new_connection_(),
      context_(io_service_, boost::asio::ssl::context::tlsv12_server)
{
    context_.set_options(
                boost::asio::ssl::context::default_workarounds
                | boost::asio::ssl::context::no_sslv2
                | boost::asio::ssl::context::no_sslv3
                | boost::asio::ssl::context::single_dh_use);
    //    context_.use_certificate_chain_file("server.crt");
    //    context_.use_private_key_file("server.key", boost::asio::ssl::context::pem);
    //    context_.use_tmp_dh_file("dh512.pem");
    context_.use_certificate_chain_file(consts::CERT_PATH);
    context_.use_private_key_file(consts::PRIV_KEY_PATH, boost::asio::ssl::context::pem);
    context_.use_tmp_dh_file(consts::DH_FILE_PATH);
    protocolFactory = factory;
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

void Server::run()
{
    // Create a pool of threads to run all of the io_services.
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i)
    {
        boost::shared_ptr<boost::thread> thread(new boost::thread(
                                                    boost::bind(&boost::asio::io_service::run,
                                                                &io_service_)));
        threads.push_back(thread);
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

void Server::start_accept()
{
    new_connection_.reset(new sp2p::tracker::Connection(io_service_, context_, protocolFactory));
    acceptor_.async_accept(new_connection_->socket(),
                           boost::bind(&sp2p::tracker::Server::handle_accept, this,
                                       boost::asio::placeholders::error));
}

void Server::handle_accept(const boost::system::error_code& e)
{
    if (!e)
    {
        new_connection_->start();
    }

    start_accept();
}

std::string Server::getPassword()
{
    return "asdasd";
}

} // namespace tracker
} // namespace sp2p
