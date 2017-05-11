#include "Null.h"

string db::Null::GetType() const
{
	return "Null";
}

string db::Null::GetValueAsString() const
{
	return "NULL";
}
