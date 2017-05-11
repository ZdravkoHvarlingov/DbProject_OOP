#include "Row.h"

DbType* & db::Row::operator[](size_t index)
{
	return columns[index];
}

DbType* db::Row::operator[](size_t index) const
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

void db::Row::ReleaseMemory()
{
	size_t len = columns.size();

	for (size_t ind = 0; ind < len; ind++)
	{
		delete columns[ind];
	}
}

void db::Row::CopyInfo(const Row & other)
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
		else if (other.columns[ind]->GetType() == "Integer")
		{
			AddColumn(other.columns[ind]->GetValueAsInt());
		}
		else
		{
			Null* nullpt = new Null;
			AddColumn(nullpt);
		}
	}
}

ostream & db::operator<<(ostream & outStr, const Row & rowToDisplay)
{
	for (size_t ind = 0; ind < rowToDisplay.colSize; ind++)
	{
		if (rowToDisplay[ind]->GetType() == "Decimal")
		{
			outStr << rowToDisplay[ind]->GetValueAsDecimal() << " ";
		}
		else if (rowToDisplay[ind]->GetType() == "Integer")
		{
			outStr << rowToDisplay[ind]->GetValueAsInt() << " ";
		}
		else// if (rowToDisplay[ind]->GetType() == "Text")
		{
			outStr << rowToDisplay[ind]->GetValueAsString() << " ";
		}
	}

	return outStr;
}
