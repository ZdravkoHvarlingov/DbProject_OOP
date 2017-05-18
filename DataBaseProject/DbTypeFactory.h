#ifndef DB_TYPE_FACTORY
#define DB_TYPE_FACTORY

#include "Decimal.h"
#include "Integer.h"
#include "Text.h"
#include <string>

using std::string;
using db::Decimal;
using db::Integer;
using db::Text;

namespace db
{
	class DbTypeFactory
	{
	public:
		static DbType* GetNewType(const string& type);
	};
}

#endif // !DB_TYPE_FACTORY
