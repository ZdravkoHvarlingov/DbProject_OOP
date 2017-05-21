#ifndef DB_INTEGER
#define DB_INTEGER

#include "DbType.h"

using db::DbType;

namespace db 
{
	class Integer : public DbType
	{
	public:
		Integer();
		Integer(int _number);
		string GetType() const override;
		int GetValueAsInt() const override;
		void SetIntValue(int value);
		
		// Inherited via DbType
		virtual bool AreEqual(const DbType* other) const override;
		virtual void Serialize(ostream & outStr) const override;
		virtual void DeSerialize(istream& inStr) override;
		virtual void CopyValueFrom(const DbType* other) override;
	
	private:
		int number;
	};
}

#endif // !DB_INTEGER
