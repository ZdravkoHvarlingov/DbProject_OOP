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

	test.AddColumn<string>("string test");
	test.AddColumn<int>(10);
	test.AddColumn<double>(12.3123);

	vector<string> headers;
	headers.push_back("Name");
	headers.push_back("Age");
	headers.push_back("FN");
	Table tbl("Test table", headers);

	tbl.MakeNewRow();
	tbl.MakeNewRow(test);
	delete tbl[0][0];
	tbl[0][0] = new Text;
	tbl[0][0]->SetStringValue("That is bullshit darling");

	cout << tbl;
}