#ifndef DB_TABLE
#define DB_TABLE

#include "Row.h"

using db::Row;

namespace db
{
	class Table
	{
	public:

		Table(string _name);
		string GetName() const;
		void SetName(string _name);
		void MakeNewRow();
		void MakeNewRow(const Row& _rowToAdd);
		void AddNewColumn(string _colName, string _colType, bool _canBeNull);
		void SetColNullExceptance(bool value, size_t _index);
		Row& operator[](size_t ind);

		friend ostream& operator << (ostream& outStr, const Table& tableToDisplay);

	private:

		struct HeaderCol
		{
			string headerName;
			string headerType;
			bool CanBeNull;
		};

		string name;
		vector<Row> rows;
		vector<HeaderCol> headerCols;
		size_t autoIncrement;
	};
}

#endif // !DB_TABLE
