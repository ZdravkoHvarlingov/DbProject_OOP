#ifndef ROW
#define ROW

#include "DbType.h"
#include <vector>
#include "NotImplementedException.h"
#include "Integer.h"
#include "Decimal.h"
#include "Text.h"
#include "Null.h"
#include <iostream>

using std::ostream;
using db::DbType;
using std::vector;
using db::Decimal;
using db::Text;
using db::Integer;
using db::NotImplementedException;
using db::Null;

namespace db
{
	class Row
	{
	public:

		template <typename T>
		void AddColumn(T value);

		DbType*& operator[] (size_t index);
		DbType* operator[] (size_t index) const;
		size_t GetColmSize() const;
		friend ostream& operator << (ostream& outStr, const Row& rowToDisplay);

		Row();
		Row(const Row& other);
		Row& operator=(const Row& other);
		~Row();

	private:

		void ReleaseMemory();
		void CopyInfo(const Row& other);
		size_t colSize = 0;
		vector<DbType*> columns;
	};

	template<typename T>
	inline void Row::AddColumn(T value)
	{
		throw NotImplementedException("No add column method implemented!"); //Maybe NULL implementation here
	}

	template<>
	inline void Row::AddColumn(Null* value)
	{
		columns.push_back(value);

		++colSize;
	}

	template<>
	inline void Row::AddColumn(int value)
	{
		DbType* cellToAdd = new Integer;
		cellToAdd->SetIntValue(value);
		columns.push_back(cellToAdd);

		++colSize;
	}

	template<>
	inline void Row::AddColumn(double value)
	{
		DbType* cellToAdd = new Decimal;
		cellToAdd->SetDecimalValue(value);
		columns.push_back(cellToAdd);
		
		++colSize;
	}

	template<>
	inline void Row::AddColumn(string value)
	{
		DbType* cellToAdd = new Text;
		cellToAdd->SetStringValue(value);
		columns.push_back(cellToAdd);
		
		++colSize;
	}
}

#endif // !ROW
