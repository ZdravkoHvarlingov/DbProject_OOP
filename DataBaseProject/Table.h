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
		string GetDescription() const;
		void SetName(string _name);
		void MakeNewRow();
		void MakeNewRow(const Row& _rowToAdd);
		void AddNewColumn(const string& _colName,const string& _colType);  //, bool _canBeNull);
		void DeleteRow(size_t rowIndex);

		size_t CountCertainRows(size_t colToSearch, DbType* elementToSearch) const;
		void DeleteCertainRows(size_t colToSearch, DbType* elementToSearch);
		void UpdateCertainRows(size_t colToSearch, DbType* elementToSearch, size_t colToChange, DbType* valueToSet);

		void ChangeCell(size_t row, size_t col, DbType* value);

		void SetNullCell(size_t row, size_t col);
		void SetColNullExceptance(bool value, size_t _index);

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
