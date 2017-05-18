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

	Row rowToAdd;
	rowToAdd.AddColumn("zdravko");
	rowToAdd.AddColumn(32);
	rowToAdd.AddColumn(5.25);

	tbl.MakeNewRow(rowToAdd);
	
	rowToAdd = Row();
	rowToAdd.AddColumn("iveta");
	rowToAdd.AddColumn(20);
	rowToAdd.AddColumn(6);

	tbl.MakeNewRow(rowToAdd);

	rowToAdd = Row();
	rowToAdd.AddColumn("Dancho");
	rowToAdd.AddColumn(21);
	rowToAdd.AddColumn(6);

	tbl.MakeNewRow(rowToAdd);

	cout << tbl;
	//cout << tbl.GetDescription() << '\n';

	Decimal name;
	name.SetDecimalValue(3);
	tbl.DeleteCertainRows(1, &name);
	cout << tbl;
}