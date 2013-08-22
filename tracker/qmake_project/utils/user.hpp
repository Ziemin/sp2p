#ifndef USER_HPP
#define USER_HPP

#include <string>

namespace sp2p{
namespace tracker{
namespace utils{

class User
{
public:
    User();

    std::string getLogin() const;
    void setLogin(const std::string &value);

    std::string getPassword() const;
    void setPassword(const std::string &value);

private:
    std::string login;
    std::string password;
};

} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // USER_HPP
