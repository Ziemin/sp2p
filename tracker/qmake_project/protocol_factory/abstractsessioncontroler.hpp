#ifndef ABSTRACTSESSIONCONTROLER_HPP
#define ABSTRACTSESSIONCONTROLER_HPP

#include <string>

namespace sp2p {
namespace tracker {
namespace protocol_factory {


/**
 * @brief The AbstractSessionControler class - interface for session controller
 */
class AbstractSessionControler
{
public:
    virtual ~AbstractSessionControler() = default;

    /**
     * @brief startDaemon - starts daemon automaticly logging out
     * @param refresheRate - how often daemon wakes up
     * @param ttl - how long user can be logged
     * @return
     */
    virtual bool startDaemon(const long long refresheRate, const long long ttl) = 0;

    /**
     * @brief stopDaemon - stops daemon
     * @return
     */
    virtual bool stopDaemon() = 0;

    /**
     * @brief daemonRunning
     * @return true if running
     */
    virtual bool daemonRunning() const = 0;

    /**
     * @brief isLogged
     * @param cookie
     * @return true if user with cookie is logged
     */
    virtual bool isLogged(const std::string &cookie) = 0;

    /**
     * @brief login - login user
     * @param login
     * @return cookie
     */
    virtual std::string login(const std::string &login) = 0;

    /**
     * @brief logout - logout user
     * @param cookie
     * @return true if existed
     */
    virtual bool logout(const std::string &cookie) = 0;

    /**
     * @brief getLogin - getter for login from cookie
     * @param cookie
     * @return user login
     */
    virtual std::string getLogin(const std::string &cookie) = 0;

    /**
     * @brief refresh - renews user ttl
     * @param cookie
     */
    virtual void refresh(const std::string &cookie) = 0;

    /**
     * @brief cleanAll - removes all users
     */
    virtual void cleanAll() = 0;

    /**
     * @brief cleanSome - cleans logins older then ttl
     * @param ttl
     */
    virtual void cleanSome(const long long ttl) = 0;
};

} // namespace protocol_factory
} // namespace tracker
} // namespace sp2p

#endif // ABSTRACTSESSIONCONTROLER_HPP
