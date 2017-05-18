#ifndef ROW
#define ROW

#include "DbType.h"
#include <vector>
#include "NotImplementedException.h"
#include "Integer.h"
#include "Decimal.h"
#include "Text.h"
#include <iostream>

using std::ostream;
using db::DbType;
using std::vector;
using db::Decimal;
using db::Text;
using db::Integer;
using db::NotImplementedException;

namespace db
{
	class Row
	{
	public:

		void AddColumn(DbType* columnToAdd, int position = -1);
		void ChangeColumnValue(size_t index, DbType* newValue);

		void AddNullColumn(const string& type);
		void DeleteColumn(size_t _ind);

		DbType*& operator[] (size_t index);
		const DbType* const &  operator[] (size_t index) const;
		size_t GetColmSize() const;
		friend ostream& operator << (ostream& outStr, const Row& rowToDisplay);

		Row();
		Row(const Row& other);
		Row& operator=(const Row& other);
		~Row();

	private:

		void PushToColumns(DbType* _cellToAdd, int _ind);
		void ReleaseMemory();
		void CopyInfo(const Row& other);
		vector<DbType*> columns;
	};
}

#endif // !ROW
