#define BOOST_TEST_MODULE manager_test

#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>
#include "../src/sp2p.hpp"

using namespace sp2p::sercli;

struct InitDestroyTest {
    InitDestroyTest() : init(5) { }
    global::Sp2pInitializer init;
};


BOOST_FIXTURE_TEST_SUITE(manager_test, InitDestroyTest)

    BOOST_AUTO_TEST_CASE(simple_manager_init) {
        BOOST_CHECK(global::io_s != nullptr);
        BOOST_CHECK(global::sp2p_handler != nullptr);

        DataManager data_manager;
        data_manager.setDataFile("manager_data_file");
        Manager manager(data_manager);
    }

    BOOST_AUTO_TEST_CASE(instantiotion_and_serialization) {
        DataManager data_manager;
        data_manager.setDataFile("manager_data_file");
        Manager manager(data_manager);

        BOOST_TEST_MESSAGE("Creating networks");
        types::NetworkDescription net_desc;
        net_desc.access_rights = types::NetworkDescription::AccessRights::PRIVATE;
        net_desc.visability = types::NetworkDescription::Visibility::LISTED;
        net_desc.participation_rights = types::NetworkDescription::ParticipationRights::CLIENT_ONLY;
        net_desc.network_name = "network1";
        net_desc.protocol_name = "protocol1";
        net_desc.creator = User();
        network_ptr net1 = manager.createNetwork(net_desc);
        net_desc.network_name = "network2";
        net_desc.protocol_name = "protocol2";
        net_desc.creator = User();
        network_ptr net2 = manager.createNetwork(net_desc);

        BOOST_TEST_MESSAGE("Creating nodes");
        types::NodeDescription node_desc;
        node_desc.node_name = "node1";
        node_desc.port = 10;
        node_desc.ip_address = boost::asio::ip::address::from_string("127.0.0.1");
        node_ptr node1 = manager.createNode(node_desc);
        node_desc.node_name = "node2";
        node_desc.port = 12;
        node_desc.ip_address = boost::asio::ip::address::from_string("127.0.0.2");
        node_ptr node2 = manager.createNode(node_desc);

        BOOST_TEST_MESSAGE("Associating nodes with networks");
        net2->associateNode(node2);
        net1->associateNode(node1);

        BOOST_TEST_MESSAGE("Manager initialized");

        manager.saveState();
        BOOST_TEST_MESSAGE("Manager -> saved state");

        manager.loadState();
        BOOST_TEST_MESSAGE("Manager -> loaded state");

    }


BOOST_AUTO_TEST_SUITE_END ( )

