#include "DbTypeFactory.h"

DbType * db::DbTypeFactory::GetType(MainDbType type)
{
	switch (type)
	{
	case db::MainDbType::Null:
		return new Null;
		break;
	case db::MainDbType::Integer:
		return new Integer;
		break;
	case db::MainDbType::Decimal:
		return new Decimal;
		break;
	case db::MainDbType::Text:
		return new Text;
		break;
	default:
		return nullptr;
		break;
	}
}
