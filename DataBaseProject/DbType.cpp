#include "DbType.h"
#include "NotImplementedException.h"

string db::DbType::GetValueAsString() const
{
	throw NotImplementedException("No suitable conversation to string!");
}

int db::DbType::GetValueAsInt() const
{
	throw NotImplementedException("No suitable conversation to int!");
}

double db::DbType::GetValueAsDecimal() const
{
	throw NotImplementedException("No suitable conversation to decimal!");
}

void db::DbType::SetStringValue(string value)
{
	throw NotImplementedException("No getter for a string value!");
}

void db::DbType::SetIntValue(int value)
{
	throw NotImplementedException("No getter for an integer value!");
}

void db::DbType::SetDecimalValue(double value)
{
	throw NotImplementedException("No getter for a decimal value!");
}
