#include "Table.h"
#include "Null.h"

using db::Null;

string db::Table::GetName() const
{
	return name;
}

void db::Table::SetName(string _name)
{
	name = _name;
}

db::Table::Table(string _name, const vector<string>& _headers)
{
	name = _name;
	headers.insert(headers.end(), _headers.begin(), _headers.end());

	size_t len = _headers.size();

	for (size_t ind = 0; ind < len; ind++)
	{
		canBeNullCol.push_back(true);
	}
}

void db::Table::MakeNewRow()
{
	Row rowToAdd;

	size_t len = headers.size();

	for (size_t ind = 0; ind < len; ind++)
	{
		Null* nullToAdd = new Null;
		rowToAdd.AddColumn(nullToAdd);
	}

	rows.push_back(rowToAdd);
}

void db::Table::MakeNewRow(const Row& _rowToAdd)
{
	rows.push_back(_rowToAdd);
}

void db::Table::AddNewColumn(string colHeader)
{
	headers.push_back(colHeader);
	canBeNullCol.push_back(true);
}

void db::Table::SetColNullExceptance(bool value, size_t _index)
{
	//exception to implement: OutOfRangeException
	canBeNullCol[_index] = value;
}

Row & db::Table::operator[](size_t ind)
{
	//OutOfRangeException?!

	return rows[ind];
}

ostream & db::operator<<(ostream & outStr, const Table & tableToDisplay)
{
	size_t hCount = tableToDisplay.headers.size();
	for (size_t ind = 0; ind < hCount; ind++)
	{
		outStr << tableToDisplay.headers[ind] << " ";
	}

	outStr << '\n';

	size_t len = tableToDisplay.rows.size();
	for (size_t ind = 0; ind < len; ind++)
	{
		outStr << tableToDisplay.rows[ind] << '\n';
	}

	return outStr;
}
