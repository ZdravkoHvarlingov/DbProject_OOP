#define NOMINMAX //because of windows.h library

#include "ConsoleCommandHandler.h"
#include "DbTypeFactory.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Text.h"
#include <windows.h>
#include <conio.h>
#include <limits>

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

	while (input != "Exit")
	{
		if (input == "Describe")
		{
			DescribeFunc();
		}
		else if (input == "Showtables")
		{
			PrintTables();
		}
		else if (input == "Load")
		{
			LoadFunc(input);
		}
		else if (input == "CreateTable")
		{
			CreateTableFunc();
		}
		else if (input == "DeleteTable")
		{
			DeleteTableFunc();
		}
		else if (input == "Save")
		{
			SaveFunc();
		}
		else if (input == "Print")
		{
			PrintPreviewFunc(input);
		}
		else if (input == "Select")
		{
			SelectFunc();
		}
		else if (input == "AddColumn")
		{
			AddColumnFunc();
		}
		else if (input == "Update")
		{
			UpdateFunc();
		}
		else if(input == "Delete")
		{
			DeleteFunc();
		}
		else if (input == "Insert")
		{
			InsertFunc();
		}
		else if (input == "InnerJoin")
		{
			InnerJoinFunc();
		}
		else if (input == "Rename")
		{
			RenameFunc();
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
			cout << "db > Invalid command! You have entered something additional or something wrong.\n";
		}

		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clears everything to the end of line

		cout << "db > ";
		cin >> input;
	}

	cout << "db > Program exit...\n";
}

