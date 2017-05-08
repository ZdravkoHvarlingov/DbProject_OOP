#ifndef ROW
#define ROW

#include "DbType.h"
#include <vector>

using db::DbType;
using std::vector;

namespace db
{
	class Row
	{
	public:
		template <typename T>
		void AddColumn(T value);

	private:
		vector<DbType*> columns;
	};
}

#endif // !ROW
