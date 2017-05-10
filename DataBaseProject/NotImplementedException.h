#ifndef NOT_IMPLEMENTED_EXCEPTION
#define NOT_IMPLEMENTED_EXCEPTION

#include <exception>
#include <string>

using std::string;
using std::exception;

namespace db
{
	class NotImplementedException : public exception
	{
	public:

		const char * what() const throw () 
		{
			return msg.c_str();
		}

		NotImplementedException(string _msg = "") : msg("Not implemented exception: " + _msg) {}

	private:

		string msg;
	};
}
#endif // !NOT_IMPLEMENTED_EXCEPTION
