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
	ConsoleCommandHandler& commandHandler = ConsoleCommandHandler::GetInstance();
	
	commandHandler.StartListening();
}