#ifndef DB_TYPE
#define DB_TYPE

#include <string>
#include <iostream>

using std::ostream;
using std::string;

namespace db
{
	class DbType
	{
	public:
		virtual const char* GetType() const = 0;
		void SetNull();
		bool CheckIfValueIsNull() const;
		virtual bool AreEqual(DbType* other) const = 0;
		virtual void CopyValueFrom(DbType* other) = 0;
		virtual void Serialize(ostream& outStr) const = 0;

		virtual string GetValueAsString() const;
		virtual int GetValueAsInt() const;
		virtual double GetValueAsDecimal() const;

		virtual void SetStringValue(string);
		virtual void SetIntValue(int);
		virtual void SetDecimalValue(double);

	private:
		bool isNull;
	
	protected:
		void MakeValueNotNull();
	};
}

#endif // !DB_TYPE
