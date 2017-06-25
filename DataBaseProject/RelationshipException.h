#ifndef RELATIONSHIP_EXCEPTION
#define RELATIONSHIP_EXCEPTION

#include <exception>
#include <string>

using std::string;
using std::exception;

namespace db
{
	class RelationshipException : public exception
	{
	public:

		const char * what() const throw ()
		{
			return msg.c_str();
		}

		RelationshipException(string _msg = "") : msg(_msg) {}

	private:

		string msg;
	};
}
#endif // !RELATIONSHIP_EXCEPTION
