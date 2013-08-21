#include "user.hpp"

namespace sp2p {
	namespace sercli {
     	
		MyUser::MyUser(std::string username, std::string password, std::string email, bool registered) 
			: User{username, email}, password(password), is_registered(registered) { }
	}
}
