#include "Text.h"

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
	text = value;
}

