#include "sessioncontroler.hpp"

#include "utils/util_functions.h"


namespace sp2p {
namespace tracker {

SessionControler::SessionControler()
{
}


bool SessionControler::startDaemon(const long long refresheRate, const long long ttl)
{
    boost::thread(boost::bind(&SessionControler::daemonFunction, this, refresheRate, ttl));
    isDaemonRunning = true;
    return true;
}

bool SessionControler::stopDaemon()
{
    daemon.interrupt();
    isDaemonRunning = false;
    return true;
}

bool SessionControler::daemonRunning() const
{
    return this->isDaemonRunning;
}

bool SessionControler::isLogged(const std::string &cookie)
{
    mutex_.lock();
    bool ret = cookieToTime.count(cookie) > 0;
    mutex_.unlock();
    return ret;
}

std::string SessionControler::login(const std::string &login)
{
    mutex_.lock();
    std::string currCookie = utils::getRandomString(COOKIE_LEN);
    cookieToLogin[currCookie] = login;
    cookieToTime[currCookie] = utils::currTimeInMS();
    mutex_.unlock();
    return currCookie;
}

bool SessionControler::logout(const std::string &cookie)
{
    std::cout << "In logout with " << cookie << std::endl;
    std::cout << "in session:" << std::endl;
    for(auto i = cookieToTime.begin(); i != cookieToTime.end(); ++i) {
        std::cout << i->first << std::endl;
    }
    bool ret = false;
    mutex_.lock();
    if(cookieToTime.find(cookie) != cookieToTime.end()) {
        ret = true;
        cookieToLogin.erase(cookieToLogin.find(cookie));
        cookieToTime.erase(cookieToTime.find(cookie));
    }
    mutex_.unlock();
    return ret;
}

std::string SessionControler::getLogin(const std::string &cookie)
{
    mutex_.lock();
    std::string ret = "";
    if(cookieToTime.count(cookie) > 0) {
        ret = cookieToLogin.at(cookie);
    }
    mutex_.unlock();
    return ret;
}

void SessionControler::refresh(const std::string &cookie)
{
    mutex_.lock();
    if(cookieToTime.count(cookie) > 0) {
        cookieToTime[cookie] = utils::currTimeInMS();
    }
    mutex_.unlock();
}

void SessionControler::cleanAll()
{
    mutex_.lock();
    cookieToLogin.clear();
    cookieToTime.clear();
    mutex_.unlock();
}

void SessionControler::cleanSome(const long long ttl)
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

void SessionControler::daemonFunction(const long long refresheRate, const long long ttl) {
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
