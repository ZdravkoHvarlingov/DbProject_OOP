#ifndef DB_DECIMAL
#define DB_DECIMAL

#include "DbType.h"
#include "InconsistentTypesException.h"

using db::DbType;

namespace db	
{
	class Decimal : public DbType
	{
	public:
		Decimal();
		Decimal(double _decimal);

		string GetType() const override;
		double GetValueAsDecimal() const override;
		void SetDecimalValue(double value) override;
		
		// Inherited via DbType
		virtual bool AreEqual(const DbType* other) const override;
		virtual void Serialize(ostream & outStr) const override;
		virtual void DeSerialize(istream& inStr) override;
		virtual void CopyValueFrom(const DbType* other) override;
	
	private:
		double decimal;
	};
}

#endif // !DB_DECIMAL
