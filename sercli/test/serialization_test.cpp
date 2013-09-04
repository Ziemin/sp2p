#define BOOST_TEST_MODULE serialization_test
#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

BOOST_AUTO_TEST_SUITE(simple_serialization)

    BOOST_AUTO_TEST_CASE(node_desc) {
        types::NodeDescription old_desc;
        old_desc.node_name = "node";
        old_desc.port = 58;
        old_desc.ip_address = boost::asio::ip::address::from_string("127.0.0.1");

        {
            std::ofstream of("node_desc_ser");
            boost::archive::text_oarchive oa(of);
            oa << (const types::NodeDescription) old_desc;
        }

        types::NodeDescription new_desc;
        {
            std::ifstream is("node_desc_ser");
            boost::archive::text_iarchive ia(is);
            ia >> new_desc;
        }

        BOOST_CHECK(old_desc.ip_address == new_desc.ip_address);
        BOOST_CHECK(old_desc.port == new_desc.port);
        BOOST_CHECK(old_desc.node_name == new_desc.node_name);
    }

    BOOST_AUTO_TEST_CASE(network_desc) {
        types::NetworkDescription old_desc;
        old_desc.protocol_name = "protocol";
        old_desc.network_name = "some name";
        old_desc.creator = MyUser("bla", "bka", "mail");
        old_desc.access_rights = types::NetworkDescription::AccessRights::PRIVATE;
        old_desc.participation_rights = types::NetworkDescription::ParticipationRights::CLIENT_ONLY;
        old_desc.visability = types::NetworkDescription::Visibility::LISTED;
        {
            std::ofstream of("network_desc_ser");
            boost::archive::text_oarchive oa(of);
            oa << (const types::NetworkDescription) old_desc;
        }

        types::NetworkDescription new_desc;
        {
            std::ifstream is("network_desc_ser");
            boost::archive::text_iarchive ia(is);
            ia >> new_desc;
        }

        BOOST_CHECK(old_desc.protocol_name == new_desc.protocol_name);
        BOOST_CHECK(old_desc.creator.username == new_desc.creator.username);
        BOOST_CHECK(old_desc.network_name == new_desc.network_name);
        BOOST_CHECK(old_desc.access_rights == new_desc.access_rights);
        BOOST_CHECK(old_desc.participation_rights == new_desc.participation_rights);
        BOOST_CHECK(old_desc.visability == new_desc.visability);

    }

    BOOST_AUTO_TEST_CASE(server_desc) {
        types::ServerDescription old_desc;
        old_desc.related_network="net";
        old_desc.related_user = "user";
        old_desc.ip_addres = boost::asio::ip::address::from_string("127.0.0.1");

        {
            std::ofstream of("server_desc_ser");
            boost::archive::text_oarchive oa(of);
            oa << (const types::ServerDescription) old_desc;
        }

        types::ServerDescription new_desc;
        {
            std::ifstream is("server_desc_ser");
            boost::archive::text_iarchive ia(is);
            ia >> new_desc;
        }

        BOOST_CHECK(old_desc.ip_addres == new_desc.ip_addres);
        BOOST_CHECK(old_desc.related_user == new_desc.related_user);
        BOOST_CHECK(old_desc.related_network == new_desc.related_network);
    }

    BOOST_AUTO_TEST_CASE(users) {
        MyUser user("username", "password", "email", true);
        {
            std::ofstream of("user_ser");
            boost::archive::text_oarchive oa(of);
            oa << user;
        }

        MyUser new_user;
        {
            std::ifstream is("user_ser");
            boost::archive::text_iarchive ia(is);
            ia >> new_user;
        }

        BOOST_CHECK(user.email == new_user.email);
        BOOST_CHECK(user.username == new_user.username);
        BOOST_CHECK(user.password == new_user.password);
        BOOST_CHECK(user.is_registered == new_user.is_registered);
    }

BOOST_AUTO_TEST_SUITE_END ( )
