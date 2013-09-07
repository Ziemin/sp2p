#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <pqxx/result>
#include <boost/asio/streambuf.hpp>

namespace sp2p{
namespace tracker{
namespace utils{

std::string getRandomString(int len);

long long currTimeInMS();

template <std::size_t N>
std::int64_t getNumFromBuff(boost::array<char, N> &buff, int numSize)
{
    std::int64_t ret = 0;
    int tmp = 1;
    for(int i = 0; i < numSize; ++i) {
        char currChar = buff.at(i);
        ret += tmp * ((unsigned int)(unsigned char)currChar);
        tmp <<= 8;
    }
    return ret;
}

std::string toString(pqxx::tuple::reference);
bool toBool(pqxx::tuple::reference);
int toInt(pqxx::tuple::reference);


} // namespace utils
} // namespace tracker
} // namespace sp2p

#endif // UTILS_H
