#ifndef INCONSISTENT_TYPES_EXCEPTION
#define INCONSISTENT_TYPES_EXCEPTION

#include <exception>
#include <string>

using std::string;
using std::exception;

namespace db
{
	class InconsistentTypesException : public exception
	{
	public:

		const char * what() const throw ()
		{
			return msg.c_str();
		}

		InconsistentTypesException(string _msg = "") : msg(_msg) {}

	private:

		string msg;
	};
}
#endif // !INCONSISTENT_TYPES_EXCEPTION
