#include <iostream>
#include "Text.h"
#include "Integer.h"
#include "Decimal.h"
#include "NotImplementedException.h"
#include "NoHeaderRowException.h"
#include "Row.h"
#include <string>
#include "Table.h"
#include "TableUtilities.h"
#include <limits>

using std::cout;
using db::Text;
using db::NotImplementedException;
using db::Row;
using std::string;
using db::Table;
using db::Integer;
using db::Decimal;
using db::TableUtilities;

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
	tbl.ChangeCell(5, 1, &Text("zd\\ra\\\"  vkopich \\\\"));

	tbl.UpdateCertainRows(1, &Text(), 1, &Text("NOTNULL"));
	tbl.ChangeCell(5, 3, &Decimal(-11));

	cout << tbl << "\n\n";
	Table des("test");
	std::cin >> des;
	cout << des << "\n\n";
	//cout << tbl.Aggregate<double>(1, &Text("zdravkopich"), 3, TableUtilities::MinimumOfNumbers<double>, std::numeric_limits<double>::max()) << '\n';
}