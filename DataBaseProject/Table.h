#ifndef DB_TABLE
#define DB_TABLE

#include "Row.h"

using db::Row;

namespace db
{
	class Table
	{
	public:

		string GetName() const;
		void SetName(string _name);
		Table(string _name, const vector<string>& headers);
		void MakeNewRow();
		void MakeNewRow(const Row& _rowToAdd);
		void AddNewColumn(string colHeader);
		void SetColNullExceptance(bool value, size_t _index);
		Row& operator[](size_t ind);

		friend ostream& operator << (ostream& outStr, const Table& tableToDisplay);

	private:
		
		string name;
		vector<Row> rows;
		vector<string> headers;
		vector<bool> canBeNullCol;
	};
}

#endif // !DB_TABLE
