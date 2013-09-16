#ifndef USER_HPP
#define USER_HPP

#include <string>

namespace sp2p{
namespace tracker{
namespace utils{


/**
 * @brief The ProtoUser class - represents protocol user
 */
class ProtoUser
{
public:
    /**
     * @brief ProtoUser - basic constructor
     */
    ProtoUser();

    /**
     * @brief getLogin - getter for login
     * @return
     */
    std::string getLogin() const;

    /**
     * @brief setLogin - setter for login
     * @param value
     */
    void setLogin(const std::string &value);

    /**
     * @brief getPassword - getter for password
     * @return
     */
    std::string getPassword() const;

    /**
     * @brief setPassword - setter for password
     * @param value
     */
    void setPassword(const std::string &value);

private:
    std::string login;
    std::string password;
};

} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // USER_HPP
