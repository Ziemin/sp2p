#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <string>
#include <iostream>

namespace sp2p{
namespace tracker{
namespace utils{


/**
 * @brief The ProtoNetwork class - represents sp2p network
 */
class ProtoNetwork
{
public:
    /**
     * @brief ProtoNetwork - basic construtor
     */
    ProtoNetwork();

    /**
     * @brief getName - getter for name
     * @return
     */
    std::string getName() const;

    /**
     * @brief setName - setter for name
     * @param value
     */
    void setName(const std::string &value);

    /**
     * @brief getPublicity - getter for publicity
     * @return
     */
    bool getPublicity() const;

    /**
     * @brief setPublicity - setter for publicity
     * @param value
     */
    void setPublicity(bool value);

    /**
     * @brief getVisible - getter for visible
     * @return
     */
    bool getVisible() const;

    /**
     * @brief setVisible - setter for visible
     * @param value
     */
    void setVisible(bool value);

    /**
     * @brief getParticipable - getter for participable
     * @return
     */
    bool getParticipable() const;

    /**
     * @brief setParticipable - setter for participable
     * @param value
     */
    void setParticipable(bool value);

    /**
     * @brief getCreatorEmail - getter for creatorEmail
     * @return
     */
    std::string getCreatorEmail() const;

    /**
     * @brief setCreatorEmail - setter for creatorEmail
     * @param value
     */
    void setCreatorEmail(const std::string &value);

    /**
     * @brief getProtocolName - getter for protocolName
     * @return
     */
    std::string getProtocolName() const;

    /**
     * @brief setProtocolName - setter for protocolName
     * @param value
     */
    void setProtocolName(const std::string &value);

    /**
     * @brief getOwnerName - getter for ownerName
     * @return
     */
    std::string getOwnerName() const;

    /**
     * @brief setOwnerName - setter for ownerName
     * @param value
     */
    void setOwnerName(const std::string &value);

    operator std::string() const;

private:
    std::string name;
    bool publicity;
    bool visible;
    bool participable;
    std::string creatorEmail;
    std::string protocolName;
    std::string ownerName;

};

} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // NETWORK_HPP
