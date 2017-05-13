#ifndef DB_TEXT
#define DB_TEXT

#include "DbType.h"

using db::DbType;

namespace db
{
	class Text : public DbType
	{
	public:
		Text();
		const char* GetType() const override;
		string GetValueAsString() const override;
		void SetStringValue(string value);

		// Inherited via DbType
		virtual void Serialize(ostream & outStr) const override;

	private:
		string text;
	};
}

#endif // !DB_TEXT
