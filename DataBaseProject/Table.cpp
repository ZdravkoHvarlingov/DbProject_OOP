#include "Table.h"
#include "NoHeaderRowException.h"
#include "InconsistentTypesException.h"
#include "OutOfRangeException.h"
#include <iostream>
#include <iomanip>
#include "DbTypeFactory.h"
#include "PointerWrapper.h"

using db::DbTypeFactory;
using db::NoHeaderRowException;
using db::OutOfRangeException;

db::Table::Table(string _name)
	: autoIncrement(0)
{
	headerCols.push_back(HeaderCol{
		"Id_auto",
		"Integer",
		false
	});

	SetName(_name);
}

string db::Table::GetName() const
{
	return name;
}

string db::Table::GetDescription() const
{
	string result = "";
	size_t len = headerCols.size();

	for (size_t ind = 0; ind < len; ind++)
	{
		result += "\"" + headerCols[ind].headerName + "\" - " + headerCols[ind].headerType;
		
		if (ind != len - 1)
		{
			result += ", ";
		}
	}

	return result;
}

vector<string> db::Table::GetColHeaders() const
{
	vector<string> result;
	size_t colsLength = headerCols.size();

	for (size_t ind = 0; ind < colsLength; ind++)
	{
		result.push_back(headerCols[ind].headerName);
	}

	return result;
}

const string & db::Table::GetColType(size_t col) const
{
	if (col >= headerCols.size() || col < 0)
	{
		throw OutOfRangeException("Search column is out of range!");
	}
	return headerCols[col].headerType;
}

size_t db::Table::GetAmountOfColumns() const
{
	return headerCols.size();
}

const vector<Row>& db::Table::GetRows() const
{
	return rows;
}

size_t db::Table::GetMaxCellSize() const
{
	size_t rowsAmount = rows.size();

	size_t max = 0;
	for (size_t ind = 0; ind < rowsAmount; ind++)
	{
		if (max < rows[ind].GetMaxCellValueLength())
		{
			max = rows[ind].GetMaxCellValueLength();
		}
	}

	return max;
}

vector<size_t> db::Table::GetColumnsMaxLengths() const
{
	vector<size_t> result;
	size_t colsAmount = headerCols.size();
	size_t rowsAmount = rows.size();

	for (size_t col = 0; col< colsAmount; col++)
	{
		size_t maxSize = headerCols[col].headerName.length();
		for (size_t row = 0; row < rowsAmount; row++)
		{
			if (rows[row][col]->GetValueLength() > maxSize)
			{
				maxSize = rows[row][col]->GetValueLength();
			}
		}
		result.push_back(maxSize);
	}

	return result;
}

void db::Table::SetName(string _name)
{
	name = _name;
}

void db::Table::MakeNewRow()
{
	if (headerCols.size() == 1)
	{
		throw NoHeaderRowException("Missing table header row!");
	}

	Integer autoCol;
	autoCol.SetIntValue(autoIncrement);

	Row rowToAdd;
	rowToAdd.AddColumn(&autoCol);
	++autoIncrement;

	size_t len = headerCols.size() - 1;
	for (size_t ind = 0; ind < len; ind++)
	{
		rowToAdd.AddNullColumn(headerCols[ind + 1].headerType);
	}

	rows.push_back(rowToAdd);
}

void db::Table::MakeNewRow(const Row& _rowToAdd)
{
	if (headerCols.size() == 1 || _rowToAdd.GetColmSize() != headerCols.size() - 1)
	{
		throw NoHeaderRowException("Missing such table header row!");
	}

	Row rowToEnter = _rowToAdd;
	for (size_t ind = 0; ind < headerCols.size() - 1; ind++)
	{
		if (rowToEnter[ind]->GetType() != headerCols[ind + 1].headerType)
		{
			throw InconsistentTypesException("Can not convert types");
		}
	}

	Integer autoCol;
	autoCol.SetIntValue(autoIncrement);

	rowToEnter.AddColumn(&autoCol, 0);
	++autoIncrement;

	rows.push_back(rowToEnter);
}

