#ifndef DB_DECIMAL
#define DB_DECIMAL

#include "DbType.h"

using db::DbType;

namespace db	
{
	class Decimal : public DbType
	{
	public:
		Decimal();
		string GetType() const override;
		double GetValueAsDecimal() const override;
		void SetDecimalValue(double value) override;
		
		// Inherited via DbType
		virtual void Serialize(ostream & outStr) const override;
	
	private:
		double decimal;


	};
}

#endif // !DB_DECIMAL
