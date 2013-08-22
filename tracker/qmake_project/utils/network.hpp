#ifndef NETWORK_HPP
#define NETWORK_HPP

namespace sp2p{
namespace tracker{
namespace utils{

class Network
{
public:
    Network();

    std::string getName() const;
    void setName(const std::string &value);

    bool getPublicity() const;
    void setPublicity(bool value);

    bool getVisible() const;
    void setVisible(bool value);

    bool getParticipable() const;
    void setParticipable(bool value);

    std::string getCreatorEmail() const;
    void setCreatorEmail(const std::string &value);

    std::string getProtocolName() const;
    void setProtocolName(const std::string &value);

    std::string getOwnerName() const;
    void setOwnerName(const std::string &value);

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
