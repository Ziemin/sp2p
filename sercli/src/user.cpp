#include "user.hpp"

namespace sp2p {
	namespace sercli {
     	
		MyUser::MyUser(std::string username, std::string password, std::string email, bool registered) 
			: User{username, email}, password(password), is_registered(registered) { }


		std::ostream& operator<<(std::ostream& os, const User& user) {
            os << "User -> username: " << user.username << ", email: " << user.email;
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const MyUser& user) {
            os << "MyUser -> username: " << user.username << ", email: " << user.email
				<< ", password: " << user.password << ", registered: " << user.is_registered;
			return os;
		}
	}
}
