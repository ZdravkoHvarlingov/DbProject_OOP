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
		void AddColumn(T value, int position = -1);

		template <typename T>
		void ChangeColumn(T value, size_t pos);

		void AddNullColumn(string type);

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
		size_t colSize = 0;
		vector<DbType*> columns;
	};

	template<typename T>
	inline void Row::ChangeColumn(T value, size_t pos)
	{
		throw NotImplementedException("No change column method implemented!");
	}

	template<>
	inline void Row::ChangeColumn(int value, size_t pos)
	{
		//OutOfRangeException?!
		columns[pos]->SetIntValue(value);
	}

	template<>
	inline void Row::ChangeColumn(double value, size_t pos)
	{
		//OutOfRangeException?!
		columns[pos]->SetDecimalValue(value);
	}

	template<>
	inline void Row::ChangeColumn(string value, size_t pos)
	{
		//OutOfRangeException?!
		columns[pos]->SetStringValue(value);
	}

	template<typename T>
	inline void Row::AddColumn(T value, int position)
	{
		throw NotImplementedException("No add column method implemented!");
	}

	template<>
	inline void Row::AddColumn(int value, int position)
	{
		DbType* cellToAdd = new Integer;
		cellToAdd->SetIntValue(value);
		
		PushToColumns(cellToAdd, position);
	}

	template<>
	inline void Row::AddColumn(double value, int position)
	{
		DbType* cellToAdd = new Decimal;
		cellToAdd->SetDecimalValue(value);
		
		PushToColumns(cellToAdd, position);
	}

	template<>
	inline void Row::AddColumn(string value, int position)
	{
		DbType* cellToAdd = new Text;
		cellToAdd->SetStringValue(value);
		
		PushToColumns(cellToAdd, position);
	}
}

#endif // !ROW
