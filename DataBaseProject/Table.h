#ifndef DB_TABLE
#define DB_TABLE

#include "Row.h"
#include "InconsistentTypesException.h"
#include "OutOfRangeException.h"
#include <map>


using db::Row;
using db::InconsistentTypesException;
using db::OutOfRangeException;
using std::map;

template <typename T>
using AggFun = T(*)(T, T);

namespace db
{
	class Table
	{
	public:

		Table(string _name = "default");
		string GetName() const;
		string GetDescription() const;
		vector<string> GetColHeaders() const;
		const string& GetColType(size_t col) const;
		size_t GetAmountOfColumns() const;
		const vector<Row>& GetRows() const;
		vector<size_t> GetColumnsMaxLengths() const;
		bool DoesSuchIdExist(int id) const;

		void SetName(string _name);
		void MakeNewRow();
		void MakeNewRow(const Row& _rowToAdd);
		void AddNewColumn(const string& _colName,const string& _colType);  //, bool _canBeNull);

		Table InnerJoin(size_t firstCol, const Table& secondTable, size_t secondCol) const;
		Table LeftOuterJoin(size_t firstCol, const Table& secondTable, size_t secondCol) const;
		Table RightOuterJoin(size_t firstCol, const Table& secondTable, size_t secondCol) const;

		template <typename ResT>
		ResT Aggregate(size_t colToSearch, DbType* valueToSearch, size_t colToAgg, AggFun<ResT> function, ResT initValue) const;

		size_t CountCertainRows(size_t colToSearch, DbType* elementToSearch) const;
		void UpdateCertainRows(size_t colToSearch, DbType* elementToSearch, size_t colToChange, DbType* valueToSet);
		vector<Row> SelectCertainRows(size_t colToSearch, DbType* elementToSearch) const;

		void SetNullCell(size_t row, size_t col);
		void SetColNullAcceptance(bool value, size_t _index);

		void SetForeignKey(int columnIndex, Table* foreignKeyTable); //more things to implement
		void DeleteCertainRows(size_t colToSearch, DbType* elementToSearch);
		void RepairTableRelatedToThis(int idToDelete, Table* relatedTable); //table which have a foreignKey column to this table

		friend ostream& operator << (ostream& outStr, const Table& tableToDisplay);
		friend istream& operator >> (istream& inStr, Table& tableToInit);

	private:

		void DeleteRow(size_t rowIndex);
		bool BinarySearchId(int id, int startIndex, int endIndex) const;
		bool DoesAMappedIdExist(map<int, int> relatedRows, int mappedId) const;

		struct HeaderCol
		{
			string headerName;
			string headerType;
			bool canBeNull;
			Table* foreignKeyTable;
			map<int, int> relatedRows;
		};

		string name;
		vector<Row> rows;
		vector<HeaderCol> headerCols;
		size_t autoIncrement;
		vector<Table*> connectedTables;
	};

	template<typename ResT>
	inline ResT Table::Aggregate(size_t colToSearch, DbType * valueToSearch, 
		size_t colToAgg, AggFun<ResT> function, ResT initValue) const
	{
		size_t maxColSize = headerCols.size();
		if (colToSearch < 0 || colToAgg <0 || colToSearch >= maxColSize || colToAgg >= maxColSize)
		{
			throw OutOfRangeException("Columns to search and aggregate are out of range!");
		}

		string type = headerCols[colToAgg].headerType;

		if (type == "Text")
		{
			throw InconsistentTypesException("Can not use aggregate function with type \"Text\"!");
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
