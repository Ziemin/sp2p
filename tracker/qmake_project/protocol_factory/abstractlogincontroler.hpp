#ifndef ABSTRACTLOGINCONTROLER_HPP
#define ABSTRACTLOGINCONTROLER_HPP

#include <string>

namespace sp2p {
namespace tracker {
namespace protocol_factory {

class AbstractLoginControler
{
public:
    virtual ~AbstractLoginControler() = default;
    virtual bool startDaemon(const long long refresheRate, const long long ttl) = 0;
    virtual bool stopDaemon() = 0;
    virtual bool daemonRunning() const = 0;
    virtual bool isLogged(const std::string &cookie) = 0;
    virtual std::string login(const std::string &login) = 0;
    virtual std::string getLogin(const std::string &cookie) = 0;
    virtual void refresh(const std::string &cookie) = 0;
    virtual void cleanAll() = 0;
    virtual void cleanSome(const long long ttl) = 0;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTLOGINCONTROLER_HPP