void ConsoleCommandHandler::DeleteTableFunc()
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);
		int tableIndex = GetTableIndex(tableName.GetValueAsString());

		if (tableIndex != -1)
		{
			loadedTables.erase(loadedTables.begin() + tableIndex);
			cout << "db > Table deleted successfully!\n";
		}
		else cout << "db > There is NO such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::CreateTableFunc()
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);

		if (GetTableIndex(tableName.GetValueAsString()) == -1)
		{
			loadedTables.push_back(Table(tableName.GetValueAsString()));
			cout << "db > Table created successfully!\n";
		}
		else cout << "db > There is already such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::RenameFunc()
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);
		cin.ignore();

		int tblIndex = GetTableIndex(tableName.GetValueAsString());
		if (tblIndex != -1)
		{
			tableName.DeSerialize(cin);

			int newNameIndex = GetTableIndex(tableName.GetValueAsString());
			if (newNameIndex == -1)
			{
				loadedTables[tblIndex].SetName(tableName.GetValueAsString());
				cout << "db > Table renamed successfully!\n";
			}
			else cout << "db > A table with the new name already exist!\n";
		}
		else
		{
			cout << "db > There is no such table!\n";
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::InnerJoinFunc()
{
	cin.ignore();

	try
	{
		Text firstTableName;
		firstTableName.DeSerialize(cin);
		cin.ignore();

		Integer firstCol;
		firstCol.DeSerialize(cin);
		cin.ignore();

		Text secondTableName;
		secondTableName.DeSerialize(cin);
		cin.ignore();

		Integer secondCol;
		secondCol.DeSerialize(cin);

		int firstTableIndex = GetTableIndex(firstTableName.GetValueAsString());
		int secondTableIndex = GetTableIndex(secondTableName.GetValueAsString());

		if (firstTableIndex != -1 && secondTableIndex != -1)
		{
			Table innerJoin = InnerJoin(loadedTables[firstTableIndex], firstCol.GetValueAsInt(),
				loadedTables[secondTableIndex], secondCol.GetValueAsInt());

			int innerJoinIndex = GetTableIndex(innerJoin.GetName());
			if (innerJoinIndex == -1)
			{
				loadedTables.push_back(innerJoin);
				cout << "db > Table made successfully. Its name is: \"" << innerJoin.GetName() << "\".\n";
			}
			else cout << "db > There is already such table! Rename or delete it in order to execute the command!\n";
		}
		else cout << "db > Invalid tables!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::InsertFunc()
{
	cin.ignore(); //removes white space after Insert string

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);
		cin.ignore();

		int tableIndex = GetTableIndex(tableName.GetValueAsString());
		if (tableIndex != -1)
		{
			size_t amountOfCols = loadedTables[tableIndex].GetAmountOfColumns();

			vector<DbType*> valueTypes;
			for (size_t ind = 1; ind < amountOfCols; ind++)
			{
				valueTypes.push_back(db::DbTypeFactory::GetNewType(loadedTables[tableIndex].GetColType(ind))); //dynamically allocated
			}

			Row valuesToGet;
			valuesToGet.Deserialize(cin, valueTypes);
			cin.putback('\n');
			loadedTables[tableIndex].MakeNewRow(valuesToGet);

			for (size_t ind = 0; ind < amountOfCols - 1; ind++)
			{
				delete valueTypes[ind];
			}
			cout << "db > Table row added successfully!\n";
		}
		else
		{
			cout << "db > There is no such table!\n";
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << "\n";
	}
}

void ConsoleCommandHandler::DeleteFunc()
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);
		cin.ignore();

		int tableIndex = GetTableIndex(tableName.GetValueAsString());
		if (tableIndex != -1)
		{
			Integer searchCol;
			searchCol.DeSerialize(cin);
			cin.ignore();

			DbType* valueToSearch = db::DbTypeFactory::GetNewType(
				loadedTables[tableIndex].GetColType(searchCol.GetValueAsInt()));
			valueToSearch->DeSerialize(cin);

			loadedTables[tableIndex].DeleteCertainRows(searchCol.GetValueAsInt(), valueToSearch);

			delete valueToSearch;
			cout << "db > Rows were deleted successfully!\n";
		}
		else cout << "db > There is no such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::UpdateFunc()
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);
		cin.ignore();

		int tableIndex = GetTableIndex(tableName.GetValueAsString());
		if (tableIndex != -1)
		{
			Integer colToSearch;
			colToSearch.DeSerialize(cin);
			cin.ignore();

			string searchColType = loadedTables[tableIndex].GetColType(colToSearch.GetValueAsInt());
			DbType* valueToSearch = db::DbTypeFactory::GetNewType(searchColType);
			valueToSearch->DeSerialize(cin);
			cin.ignore();

			Integer colToChange;
			colToChange.DeSerialize(cin);
			cin.ignore();

			string typeToChange = loadedTables[tableIndex].GetColType(colToChange.GetValueAsInt());
			DbType* valueToChangeWith = db::DbTypeFactory::GetNewType(typeToChange);

			valueToChangeWith->DeSerialize(cin);
			//no need of cin.ignore();

			loadedTables[tableIndex].UpdateCertainRows(colToSearch.GetValueAsInt(), valueToSearch,
				colToChange.GetValueAsInt(), valueToChangeWith);

			delete valueToSearch;
			delete valueToChangeWith;
			cout << "db > Table successfully updated!\n";
		}
		else cout << "db > There is no such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::AddColumnFunc()
{
	cin.get(); // remove whitespace that >> operator ignores after AddColumn

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);
		cin.ignore();

		Text colName;
		colName.DeSerialize(cin);
		cin.ignore();

		string colType;
		cin >> colType;

		AddColumn(tableName.GetValueAsString(), colName.GetValueAsString(), colType);
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::DescribeFunc()
{
	cin.ignore();
	try
	{
		Text tableName;
		tableName.DeSerialize(cin);

		PrintTableDescription(tableName.GetValueAsString());
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid table name. " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::LoadFunc(std::string &input)
{
	cin.ignore();
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

void ConsoleCommandHandler::PrintPreviewFunc(std::string &input)
{
	cin.ignore();
	try
	{
		Text tableName;
		tableName.DeSerialize(cin);

		int tableIndex = GetTableIndex(tableName.GetValueAsString());

		if (tableIndex == -1)
		{
			cout << "db > There is no such table!\n";
		}
		else
		{
			PrintTableInPreviewMode(tableName.GetValueAsString(), loadedTables[tableIndex].GetRows());
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid file table name. " << e.what() << '\n';
		cin >> input;
	}
}

void ConsoleCommandHandler::SelectFunc()
{
	cin.ignore();
	try
	{
		size_t columnToSearch;
		cin >> columnToSearch;
		if (cin.fail())
		{
			cout << "db > Invalid column!\n";
			cin.clear();
		}
		else
		{
			cin.ignore();

			Text tableName;
			tableName.DeSerialize(cin);
			int tableInd = GetTableIndex(tableName.GetValueAsString());
			cin.ignore();

			if (tableInd == -1)
			{
				cout << "db > There is no such table!\n";
			}
			else if (columnToSearch >= loadedTables[tableInd].GetAmountOfColumns() || columnToSearch < 0)
			{
				cout << "db > Column is out of range!\n";
			}
			else
			{
				string columnType = loadedTables[tableInd].GetColType(columnToSearch);
				DbType* typeToDeserialize = db::DbTypeFactory::GetNewType(columnType);
				typeToDeserialize->DeSerialize(cin);

				PrintTableInPreviewMode(tableName.GetValueAsString(),
					loadedTables[tableInd].SelectCertainRows(columnToSearch, typeToDeserialize));

				delete typeToDeserialize;
			}
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid Select command arguments! " << e.what() << '\n';
		cin.clear();
	}
}

void ConsoleCommandHandler::SaveFunc()
{
	cin.ignore();
	try
	{
		Text inputText;
		inputText.DeSerialize(cin);
		string tableName = inputText.GetValueAsString();

		cin.ignore();
		inputText.DeSerialize(cin);
		string fileName = inputText.GetValueAsString();

		SaveTableToFile(fileName.c_str(), tableName);
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid file name " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::PrintHelp() const
{
	cout << "*** All commands start with a capital letter without quotes.\n" << 
		"*** All table names or text(string) cells should be typed with quotes!\n" <<
		"*** Database types are: Text, Integer, Decimal. Type them without quotes!\n" <<
		"*** Cells can be null - null is typed NULL in the terminal without quotes.\n"
		"*** Characters \" and \\ should be escaped in the table names and text(string) cells with backslash \\ !\n" 
		"*** Id_auto column should not be entered. It is auto generated. Do not change or delete it!\n\n"<<
		"Commands:\n" 
		"1) Load \"file_name\" - loads a single table from a file\n" 
		"2) Showtables - prints all the table names currently loaded\n" 
		"3) Describe \"table_name\" - prints the description of a table in case it is loaded.\n" 
		"4) Print \"table_name\" - prints the table content in a preview mode.\n" 
		"5) Save \"table_name\" \"file_name\" - saves the table content in a file.\n"
		"6) Select column_number \"table_name\" value_to_search - prints rows with the selected value at column column_number.\n"
		"7) AddColumn \"table_name\" \"column_name\" column_type - adds a new column to the selected table.\n"
		"8) Update \"table_name\" colToSearch_number <valueToSearch> colToChange_number <updateValue> - updates certain rows of the table.\n"
		"9) Delete \"table_name\" colToSearch_number <valueToSearch> - deletes certain rows of the table.\n"
		"10) Insert \"table name\" <column 1> … <column n> - insert a row in the table with the entered values.\n"
		"11) InnerJoin \"first_table\" <column> \"second_table\" <column> - makes a inner join table.\n"
		"12) Rename \"old_table_name\" \"new_name\" - renames the table in case the name is available.\n"
		"13) CreateTable \"table_name\" - creates a table with the entered name if it is available.\n"
		"14) DeleteTable \"table_name\" - deletes the table with entered name, if it exist.\n"
		"15) Exit - program exit.\n\n";
}

void ConsoleCommandHandler::PrintTableDescription(const string & tableName) const
{
	int tableIndex = GetTableIndex(tableName);
	if (tableIndex == -1)
	{
		cout << "db > No such table found!\n";
		return;
	}

	cout << "db > " << loadedTables[tableIndex].GetDescription() << '\n';
}

void ConsoleCommandHandler::PrintTableInPreviewMode(const string & tableName, const vector<Row>& tableRows) const
{
	if (tableRows.size() == 0)
	{
		cout << "db > There are no rows to print!\n";
		return;
	}
	
	system("cls");
	int tableInd = GetTableIndex(tableName);

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
	ifstream inFile;

	try
	{
		inFile.open(fileName);

		Table tableToGet;
		inFile >> tableToGet;

		int tableIndex = GetTableIndex(tableToGet.GetName());

		if (tableIndex == -1)
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

void ConsoleCommandHandler::SaveTableToFile(const char * fileName, const string & tableName)
{
	ofstream outFile;
	try
	{
		outFile.open(fileName);

		int tableIndex = GetTableIndex(tableName);
		if (tableIndex != -1)
		{
			outFile << loadedTables[tableIndex];
			cout << "db > Table successfully saved!\n";
		}
		else
		{
			cout << "db > There is no such table to save!\n";
		}
	}
	catch (const std::exception&)
	{
		cout << "db > Problem with opening the file or saving the table!\n";
	}

	outFile.close();
}

void ConsoleCommandHandler::AddColumn(const string & tableName, const string & colName, const string & type)
{
	int tableIndex = GetTableIndex(tableName);

	if (tableIndex == -1)
	{
		cout << "db > There is no such table!\n";
		return;
	}

	if (type != "Integer" && type != "Decimal" && type != "Text")
	{
		cout << "db > Invalid column type! Use Help command for more information.\n";
		return;
	}

	loadedTables[tableIndex].AddNewColumn(colName, type);
	cout << "db > Column successfully added!\n";
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