void db::Table::AddNewColumn(const string& _colName, const  string& _colType)  //, bool _canBeNull = true)
{
	HeaderCol headerColToAdd = {
		_colName,
		_colType,
		true
	};

	headerCols.push_back(headerColToAdd);

	size_t rowsCount = rows.size();
	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		rows[ind].AddNullColumn(_colType);
	}
}

void db::Table::DeleteRow(size_t rowIndex)
{
	rows.erase(rows.begin() + rowIndex);
}

size_t db::Table::CountCertainRows(size_t colToSearch, DbType * elementToSearch) const
{
	if (colToSearch < 0 || colToSearch >= headerCols.size())
	{
		throw OutOfRangeException("Search column is out of range!");
	}

	size_t rowsCount = rows.size();

	size_t counter = 0;

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch)) //throws exceptions if types are different
		{
			counter++;
		}
	}

	return counter;
}

void db::Table::DeleteCertainRows(size_t colToSearch, DbType * elementToSearch)
{
	if (colToSearch < 0 || colToSearch >= headerCols.size())
	{
		throw OutOfRangeException("Search column is out of range!");
	}

	size_t rowsCount = rows.size();
	vector<size_t> indexesToDelete;

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			indexesToDelete.push_back(ind);
		}
	}

	size_t amountOfIndexes = indexesToDelete.size();
	for (size_t ind = 0; ind < amountOfIndexes; ind++)
	{
		DeleteRow(indexesToDelete[ind]);
		for (size_t innerInd = ind + 1; innerInd < amountOfIndexes; innerInd++)
		{
			--indexesToDelete[innerInd];
		}
	}
}

void db::Table::UpdateCertainRows(size_t colToSearch, DbType * elementToSearch, size_t colToChange, DbType * valueToSet)
{
	if (colToSearch >= headerCols.size() || colToSearch < 0)
	{
		throw OutOfRangeException("Search column is out of range!");
	}
	if (colToChange >= headerCols.size() || colToChange < 0)
	{
		throw OutOfRangeException("Change column is out of range!");
	}

	size_t rowsCount = rows.size();

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			rows[ind][colToChange]->CopyValueFrom(valueToSet);
		}
	}
}

vector<Row> db::Table::SelectCertainRows(size_t colToSearch, DbType * elementToSearch) const
{
	if (colToSearch >= headerCols.size() || colToSearch < 0)
	{
		throw OutOfRangeException("Search column is out of range!");
	}

	size_t rowsAmount = rows.size();
	vector<Row> result;

	for (size_t ind = 0; ind < rowsAmount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			result.push_back(rows[ind]);
		}
	}

	return result;
}

void db::Table::ChangeCell(size_t row, size_t col, DbType * value)
{
	//Out of range exception for col?
	if (value->CheckIfValueIsNull() && headerCols[col].CanBeNull == false)
	{
		return; // maybe something smarter
	}

	rows[row].ChangeColumnValue(col, value); // InconsistentTypesException will be thrown in case of different types along the functions chain
}

void db::Table::SetNullCell(size_t row, size_t col)
{
	if (headerCols[col].CanBeNull)
	{
		rows[row][col]->SetNull();
	}
	//else throw exception ... TO BE IMPLEMENTED
}

void db::Table::SetColNullExceptance(bool value, size_t _index)
{
	//exception to implement: OutOfRangeException

	headerCols[_index].CanBeNull = value;
}

