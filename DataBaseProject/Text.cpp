#include "Text.h"

db::Text::Text()
{
	SetNull();
}

string db::Text::GetType() const
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

void db::Text::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL ";
	}
	else outStr << GetValueAsString() << " ";
}

