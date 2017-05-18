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
		const char* GetType() const override;
		int GetValueAsInt() const override;
		void SetIntValue(int value);
		
		// Inherited via DbType
		virtual bool AreEqual(DbType* other) const override;
		virtual void Serialize(ostream & outStr) const override;
		virtual void CopyValueFrom(DbType* other) override;
	
	private:
		int number;
	};
}

#endif // !DB_INTEGER
