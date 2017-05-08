#ifndef DB_INTEGER
#define DB_INTEGER

#include "DbType.h"

using db::DbType;

namespace db 
{
	class Integer : public DbType
	{
	public:
		string GetType() const override;
		int GetNumber() const;
		void SetNumber(int _number);

	private:
		int number;
	};
}

#endif // !DB_INTEGER
