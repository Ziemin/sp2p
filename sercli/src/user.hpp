#ifndef USER_H
#define USER_H

#include <string>

namespace sp2p {
	namespace sercli {

        /**
		 * Class representing user account on some node.
		 * It stores username and other necessary data
		 */
		class User {
			public:
				std::string username, email;
		};

		/**
		 * This class stores password for user
		 */
		class MyUser : public User {
			public:
				MyUser(std::string username, std::string password, std::string email);

			private:
				std::string password;

		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* USER_H */
