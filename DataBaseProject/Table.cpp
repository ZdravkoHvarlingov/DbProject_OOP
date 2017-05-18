#include "Table.h"
#include "NoHeaderRowException.h"
#include "InconsistentTypesException.h"
#include <iostream>

using db::NoHeaderRowException;

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
		result += headerCols[ind].headerName + " <" + headerCols[ind].headerType + ">";
		
		if (ind != len - 1)
		{
			result += ", ";
		}
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
	size_t rowsCount = rows.size();

	size_t counter = 0;

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			counter++;
		}
	}

	return counter;
}

void db::Table::DeleteCertainRows(size_t colToSearch, DbType * elementToSearch)
{
	size_t rowsCount = rows.size();
	vector<size_t> indexesToDelete;

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			indexesToDelete.push_back(ind);
		}
	}

	for (size_t ind = 0; ind < indexesToDelete.size(); ind++)
	{
		DeleteRow(indexesToDelete[ind]);
	}
}

void db::Table::UpdateCertainRows(size_t colToSearch, DbType * elementToSearch, size_t colToChange, DbType * valueToSet)
{
	size_t rowsCount = rows.size();

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			rows[ind][colToChange]->CopyValueFrom(valueToSet);
		}
	}
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
	outStr << "Table name: " << tableToDisplay.name << '\n';
	size_t hCount = tableToDisplay.headerCols.size();
	for (size_t ind = 0; ind < hCount; ind++)
	{
		outStr << tableToDisplay.headerCols[ind].headerName <<
			"<" << tableToDisplay.headerCols[ind].headerType << ">" << " ";
	}

	outStr << '\n';

	size_t len = tableToDisplay.rows.size();
	for (size_t ind = 0; ind < len; ind++)
	{
		outStr << tableToDisplay.rows[ind] << '\n';
	}

	return outStr;
}
