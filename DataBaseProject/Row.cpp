#include "Row.h"
#include "DbTypeFactory.h"
#include <iomanip>

using db::DbTypeFactory;

void db::Row::AddColumn(const DbType * columnToAdd, int position)
{
	DbType* colToAdd = DbTypeFactory::GetNewType(columnToAdd->GetType());
	colToAdd->CopyValueFrom(columnToAdd);

	PushToColumns(colToAdd, position);
}

void db::Row::ChangeColumnValue(size_t index, DbType * newValue)
{
	columns[index]->CopyValueFrom(newValue);
}

void db::Row::AddNullColumn(const string& type)
{
	DbType* colToAdd = DbTypeFactory::GetNewType(type.c_str());

	columns.push_back(colToAdd);
}

void db::Row::DeleteColumn(size_t _ind)
{
	size_t cols = columns.size();

	delete columns[_ind];

	for (size_t ind = _ind + 1; ind < cols; ind++)
	{
		columns[ind - 1] = columns[ind];
	}
	
	columns.resize(cols - 1);
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
	return columns.size();
}

void db::Row::Serialize(ostream& outStr, size_t setWValue) const
{
	size_t colSize = columns.size();
	for (size_t ind = 0; ind < colSize - 1; ind++)
	{
		columns[ind]->Serialize(outStr);
		outStr << " ";
	}
	columns[colSize - 1]->Serialize(outStr);
}

void db::Row::Deserialize(istream & inStr, vector<DbType*> types)
{
	size_t vectSize = types.size();

	for (size_t ind = 0; ind < vectSize; ind++)
	{
		types[ind]->DeSerialize(inStr);
		inStr.ignore();

		AddColumn(types[ind]);
	}
}

db::Row::Row(){}

db::Row::Row(const Row & other)
{
	CopyInfo(other);
}

db::Row & db::Row::operator=(const Row & other)
{
	if (this != &other)
	{
		ReleaseMemory();
		columns.clear();
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
	size_t length = columns.size();

	if (ind == -1 || ind == length)
	{
		columns.push_back(_cellToAdd);
	}	
	else if(ind >= 0 && ind < length)
	{
		vector<DbType*> result;

		for (size_t index = 0; index < ind; index++)
		{
			result.push_back(columns[index]);
		}
		result.push_back(_cellToAdd);
		for (size_t index = ind; index < length; index++)
		{
			result.push_back(columns[index]);
		}

		columns = result;
	}
	//maybe out of range for the index?
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
	size_t len = other.columns.size();

	for (size_t ind = 0; ind < len; ind++)
	{
		AddColumn(other.columns[ind]);
	}
}

ostream & db::operator<<(ostream & outStr, const Row & rowToDisplay)
{
	rowToDisplay.Serialize(outStr);

	return outStr;
}

