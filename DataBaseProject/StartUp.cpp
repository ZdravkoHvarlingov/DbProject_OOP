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
	Row test;

	test.AddColumn<string>("stringName");
	test.AddColumn<int>(10);

	Table tbl("TestTable");
	tbl.AddNewColumn("name", "Text", true);
	tbl.AddNewColumn("age", "Integer", true);

	tbl.MakeNewRow();
	tbl.MakeNewRow(test);


	///table.DeleteRow();
	///

	cout << tbl;
}