#include "Decimal.h"

db::Decimal::Decimal()
{
	SetNull();
}

db::Decimal::Decimal(double _decimal)
{
	SetDecimalValue(_decimal);
}

const char* db::Decimal::GetType() const
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

bool db::Decimal::AreEqual(DbType * other) const
{
	if (other->GetType() != "Decimal")
	{
		throw db::InconsistentTypesException("Cannot implicit convert to Decimal");
	}

	else return fabs(other->GetValueAsDecimal() - decimal) < 0.00001 &&
		other->CheckIfValueIsNull() == CheckIfValueIsNull();
}

void db::Decimal::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL ";
	}
	else outStr << GetValueAsDecimal() << " ";
}

void db::Decimal::CopyValueFrom(DbType * other)
{
	if (other->GetType() != "Decimal")
	{
		throw db::InconsistentTypesException("Cannot implicit convert to Decimal");
	}

	SetDecimalValue(other->GetValueAsDecimal());
	
	if (other->CheckIfValueIsNull())
	{
		SetNull();
	}
}
