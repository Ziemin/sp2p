#include "user.hpp"

namespace sp2p {
	namespace sercli {
     	
		MyUser::MyUser(std::string username, std::string password, std::string email) 
			: User{username, email}, password(password) { }
	}
}
