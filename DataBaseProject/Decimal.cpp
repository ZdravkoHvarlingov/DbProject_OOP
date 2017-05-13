#include "Decimal.h"

db::Decimal::Decimal()
{
	SetNull();
}

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
	MakeValueNotNull();
}

void db::Decimal::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL ";
	}
	else outStr << GetValueAsDecimal() << " ";
}
