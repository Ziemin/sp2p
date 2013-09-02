#include "logincontroler.hpp"

#include "utils/util_functions.h"


namespace sp2p {
namespace tracker {

LoginControler::LoginControler()
{
}


bool LoginControler::startDaemon(const long long refresheRate, const long long ttl)
{
    boost::thread(boost::bind(&LoginControler::daemonFunction, this, refresheRate, ttl));
    isDaemonRunning = true;
    return true;
}

bool LoginControler::stopDaemon()
{
    daemon.interrupt();
    isDaemonRunning = false;
    return true;
}

bool LoginControler::daemonRunning() const
{
    return this->isDaemonRunning;
}

bool LoginControler::isLogged(const std::string &cookie)
{
    mutex_.lock();
    bool ret = cookieToTime.count(cookie) > 0;
    mutex_.unlock();
    return ret;
}

std::string LoginControler::login(const std::string &login)
{
    mutex_.lock();
    std::string currCookie = utils::getRandomString(COOKIE_LEN);
    cookieToLogin[currCookie] = login;
    cookieToTime[currCookie] = utils::currTimeInMS();
    mutex_.unlock();
    return currCookie;
}

std::string LoginControler::getLogin(const std::string &cookie)
{
    mutex_.lock();
    std::string ret = "";
    if(cookieToTime.count(cookie) > 0) {
        ret = cookieToLogin.at(cookie);
    }
    mutex_.unlock();
    return ret;
}

void LoginControler::refresh(const std::string &cookie)
{
    mutex_.lock();
    if(cookieToTime.count(cookie) > 0) {
        cookieToTime[cookie] = utils::currTimeInMS();
    }
    mutex_.unlock();
}

void LoginControler::cleanAll()
{
    mutex_.lock();
    cookieToLogin.clear();
    cookieToTime.clear();
    mutex_.unlock();
}

void LoginControler::cleanSome(const long long ttl)
{
    mutex_.lock();
    for(auto it = cookieToTime.begin(); it != cookieToTime.end(); ++it) {
        if(it->second > ttl) {
            cookieToTime.erase(it);
            if(cookieToLogin.find(it->first) != cookieToLogin.end())
                cookieToLogin.erase(cookieToLogin.find(it->first));
        }
    }
    mutex_.unlock();
}

void LoginControler::daemonFunction(const long long refresheRate, const long long ttl) {
    while(true) {
        try {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(refresheRate));
            cleanSome(utils::currTimeInMS() - ttl);
        }
        catch(boost::thread_interrupted) {
            cleanSome(utils::currTimeInMS() - ttl);
            return;
        }
    }
}

} // namespace tracker
} // namespace sp2p
