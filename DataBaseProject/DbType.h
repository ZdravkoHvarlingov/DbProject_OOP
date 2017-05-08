#ifndef DB_TYPE
#define DB_TYPE

#include <string>

using std::string;

namespace db
{
	class DbType
	{
	public:
		virtual string GetType() const = 0;
	};
}

#endif // !DB_TYPE
