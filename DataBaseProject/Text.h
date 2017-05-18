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
		Text(const string& _text);

		const char* GetType() const override;
		string GetValueAsString() const override;
		void SetStringValue(string value);

		// Inherited via DbType
		virtual bool AreEqual(DbType* other) const override;
		virtual void Serialize(ostream & outStr) const override;
		virtual void CopyValueFrom(DbType* other) override;

	private:
		string text;
	};
}

#endif // !DB_TEXT
