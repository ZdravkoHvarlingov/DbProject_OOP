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

const char* db::Integer::GetType() const
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

bool db::Integer::AreEqual(DbType * other) const
{
	if (other->GetType() != "Integer")
	{
		throw db::InconsistentTypesException("Cannot implicit convert to Integer");
	}

	else return other->GetValueAsInt() == number &&
		other->CheckIfValueIsNull() == CheckIfValueIsNull();
}

void db::Integer::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL ";
	}
	else outStr << GetValueAsInt() << " ";
}

void db::Integer::CopyValueFrom(DbType * other)
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

