#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

namespace sp2p{
namespace tracker{
namespace utils{


/**
 * @brief The ProtoServer class - represents protocol server
 */
class ProtoServer
{
public:
    /**
     * @brief ProtoServer - basic constructor
     */
    ProtoServer();

    /**
     * @brief getUserLogin - getter for userLogin
     * @return
     */
    std::string getUserLogin() const;

    /**
     * @brief setUserLogin - setter for userLogin
     * @param value
     */
    void setUserLogin(const std::string &value);

    /**
     * @brief getNetworkName - getter for networkName
     * @return
     */
    std::string getNetworkName() const;

    /**
     * @brief setNetworkName - setter for networkName
     * @param value
     */
    void setNetworkName(const std::string &value);

    /**
     * @brief getIp - getter for ip
     * @return
     */
    std::string getIp() const;

    /**
     * @brief setIp - setter for ip
     * @param value
     */
    void setIp(const std::string &value);

    /**
     * @brief getPort - getter for port
     * @return
     */
    int getPort() const;

    /**
     * @brief setPort - setter for port
     * @param value
     */
    void setPort(int value);

    /**
     * @brief getTtl - getter for ttl
     * @return
     */
    long long getTtl() const;

    /**
     * @brief setTtl - setter for ttl
     * @param value
     */
    void setTtl(long long value);

private:
    std::string userLogin;
    std::string networkName;
    std::string ip;
    int port;
    long long ttl;
};

} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // SERVER_HPP
