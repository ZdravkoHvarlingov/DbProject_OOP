#include "DbTypeFactory.h"

using db::DbTypeFactory;

DbType * DbTypeFactory::GetNewType(const string& type)
{
	DbType* result = nullptr;

	if (type == "Decimal")
	{
		result = new Decimal;
	}
	else if (type == "Integer")
	{
		result = new Integer;
	}
	else if (type == "Text")
	{
		result = new Text;
	}

	return result;
}
