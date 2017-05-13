#include "Integer.h"

db::Integer::Integer()
{
	SetNull();
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

void db::Integer::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL ";
	}
	else outStr << GetValueAsInt() << " ";
}

