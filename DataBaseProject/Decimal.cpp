#include "Decimal.h"

string db::Decimal::GetType() const
{
	return "Decimal";
}

double db::Decimal::GetDecimal() const
{
	return decimal;
}

void db::Decimal::SetDecimal(double _decimal)
{
	decimal = _decimal;
}
