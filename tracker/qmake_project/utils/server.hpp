#ifndef SERVER_HPP
#define SERVER_HPP

namespace sp2p{
namespace tracker{
namespace utils{

class Server
{
public:
    Server();

    std::string getUserLogin() const;
    void setUserLogin(const std::string &value);

    std::string getNetworkName() const;
    void setNetworkName(const std::string &value);

    std::string getIp() const;
    void setIp(const std::string &value);

    int getPort() const;
    void setPort(int value);

    long long getTtl() const;
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
