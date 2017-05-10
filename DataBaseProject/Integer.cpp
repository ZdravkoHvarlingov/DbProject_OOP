#include "Integer.h"

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
	number = value;
}

