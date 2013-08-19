#ifndef USER_H
#define USER_H

#include <string>

namespace sp2p {
	namespace sercli {

		/**
		 * Class representing user account on some node.
		 * It stores username and other necessary data
		 */
		struct User {

			std::string username, email;
		};

		/**
		 * This class stores password for user
		 */
		struct MyUser : public User {

			MyUser(std::string username, std::string password, std::string email, bool registered=false);

			std::string password;
			bool is_registered;
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* USER_H */
