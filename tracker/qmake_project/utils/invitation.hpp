#ifndef INVITATION_HPP
#define INVITATION_HPP

#include <string>

namespace sp2p{
namespace tracker{
namespace utils{

class Invitation
{
public:
    Invitation();


    std::string getUserLogin() const;
    void setUserLogin(const std::string &value);

    std::string getNetworkName() const;
    void setNetworkName(const std::string &value);

private:
    std::string userLogin;
    std::string networkName;
};

} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // INVITATION_HPP
