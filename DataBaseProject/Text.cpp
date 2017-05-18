#include "Text.h"
#include "InconsistentTypesException.h"

db::Text::Text()
{
	SetNull();
}

const char* db::Text::GetType() const
{
	return "Text";
}

string db::Text::GetValueAsString() const
{
	return text;
}

void db::Text::SetStringValue(string value)
{
	MakeValueNotNull();
	text = value;
}

bool db::Text::AreEqual(DbType * other) const
{
	if (other->GetType() != "Text")
	{
		throw db::InconsistentTypesException("Can not implicit convert to Text");
	}

	return text == other->GetValueAsString() &&
		CheckIfValueIsNull() == other->CheckIfValueIsNull();
}

void db::Text::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL ";
	}
	else outStr << GetValueAsString() << " ";
}

void db::Text::CopyValueFrom(DbType * other)
{
	if (other->GetType() != "Text")
	{
		throw db::InconsistentTypesException("Can not implicit convert to Text");
	}

	SetStringValue(other->GetValueAsString());

	if (other->CheckIfValueIsNull())
	{
		SetNull();
	}
}

