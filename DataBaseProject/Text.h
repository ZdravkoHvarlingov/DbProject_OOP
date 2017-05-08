#ifndef DB_TEXT
#define DB_TEXT

#include "DbType.h"

using db::DbType;

namespace db
{
	class Text : DbType
	{
	public:
		string GetType() const override;
		void SetText(string _text);
		string GetText() const;

	private:
		string text;
	};
}

#endif // !DB_TEXT
