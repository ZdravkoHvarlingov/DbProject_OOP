#ifndef DB_TABLE
#define DB_TABLE

#include "Row.h"
#include "InconsistentTypesException.h"

using db::Row;
using db::InconsistentTypesException;

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
		void AddNewColumn(string _colName, string _colType);  //, bool _canBeNull);

		template<typename T>
		void ChangeCell(size_t row, size_t col, T value, string type);

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
	template<typename T>
	inline void Table::ChangeCell(size_t row, size_t col, T value, string type)
	{
		if (type != headerCols[col].headerType)
		{
			throw InconsistentTypesException("Can not implicit convert" + type + " to " + headerCols[col].headerType);
		}

		rows[row].ChangeColumn(value, col);
	}
}

#endif // !DB_TABLE
