#ifndef DB_TEXT
#define DB_TEXT

#include "DbType.h"

using db::DbType;

namespace db
{
	class Text : public DbType
	{
	public:
		string GetType() const override;
		string GetValueAsString() const override;
		void SetStringValue(string value);

	private:
		string text;
	};
}

#endif // !DB_TEXT
