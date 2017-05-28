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
#include "ConsoleCommandHandler.h"
#include <limits>
#include <fstream>
#include <windows.h>
#include <iomanip>

using std::cout;
using db::Text;
using db::NotImplementedException;
using db::Row;
using std::string;
using db::Table;
using db::Integer;
using db::Decimal;
using db::TableUtilities;
using std::ofstream;
using std::setw;

int main()
{

	//CONSOLE_SCREEN_BUFFER_INFO csbi;
	//int columns, rows;

	//GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	//columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	//rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	//string msg = "asda";
	//cout << std::left << setw(0) << msg << setw(0) << msg << '\n';
	//cout << setw(0) << msg << '\n';
	//cout << std::setprecision(3) << std::fixed << 121312543 << '\n';

	//cout << tbl.Aggregate<double>(1, &Text("zdravkopich"), 3, TableUtilities::MinimumOfNumbers<double>, std::numeric_limits<double>::max()) << '\n';

	ConsoleCommandHandler commandHandler;
	commandHandler.StartListening();
}