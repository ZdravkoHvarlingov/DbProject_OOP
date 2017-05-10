#include "Decimal.h"

string db::Decimal::GetType() const
{
	return "Decimal";
}

double db::Decimal::GetValueAsDecimal() const
{
	return decimal;
}

void db::Decimal::SetDecimalValue(double value)
{
	decimal = value;
}
