#include "Decimal.h"

db::Decimal::Decimal()
{
	SetNull();
}

db::Decimal::Decimal(double _decimal)
{
	SetDecimalValue(_decimal);
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

bool db::Decimal::AreEqual(const DbType * other) const
{
	if (other->GetType() != "Decimal")
	{
		throw db::InconsistentTypesException("Cannot implicit convert to Decimal");
	}

	else return (other->CheckIfValueIsNull() && CheckIfValueIsNull()) ||
		(fabs(other->GetValueAsDecimal() - decimal) < 0.00001 &&
		other->CheckIfValueIsNull() == CheckIfValueIsNull());
}

void db::Decimal::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL";
	}
	else outStr << GetValueAsDecimal();
}

void db::Decimal::DeSerialize(istream & inStr)
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
			decimal = 0;
			SetNull();

			return;
		}
	}

	inStr >> decimal;
	MakeValueNotNull();
}

void db::Decimal::CopyValueFrom(const DbType * other)
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
