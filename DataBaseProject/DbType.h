#ifndef DB_TYPE
#define DB_TYPE

#include <string>
#include <iostream>
#include <iomanip>
#include <string>

using std::string;
using std::ostream;
using std::istream;
using std::string;

namespace db
{
	class DbType
	{
	public:
		virtual string GetType() const = 0;
		void SetNull();
		bool CheckIfValueIsNull() const;
		virtual size_t GetValueLength() const = 0;
		virtual bool AreEqual(const DbType* other) const = 0;
		virtual void CopyValueFrom(const DbType* other) = 0;
		virtual void Serialize(ostream& outStr, size_t setWSize = 0) const = 0;
		virtual void DeSerialize(istream& inStr) = 0;

		virtual string GetValueAsString() const;
		virtual int GetValueAsInt() const;
		virtual double GetValueAsDecimal() const;

		virtual void SetStringValue(const string&);
		virtual void SetIntValue(int);
		virtual void SetDecimalValue(double);

	private:
		bool isNull;
	
	protected:
		void MakeValueNotNull();
	};
}

#endif // !DB_TYPE
