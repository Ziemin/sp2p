#ifndef INVITATION_HPP
#define INVITATION_HPP

#include <string>

namespace sp2p{
namespace tracker{
namespace utils{


/**
 * @brief The ProtoInvitation class - represents invitation to network
 */
class ProtoInvitation
{
public:
    /**
     * @brief ProtoInvitation - basic constructor
     */
    ProtoInvitation();

    /**
     * @brief getUserLogin - getter for user login
     * @return
     */
    std::string getUserLogin() const;

    /**
     * @brief setUserLogin - setter for user login
     * @param value
     */
    void setUserLogin(const std::string &value);

    /**
     * @brief getNetworkName - getter for network name
     * @return
     */
    std::string getNetworkName() const;

    /**
     * @brief setNetworkName - setter for network name
     * @param value
     */
    void setNetworkName(const std::string &value);

private:
    std::string userLogin;
    std::string networkName;
};

} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // INVITATION_HPP
