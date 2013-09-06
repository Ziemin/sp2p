#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/base_object.hpp>

namespace sp2p {
	namespace sercli {

		/**
		 * Class representing user account on some node.
		 * It stores username and other necessary data
		 */
		struct User {

			std::string username, email;
			// TODO public_key
			private:

				friend std::ostream& operator<<(std::ostream& os, const User& user);

				friend class boost::serialization::access;

				template<class Archive> void serialize(Archive& ar, const unsigned int /* version */) {
					ar & username & email;
				}

		};

		/**
		 * This class stores password for user
		 */
		struct MyUser : public User {

			MyUser() = default;
			MyUser(std::string username, std::string password, std::string email, bool registered=false);

			std::string password;
			bool is_registered;

			private:

				friend std::ostream& operator<<(std::ostream& os, const MyUser& user);

				friend class boost::serialization::access;

				template<class Archive> void serialize(Archive& ar, const unsigned int /* version */) {
					ar & boost::serialization::base_object<User>(*this);
					ar & password & is_registered;
				}
		};

	} /* namespace sercli */
} /* namespace sp2p */

#endif /* USER_H */
