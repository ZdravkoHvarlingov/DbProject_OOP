#include "ConsoleCommandHandler.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Text.h"

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
		else if (input == "Exit")
		{
			break;
		}


		cout << "db > ";
		cin >> input;
		cin.ignore();
	}

	cout << "db > Program exit...\n";
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
		cout << "db > No such table found!\n";
		return;
	}

	vector<Row> tableRows = loadedTables[ind - 1].GetRows();
	vector<size_t> setWSizes = loadedTables[ind - 1].GetColumnsMaxLengts();

	for (size_t ind = 0; ind < tableRows.size(); ind++)
	{
		cout << tableRows[ind].GetAsString(setWSizes) << '\n';
	}
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
