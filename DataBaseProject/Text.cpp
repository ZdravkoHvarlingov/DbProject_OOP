#include "Text.h"

string db::Text::GetType() const
{
	return "Text";
}

void db::Text::SetText(string _text)
{
	text = _text;
}

string db::Text::GetText() const
{
	return text;
}
