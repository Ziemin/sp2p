#ifndef LOGINCONTROLER_HPP
#define LOGINCONTROLER_HPP

#include <string>
#include <map>
#include <utility>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

#include "protocol_factory/abstractlogincontroler.hpp"

namespace sp2p {
namespace tracker {

class LoginControler : public protocol_factory::AbstractLoginControler
{
public:
    LoginControler();

public:
    bool startDaemon(const long long refresheRate, const long long ttl);
    bool stopDaemon();
    bool daemonRunning() const;
    bool isLogged(const std::string &cookie);
    std::string login(const std::string &login);
    std::string getLogin(const std::string &cookie);
    void refresh(const std::string &cookie);
    void cleanAll();
    void cleanSome(const long long ttl);

private:
    static const int COOKIE_LEN = 15;

    std::map<std::string, std::string> cookieToLogin;
    std::map<std::string, long long> cookieToTime;

    boost::mutex mutex_;
    boost::thread daemon;
    bool isDaemonRunning;
    void daemonFunction(const long long refresheRate, const long long ttl);
};

} // namespace tracker
} // namespace sp2p

#endif // LOGINCONTROLER_HPP
