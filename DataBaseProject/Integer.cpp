#include "Integer.h"
#include "InconsistentTypesException.h"

db::Integer::Integer()
{
	SetNull();
}

db::Integer::Integer(int _number)
{
	SetIntValue(_number);
}

string db::Integer::GetType() const
{
	return "Integer";
}

int db::Integer::GetValueAsInt() const
{
	return number;
}

void db::Integer::SetIntValue(int value)
{
	MakeValueNotNull();
	number = value;
}

bool db::Integer::AreEqual(const DbType * other) const
{
	if (other->GetType() != "Integer")
	{
		throw db::InconsistentTypesException("Cannot implicit convert to Integer");
	}

	else return (other->CheckIfValueIsNull() && CheckIfValueIsNull()) ||
		(other->GetValueAsInt() == number &&
		other->CheckIfValueIsNull() == CheckIfValueIsNull());
}

void db::Integer::Serialize(ostream & outStr, size_t setWSize) const
{
	if (CheckIfValueIsNull())
	{
		outStr << std::left << std::setw(setWSize) << "NULL";
	}
	else outStr << std::left << std::setw(setWSize) << GetValueAsInt();
}

void db::Integer::DeSerialize(istream & inStr)
{
	if (inStr.peek() == 'N')
	{
		string nullStr;

		nullStr += inStr.get();
		nullStr += inStr.get();
		nullStr += inStr.get();
		nullStr += inStr.get();

		if (nullStr == "NULL")
		{
			number = 0;
			SetNull();

			return;
		}
	}

	inStr >> number;
	MakeValueNotNull();
}

void db::Integer::CopyValueFrom(const DbType * other)
{
	if (other->GetType() != "Integer")
	{
		throw db::InconsistentTypesException("Cannot implicit convert to Integer");
	}

	SetIntValue(other->GetValueAsInt());

	if (other->CheckIfValueIsNull())
	{
		SetNull();
	}
}

size_t db::Integer::GetValueLength() const
{
	if (CheckIfValueIsNull())
	{
		return 4;
	}

	return log10(number) + 1;
}

