#include "ConsoleCommandHandler.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Text.h"
#include <windows.h>
#include <conio.h>

using std::cout;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::string;
using db::Text;

void ConsoleCommandHandler::StartListening()
{
	cout << "db > Welcome to database system!\ndb > For any help type \"Help\"\n\ndb > ";

	string input;
	cin >> input;
	cin.ignore();

	while (input != "Exit")
	{
		if (input == "Describe")
		{
			try
			{
				Text tableName;
				tableName.DeSerialize(cin);

				PrintTableDescription(tableName.GetValueAsString());
			}
			catch (const std::exception& e)
			{
				cout << "db > Invalid table name. " << e.what() << '\n';
				cin >> input;
			}
		}
		else if (input == "Showtables")
		{
			PrintTables();
		}
		else if (input == "Load")
		{
			try 
			{
				Text fileName;
				fileName.DeSerialize(cin);

				LoadTableFromFile(fileName.GetValueAsString().c_str());
			}
			catch (const std::exception& e)
			{
				cout << "db > Invalid file name. " << e.what() << '\n';
				cin >> input;
			}
		}
		else if (input == "Print")
		{
			try
			{
				Text fileName;
				fileName.DeSerialize(cin);

				PrintTableInPreviewMode(fileName.GetValueAsString().c_str());
			}
			catch (const std::exception& e)
			{
				cout << "db > Invalid file table name. " << e.what() << '\n';
				cin >> input;
			}
		}
		else if (input == "Help")
		{
			PrintHelp();
		}
		else if (input == "Exit")
		{
			break;
		}
		else
		{
			cout << "Invalid command! You have entered something additional or something wrong.\n";
		}


		cout << "db > ";
		cin >> input;
		cin.ignore();
	}

	cout << "db > Program exit...\n";
}

void ConsoleCommandHandler::PrintHelp() const
{
	cout << "*** All commands start with a capital letter without quotes.\n" << 
		"*** All table names or text(string) cells should be typed with quotes!\n" <<
		"*** Characters \" and \\ should be escaped in the table names and text(string) cells with backslash \\!\n\n" <<
		"Commands:\n" <<
		"1) Load \"file_name\" - loads a single table from a file\n" <<
		"2) Showtables - prints all the table names currently loaded\n" <<
		"3) Describe \"table_name\" - prints the description of a table in case it is loaded.\n" <<
		"4) Print \"table_name\" - prints the table content in a preview mode.\n\n";
}

void ConsoleCommandHandler::PrintTableDescription(const string & tableName) const
{
	size_t len = loadedTables.size();
	bool isFound = false;

	size_t ind = 0;
	while (ind < len && !isFound)
	{
		if (tableName == loadedTables[ind].GetName())
		{
			cout << "db > " << loadedTables[ind].GetDescription() << '\n';
			isFound = true;
		}

		++ind;
	}

	if (!isFound)
	{
		cout << "db > No such table found!\n";
	}
}

void ConsoleCommandHandler::PrintTableInPreviewMode(const string & tableName) const
{
	int tableInd = GetTableIndex(tableName);

	if (tableInd == -1)
	{
		cout << "db > No such table found!\n";
		return;
	}

	system("cls");
	vector<Row> tableRows = loadedTables[tableInd].GetRows();
	if (tableRows.size() == 0)
	{
		cout << "db > Table has no rows to display!\n";
		return;
	}

	vector<string> tableHeaders = loadedTables[tableInd].GetColHeaders();
	vector<size_t> setWSizes = loadedTables[tableInd].GetColumnsMaxLengths();
	
	size_t amountOfRowsToPrint = GetAmountOfRowsToPrint(tableInd);
	size_t pages = tableRows.size() / amountOfRowsToPrint;
	if (tableRows.size() % amountOfRowsToPrint != 0)
	{
		++pages;
	}

	size_t currPage = 1;
	size_t amountOfPrintedRows = amountOfRowsToPrint;
	PrintSpecificRows(tableRows, 0, amountOfRowsToPrint, tableHeaders, setWSizes, currPage, pages);
		
	char keyboardKey = _getch();

	while (keyboardKey != 'q')
	{
		if (keyboardKey == 'a' && currPage > 1)
		{
			amountOfPrintedRows -= 2 * amountOfRowsToPrint;
			--currPage;
			system("cls");
			PrintSpecificRows(tableRows, amountOfPrintedRows, amountOfRowsToPrint, tableHeaders, setWSizes, currPage, pages);
			amountOfPrintedRows += amountOfRowsToPrint;
		}
		else if (keyboardKey == 'd' && currPage < pages)
		{
			++currPage;
			system("cls");
			PrintSpecificRows(tableRows, amountOfPrintedRows, amountOfRowsToPrint, tableHeaders, setWSizes, currPage, pages);
			amountOfPrintedRows += amountOfRowsToPrint;
		}
		
		keyboardKey = _getch();
	}

	system("cls");
}

