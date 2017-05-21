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

		string GetType() const override;
		string GetValueAsString() const override;
		void SetStringValue(const string& value);

		// Inherited via DbType
		virtual bool AreEqual(const DbType* other) const override;
		virtual void Serialize(ostream & outStr) const override;
		virtual void DeSerialize(istream& inStr) override;
		virtual void CopyValueFrom(const DbType* other) override;

	private:
		string text;
	};
}

#endif // !DB_TEXT
