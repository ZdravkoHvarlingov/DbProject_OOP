#include "Row.h"

void db::Row::AddNullColumn(string type)
{
	DbType* colToAdd;
	if (type == "Decimal")
	{
		colToAdd = new Decimal;
	}
	else if (type == "Integer")
	{
		colToAdd = new Integer;
	}
	else colToAdd = new Text;

	columns.push_back(colToAdd);
	++colSize;
}

DbType* & db::Row::operator[](size_t index)
{
	return columns[index];
}

const DbType* const & db::Row::operator[](size_t index) const
{
	return columns[index];
}

size_t db::Row::GetColmSize() const
{
	return colSize;
}

db::Row::Row() 
	: colSize(0)
{
}

db::Row::Row(const Row & other)
{
	CopyInfo(other);
}

db::Row & db::Row::operator=(const Row & other)
{
	if (this != &other)
	{
		ReleaseMemory();
		CopyInfo(other);
	}

	return *this;
}

db::Row::~Row()
{
	ReleaseMemory();
}

void db::Row::PushToColumns(DbType * _cellToAdd, int ind)
{
	if (ind == -1 || ind == columns.size())
	{
		columns.push_back(_cellToAdd);
	}	
	else if(ind >= 0 && ind < columns.size())
	{
		vector<DbType*> result;

		for (size_t index = 0; index < ind; index++)
		{
			result.push_back(columns[index]);
		}
		result.push_back(_cellToAdd);
		for (size_t index = ind; index < colSize; index++)
		{
			result.push_back(columns[index]);
		}

		columns = result;
	}
	else return;

	++colSize;
}

void db::Row::ReleaseMemory()
{
	size_t len = columns.size();

	for (size_t ind = 0; ind < len; ind++)
	{
		delete columns[ind];
	}
}

void db::Row::CopyInfo(const Row & other) // a better polymorphism way?
{
	for (size_t ind = 0; ind < other.colSize; ind++)
	{
		if (other.columns[ind]->GetType() == "Text")
		{
			AddColumn(other.columns[ind]->GetValueAsString());
		}
		else if (other.columns[ind]->GetType() == "Decimal")
		{
			AddColumn(other.columns[ind]->GetValueAsDecimal());
		}
		else
		{
			AddColumn(other.columns[ind]->GetValueAsInt());
		}

		if (other.columns[ind]->CheckIfValueIsNull())
		{
			columns[ind]->SetNull();
		}
	}
}

ostream & db::operator<<(ostream & outStr, const Row & rowToDisplay)
{
	for (size_t ind = 0; ind < rowToDisplay.colSize; ind++)
	{
		rowToDisplay[ind]->Serialize(outStr);
	}

	return outStr;
}
