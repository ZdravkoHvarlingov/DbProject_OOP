#ifndef OUT_OF_RANGE_EXCEPTION
#define OUT_OF_RANGE_EXCEPTION

#include <exception>
#include <string>

using std::string;
using std::exception;

namespace db
{
	class OutOfRangeException : public exception
	{
	public:

		const char * what() const throw ()
		{
			return msg.c_str();
		}

		OutOfRangeException(string _msg = "") : msg(_msg) {}

	private:

		string msg;
	};
}
#endif // !OUT_OF_RANGE_EXCEPTION
