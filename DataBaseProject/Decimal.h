#ifndef DB_DECIMAL
#define DB_DECIMAL

#include "DbType.h"

using db::DbType;

namespace db
{
	class Decimal : DbType
	{
	public:
		string GetType() const override;
		double GetDecimal() const;
		void SetDecimal(double _decimal);

	private:
		double decimal;
	};
}

#endif // !DB_DECIMAL
