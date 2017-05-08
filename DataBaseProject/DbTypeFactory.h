#ifndef DB_TYPE_FACTORY
#define DB_TYPE_FACTORY

#include "MainDbType.h"
#include "DbType.h"
#include "Null.h"
#include "Integer.h"
#include "Decimal.h"
#include "Text.h"

using db::DbType;
using db::Decimal;
using db::Integer;
using db::Null;
using db::Text;
using db::MainDbType;

namespace db
{
	class DbTypeFactory
	{
	public:
		static DbType* GetType(MainDbType type);
	};
}

#endif // !DB_TYPE_FACTORY
