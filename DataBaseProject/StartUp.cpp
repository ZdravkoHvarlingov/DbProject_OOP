#include <iostream>
#include "Text.h"
#include "Integer.h"
#include "Decimal.h"
#include "NotImplementedException.h"
#include "NoHeaderRowException.h"
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
	tbl.MakeNewRow(row);
	tbl.MakeNewRow(copy);
	tbl.MakeNewRow();

	Table second = tbl;
	second.SetName("second table");
	tbl.ChangeCell(1, 1, &Text("novoime-idiot"));
	tbl.ChangeCell(2, 1, &Text("okay"));
	tbl.ChangeCell(0, 2, &Integer(12));
	cout << tbl;

	cout << tbl.CountCertainRows(1, &Text("zdravkopich")) << '\n';
	tbl.UpdateCertainRows(1, &Text("zdravkopich"), 3, &Decimal(6.11));
	tbl.UpdateCertainRows(1, &Text(), 1, &Text("NOTNULL"));
	cout << tbl;

	tbl.DeleteRow(2);
	cout << tbl;
}