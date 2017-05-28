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
		virtual void Serialize(ostream & outStr, size_t setWSize = 0) const override;
		virtual void DeSerialize(istream& inStr) override;
		virtual void CopyValueFrom(const DbType* other) override;
		virtual size_t GetValueLength() const override;

	private:

		size_t CountEscapeCharacters(const string& _text, size_t beforeInd);
		string text;
	};
}

#endif // !DB_TEXT
