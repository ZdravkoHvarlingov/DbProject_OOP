#include <iostream>
#include "Text.h"
#include "NotImplementedException.h"
#include "Row.h"
#include <string>
#include "Table.h"

using std::cout;
using db::Text;
using db::NotImplementedException;
using db::Row;
using std::string;
using db::Table;

int main()
{
	Table tbl("Test table");
	tbl.AddNewColumn("Name", "Text");
	tbl.AddNewColumn("Age", "Integer");
	tbl.AddNewColumn("Grade", "Decimal");

	cout << tbl;
	//cout << tbl.GetDescription() << '\n';

	//Decimal name;
	//name.SetDecimalValue(3);
	//tbl.DeleteCertainRows(1, &name);
	//cout << tbl;
}