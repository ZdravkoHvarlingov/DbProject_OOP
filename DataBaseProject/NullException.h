#ifndef NULL_EXCEPTION
#define NULL_EXCEPTION

#include <exception>
#include <string>

using std::string;
using std::exception;

namespace db
{
	class NullException : public exception
	{
	public:

		const char * what() const throw ()
		{
			return msg.c_str();
		}

		NullException(const string& _msg = "") : msg(_msg) {}

	private:

		string msg;
	};
}
#endif // !NULL_EXCEPTION
