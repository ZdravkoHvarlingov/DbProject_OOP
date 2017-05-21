#ifndef TABLE_UTILITIES
#define TABLE_UTILITIES

#include <iostream>
#include <string>

using std::istream;
using std::string;

namespace db
{
	class TableUtilities
	{
	public:

		template <typename T>
		static T SumOfNumbers(T accumotatedByNow, T number);

		template <typename T>
		static T ProductOfNumbers(T accumotatedByNow, T number);

		template <typename T>
		static T MaximumOfNumbers(T maxByNow, T number);

		template <typename T>
		static T MinimumOfNumbers(T minByNow, T number);
	};

	template<typename T>
	inline T TableUtilities::SumOfNumbers(T accumotatedByNow, T number)
	{
		return accumotatedByNow + number;
	}

	template<typename T>
	inline T TableUtilities::ProductOfNumbers(T accumotatedByNow, T number)
	{
		return accumotatedByNow * number;
	}

	template<typename T>
	inline T TableUtilities::MaximumOfNumbers(T maxByNow, T number)
	{
		if (number > maxByNow)
		{
			maxByNow = number;
		}

		return maxByNow;
	}

	template<typename T>
	inline T TableUtilities::MinimumOfNumbers(T minByNow, T number)
	{
		if (number < minByNow)
		{
			minByNow = number;
		}

		return minByNow;
	}
}

#endif // !TABLE_UTILITIES
