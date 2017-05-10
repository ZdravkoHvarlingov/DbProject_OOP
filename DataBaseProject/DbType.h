#ifndef DB_TYPE
#define DB_TYPE

#include <string>

using std::string;

namespace db
{
	class DbType
	{
	public:
		virtual string GetType() const = 0;

		virtual string GetValueAsString() const;
		virtual int GetValueAsInt() const;
		virtual double GetValueAsDecimal() const;

		virtual void SetStringValue(string);
		virtual void SetIntValue(int);
		virtual void SetDecimalValue(double);
	};
}

#endif // !DB_TYPE
