#include "Decimal.h"

db::Decimal::Decimal()
	: MaxFloatingPoints(3)
{
	SetNull();
}

db::Decimal::Decimal(double _decimal)
	: MaxFloatingPoints(3)
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

void db::Decimal::Serialize(ostream & outStr, size_t setWSize) const
{
	if (CheckIfValueIsNull())
	{
		outStr << std::left << std::setw(setWSize) << "NULL";
	}
	else outStr << std::left << std::setw(setWSize) << std::setprecision(MaxFloatingPoints) << std::fixed << GetValueAsDecimal();
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
	if (inStr.fail())
	{
		throw InconsistentTypesException("Can not convert proper to Decimal!");
	}
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

size_t db::Decimal::GetValueLength() const
{
	if (CheckIfValueIsNull())
	{
		return 4;
	}

	unsigned int modValue = fabs(decimal);

	size_t result = (size_t)log10(modValue) + 1 + MaxFloatingPoints + 1; //because of the floating point
	if (decimal < 0)
	{
		++result;
	}

	return result;
}