ostream & db::operator<<(ostream & outStr, const Table & tableToDisplay)
{
	outStr << tableToDisplay.name << '\n';
	size_t hCount = tableToDisplay.headerCols.size();
	outStr << hCount << "\n";
	for (size_t ind = 0; ind < hCount; ind++)
	{
		Text hdr(tableToDisplay.headerCols[ind].headerName);
		hdr.Serialize(outStr);
		outStr << " " << tableToDisplay.headerCols[ind].headerType << " ";
	}

	outStr << '\n' << tableToDisplay.rows.size() << '\n';

	size_t len = tableToDisplay.rows.size();
	for (size_t ind = 0; ind < len - 1; ind++)
	{
		tableToDisplay.rows[ind].Serialize(outStr);
		outStr << '\n';
	}
	if (len != 0)
	{
		tableToDisplay.rows[len - 1].Serialize(outStr);
	}

	return outStr;
}

istream & db::operator>>(istream & inStr, Table & tableToInit)
{
	tableToInit.headerCols.erase(tableToInit.headerCols.begin());
	string input;
	std::getline(inStr, input); //deserialize table name
	tableToInit.SetName(input);

	int inpNumber;
	inStr >> inpNumber;
	inStr.ignore();

	Text columnText;

	vector<PointerWrapper<DbType>> colTypes;
	for (size_t ind = 0; ind < inpNumber; ind++) //deserialize columns
	{
		columnText.DeSerialize(inStr);
		inStr >> input;
		inStr.ignore();

		tableToInit.AddNewColumn(columnText.GetValueAsString(), input);
		colTypes.push_back(PointerWrapper<DbType>(DbTypeFactory::GetNewType(input)));
	}

	inStr >> inpNumber;
	inStr.ignore();

	for (size_t ind = 0; ind < inpNumber; ind++)
	{
		Row rowToAdd;
		rowToAdd.Deserialize(inStr, colTypes); //deserialize rows
		tableToInit.rows.push_back(rowToAdd);
	}

	tableToInit.autoIncrement = tableToInit.rows[inpNumber - 1][0]->GetValueAsInt() + 1;
	return inStr;
}

db::Table db::Table::InnerJoin(size_t firstCol, const db::Table & secondTable, size_t secondCol)
{
	db::Table result("InnerJoin: " + GetName() + ", " + secondTable.GetName());

	size_t firstTableCols = headerCols.size();
	size_t secondTableCols = secondTable.headerCols.size();

	if (firstCol < 0 || firstCol >= firstTableCols || secondCol < 0 || secondCol >= secondTableCols)
	{
		throw OutOfRangeException("Invalid inner join columns!");
	}

	for (size_t ind = 0; ind <= firstCol; ind++)
	{
		result.AddNewColumn("FTable:" + headerCols[ind].headerName, headerCols[ind].headerType);
	}

	for (size_t ind = 0; ind < secondTableCols; ind++)
	{
		result.AddNewColumn("STable:" + secondTable.headerCols[ind].headerName, secondTable.headerCols[ind].headerType);
	}

	for (size_t ind = firstCol + 1; ind < firstTableCols; ind++)
	{
		result.AddNewColumn("FTable:" + headerCols[ind].headerName, headerCols[ind].headerType);
	}

	size_t firstTableRows = rows.size();
	size_t secondTableRows = secondTable.rows.size();
	for (size_t fInd = 0; fInd < firstTableRows; fInd++)
	{
		for (size_t sInd = 0; sInd < secondTableRows; sInd++)
		{
			if (rows[fInd][firstCol]->AreEqual(secondTable.rows[sInd][secondCol]))
			{
				Row rowToAdd;
				for (size_t fCols = 0; fCols <= firstCol; fCols++)
				{
					rowToAdd.AddColumn(rows[fInd][fCols]);
				}

				for (size_t sCols = 0; sCols < secondTableCols; sCols++)
				{
					rowToAdd.AddColumn(secondTable.rows[sInd][sCols]);
				}

				for (size_t fCols = firstCol + 1; fCols < firstTableCols; fCols++)
				{
					rowToAdd.AddColumn(rows[fInd][fCols]);
				}

				result.MakeNewRow(rowToAdd);
			}
		}
	}

	return result;
}