#ifndef NO_HEADERROW_EXCEPTION
#define NO_HEADERROW_EXCEPTION

#include <exception>
#include <string>

using std::string;
using std::exception;

namespace db
{
	class NoHeaderRowException : public exception
	{
	public:

		const char * what() const throw ()
		{
			return msg.c_str();
		}

		NoHeaderRowException(string _msg = "") : msg(_msg) {}

	private:

		string msg;
	};
}
#endif // !NO_HEADERROW_EXCEPTION
