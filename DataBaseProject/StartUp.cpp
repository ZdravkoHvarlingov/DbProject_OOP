#include <iostream>
#include "Text.h"
#include "Integer.h"
#include "Decimal.h"
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
using db::Integer;
using db::Decimal;

int main()
{
	Table tbl("Test table");
	tbl.AddNewColumn("Name", "Text");
	tbl.AddNewColumn("Age", "Integer");
	tbl.AddNewColumn("Grade", "Decimal");

	Row row;
	row.AddColumn(&Text("zdravkopich"));
	row.AddColumn(&Integer(11));
	row.AddColumn(&Decimal(1.56));
	Row copy = row;

	row.ChangeColumnValue(0, &Text("qskaTA"));
	tbl.MakeNewRow();
	tbl.MakeNewRow(row);
	tbl.MakeNewRow(copy);
	cout << tbl;

	Table second = tbl;
	second.SetName("second table");
	tbl.ChangeCell(1, 1, &Text("novoime-idiot"));
	
	cout << tbl;
	cout << second;

}