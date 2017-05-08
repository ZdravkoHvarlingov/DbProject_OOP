#include "Integer.h"

string db::Integer::GetType() const
{
	return "Integer";
}

int db::Integer::GetNumber() const
{
	return number;
}

void db::Integer::SetNumber(int _number)
{
	number = _number;
}
