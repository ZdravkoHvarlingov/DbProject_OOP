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

void db::Text::Serialize(ostream & outStr, size_t setWSize) const
{
	if (CheckIfValueIsNull())
	{
		outStr << std::left << std::setw(setWSize) << "NULL";
	}
	else
	{
		outStr << "\"";
		string value = GetValueAsString();
		size_t valueLen = value.length();

		size_t additionalSymbols = 0;
		for (size_t ind = 0; ind < valueLen; ind++)
		{
			if (value[ind] == '"' || value[ind] == '\\')
			{
				outStr << '\\';
				additionalSymbols++;
			}
			outStr << value[ind];
		}

		outStr << "\"";

		int diff = setWSize - value.size() + additionalSymbols + 2;
		if (diff > 0)
		{
			for (size_t ind = 0; ind < diff; ind++)
			{
				outStr << " ";
			}
		}
	}
}

void db::Text::DeSerialize(istream & inStr)
{
	string result = "";
	result += inStr.get();
	if (result == "N") //check for NULL value
	{
		result += inStr.get();
		result += inStr.get();
		result += inStr.get();

		if (result == "NULL")
		{
			text = "";
			SetNull();

			return;
		}
		else throw InconsistentTypesException("Can not convert proper to text!");
	}
	else if (result != "\"")
	{
		throw InconsistentTypesException("Can not convert proper to text!");
	}

	char charToGet = inStr.get(); 
	while (charToGet != '"' ||
		(charToGet == '"' && CountEscapeCharacters(result, result.length()) % 2 != 0))
	{
		result += charToGet;

		charToGet = inStr.get();
	}

	result = result.substr(1, result.length() - 1);

	size_t resLen = result.length();
	vector<size_t> indexesToRemove;
	for (size_t ind = 0; ind < resLen; ind++)
	{
		if (result[ind] == '"' || (result[ind] == '\\' && CountEscapeCharacters(result, ind) % 2 == 1))
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
			indexesToRemove[ind + 1] = indexesToRemove[ind + 1] - ind - 1;
		}

		result.erase(result.begin() + indexesToRemove[indxLen - 1]);
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

size_t db::Text::GetValueLength() const
{
	if (CheckIfValueIsNull())
	{
		return 4;
	}

	return text.length() + 2; //because of the quotes
}

size_t db::Text::CountEscapeCharacters(const string & _text, size_t beforeInd)
{
	size_t result = 0;
	--beforeInd;
	char symbol = _text[beforeInd];

	while (symbol == '\\')
	{
		++result;

		--beforeInd;
		symbol = _text[beforeInd];
	}

	return result;
}

