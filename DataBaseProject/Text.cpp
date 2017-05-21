#include "Text.h"
#include "InconsistentTypesException.h"
#include <vector>

using std::vector;

db::Text::Text()
{
	SetNull();
}

db::Text::Text(const string & _text)
{
	SetStringValue(_text);
}

string db::Text::GetType() const
{
	return "Text";
}

string db::Text::GetValueAsString() const
{
	return text;
}

void db::Text::SetStringValue(const string& value)
{
	MakeValueNotNull();
	text = value;
}

bool db::Text::AreEqual(const DbType * other) const
{
	if (other->GetType() != "Text")
	{
		throw db::InconsistentTypesException("Can not implicit convert to Text");
	}

	return (other->CheckIfValueIsNull() && CheckIfValueIsNull()) ||
		(text == other->GetValueAsString() &&
		CheckIfValueIsNull() == other->CheckIfValueIsNull());
}

void db::Text::Serialize(ostream & outStr) const
{
	if (CheckIfValueIsNull())
	{
		outStr << "NULL";
	}
	else
	{
		outStr << "\" ";
		string value = GetValueAsString();
		size_t valueLen = value.length();

		for (size_t ind = 0; ind < valueLen; ind++)
		{
			if (value[ind] == '"' || value[ind] == '\\')
			{
				outStr << '\\';
			}
			outStr << value[ind];
		}

		outStr << " \"";
	}
}

void db::Text::DeSerialize(istream & inStr)
{
	string result = "";
	result += inStr.get();
	result += inStr.get(); 
	char charToGet = inStr.get(); //check for NULL value
	result += charToGet;
	result += inStr.peek();

	if (result == "NULL")
	{
		text = "";
		SetNull();

		return;
	}

	result = "a";
	while (charToGet != '"' || result[result.length() - 1] != ' ')
	{
		result += charToGet;

		charToGet = inStr.get();
	}

	result = result.substr(1, result.length() - 2);

	size_t resLen = result.length();
	vector<size_t> indexesToRemove;
	for (size_t ind = 0; ind < resLen; ind++)
	{
		if (result[ind] == '"' || (result[ind] == '\\' && result[ind - 1] == '\\'))
		{
			indexesToRemove.push_back(ind - 1);
		}
	}

	size_t indxLen = indexesToRemove.size();
	if (indxLen != 0)
	{
		for (size_t ind = 0; ind < indxLen - 1; ind++)
		{
			result.erase(result.begin() + indexesToRemove[ind]);
			indexesToRemove[ind + 1] = indexesToRemove[ind + 1] - 1;
		}

		size_t indToRemove = indexesToRemove[indxLen - 1] - indxLen + 2;
		result.erase(result.begin() + indToRemove);
	}

	SetStringValue(result);
}

void db::Text::CopyValueFrom(const DbType * other)
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

