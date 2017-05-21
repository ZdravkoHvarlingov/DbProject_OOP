#ifndef DB_TABLE
#define DB_TABLE

#include "Row.h"
#include "InconsistentTypesException.h"

using db::Row;
using db::InconsistentTypesException;

template <typename T>
using AggFun = T(*)(T, T);

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

		friend Table InnerJoin(const Table& firstTable, size_t firstCol, const Table& secondTable, size_t secondCol);

		template <typename ResT>
		ResT Aggregate(size_t colToSearch, DbType* valueToSearch, size_t colToAgg, AggFun<ResT> function, ResT initValue) const;

		size_t CountCertainRows(size_t colToSearch, DbType* elementToSearch) const;
		void DeleteCertainRows(size_t colToSearch, DbType* elementToSearch);
		void UpdateCertainRows(size_t colToSearch, DbType* elementToSearch, size_t colToChange, DbType* valueToSet);

		void ChangeCell(size_t row, size_t col, DbType* value);

		void SetNullCell(size_t row, size_t col);
		void SetColNullExceptance(bool value, size_t _index);

		friend ostream& operator << (ostream& outStr, const Table& tableToDisplay);
		friend istream& operator >> (istream& inStr, Table& tableToInit);

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

	template<typename ResT>
	inline ResT Table::Aggregate(size_t colToSearch, DbType * valueToSearch, 
		size_t colToAgg, AggFun<ResT> function, ResT initValue) const
	{
		string type = headerCols[colToAgg].headerType;

		if (type == "Text")
		{
			throw InconsistentTypesException("Can not use aggregate function with type \"Text\"");
		}

		size_t rowsCount = rows.size();
		ResT accumotator = initValue;

		for (size_t ind = 0; ind < rowsCount; ind++)
		{
			if (rows[ind][colToSearch]->AreEqual(valueToSearch))
			{
				if (rows[ind][colToAgg]->CheckIfValueIsNull() == false)
				{
					if (type == "Decimal")
					{
						accumotator = function(accumotator, rows[ind][colToAgg]->GetValueAsDecimal());
					}
					else accumotator = function(accumotator, rows[ind][colToAgg]->GetValueAsInt());
				}
			}
		}

		return accumotator;
	}
}

#endif // !DB_TABLE
