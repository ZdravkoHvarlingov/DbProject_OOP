#ifndef DB_NULL
#define DB_NULL

#include "DbType.h"

using db::DbType;

namespace db
{
	class Null : public DbType
	{
	public:
		string GetType() const override;
	};
}

#endif // !DB_NULL