void ConsoleCommandHandler::PrintSpecificRows(const vector<Row>& rows, size_t startingIndex, size_t amount,
							const vector<string>& headerCols, const vector<size_t>& setWSizes, size_t currPage, size_t amountOfPages) const
{
	if (startingIndex >= rows.size() || startingIndex < 0)
	{
		return;
	}

	size_t amountOfCols = headerCols.size();
	for (size_t ind = 0; ind < amountOfCols; ind++)
	{
		cout << std::left << std::setw((int)setWSizes[ind]) << headerCols[ind];
		if (ind != amountOfCols - 1)
		{
			cout << " ";
		}
	}
	cout << "\n\n";

	size_t upperLimit = startingIndex + amount;
	if (upperLimit > rows.size())
	{
		upperLimit = rows.size();
	}
	for (size_t ind = startingIndex; ind < upperLimit; ind++)
	{
		cout << rows[ind].GetAsString(setWSizes) << '\n';
	}
	cout << '\n' << "page " << currPage << " of " << amountOfPages << "\n" << "\"a\" - previous page, \"d\" - next page, \"q\" - quit\n" ;
}

size_t ConsoleCommandHandler::GetAmountOfRowsToPrint(size_t tableIndex) const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	vector<size_t> sizes = loadedTables[tableIndex].GetColumnsMaxLengths();
	size_t rowLength = 0;
	for (size_t ind = 0; ind < sizes.size(); ind++)
	{
		rowLength += sizes[ind] + 1;
	}

	size_t consoleRowsForTableRow = rowLength / columns;
	if (rowLength % columns != 0)
	{
		++consoleRowsForTableRow;
	}

	rows -= 4 + consoleRowsForTableRow + 1; // space for last two rows that are going to be info and two(or more) for header row and one for page row

	return rows / consoleRowsForTableRow;
}

void ConsoleCommandHandler::LoadTableFromFile(const char * fileName)
{
	ifstream inFile(fileName);

	try
	{
		Table tableToGet;
		inFile >> tableToGet;

		size_t tablesAmount = loadedTables.size();
		bool isAlreadyLoaded = false;
		size_t ind = 0;
		while (ind < tablesAmount && !isAlreadyLoaded)
		{
			if (loadedTables[ind].GetName() == tableToGet.GetName())
			{
				isAlreadyLoaded = true;
			}

			ind++;
		}

		if (!isAlreadyLoaded)
		{
			loadedTables.push_back(tableToGet);
			cout << "db > Table successfully loaded!\n";
		}
		else
		{
			cout << "db > Such tables with name \"" << tableToGet.GetName() << "\" is already loaded!\n";
		}

	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid table file. " << e.what() << "\n";
	}

	inFile.close();
}

void ConsoleCommandHandler::PrintTables() const
{
	size_t tablesAmount = loadedTables.size();

	if (tablesAmount == 0)
	{
		cout << "db > No tables loaded!\n";
		return;
	}
	else cout << "db > ";

	for (size_t ind = 0; ind < tablesAmount - 1; ind++)
	{
		cout << "\"" << loadedTables[ind].GetName() << "\", ";
	}

	cout << "\"" << loadedTables[tablesAmount - 1].GetName() << "\"\n";

}

int ConsoleCommandHandler::GetTableIndex(const string & tableName) const
{
	size_t len = loadedTables.size();
	bool isFound = false;

	size_t ind = 0;
	while (ind < len && !isFound)
	{
		if (tableName == loadedTables[ind].GetName())
		{
			isFound = true;
		}
		++ind;
	}

	if (!isFound)
	{
		return -1;
	}

	return --ind;
}
