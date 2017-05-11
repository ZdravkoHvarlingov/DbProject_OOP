#include "Table.h"
#include "Null.h"
#include "NoHeaderRowException.h"

using db::Null;
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

	Row rowToAdd;
	rowToAdd.AddColumn((int)autoIncrement);
	++autoIncrement;

	size_t len = headerCols.size() - 1;
	for (size_t ind = 0; ind < len; ind++)
	{
		Null* nullToAdd = nullptr;
		rowToAdd.AddColumn<Null*>(nullToAdd);
	}

	rows.push_back(rowToAdd);
}

void db::Table::MakeNewRow(const Row& _rowToAdd)
{
	if (headerCols.size() == 1 || _rowToAdd.GetColmSize() != headerCols.size() - 1)
	{
		throw NoHeaderRowException("Missing such table header row!");
	}
	Row rowToEnter;
	rowToEnter.AddColumn((int)autoIncrement);
	++autoIncrement;

	for (size_t ind = 0; ind < _rowToAdd.GetColmSize(); ind++)
	{
		if (_rowToAdd[ind]->GetType() == "Text")
		{
			rowToEnter.AddColumn(_rowToAdd[ind]->GetValueAsString());
		}
		else if (_rowToAdd[ind]->GetType() == "Decimal")
		{
			rowToEnter.AddColumn(_rowToAdd[ind]->GetValueAsDecimal());
		}
		else if (_rowToAdd[ind]->GetType() == "Integer")
		{
			rowToEnter.AddColumn(_rowToAdd[ind]->GetValueAsInt());
		}
		else
		{
			Null* ptr = nullptr;
			rowToEnter.AddColumn<Null*>(ptr);
		}
	}

	rows.push_back(rowToEnter);
}

void db::Table::AddNewColumn(string _colName, string _colType, bool _canBeNull = true)
{
	HeaderCol headerColToAdd = {
		_colName,
		_colType,
		_canBeNull
	};

	headerCols.push_back(headerColToAdd);
}

void db::Table::SetColNullExceptance(bool value, size_t _index)
{
	//exception to implement: OutOfRangeException

	headerCols[_index].CanBeNull = value;
}

Row & db::Table::operator[](size_t ind)
{
	//OutOfRangeException?!

	return rows[ind];
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
