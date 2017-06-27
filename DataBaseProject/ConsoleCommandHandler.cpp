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
#include "PointerWrapper.h"
#include "TableUtilities.h"
#include <limits>

using std::cout;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::string;
using db::Text;

ConsoleCommandHandler & ConsoleCommandHandler::GetInstance()
{
	static ConsoleCommandHandler instance;

	return instance;
}

void ConsoleCommandHandler::StartListening()
{
	cout << "db > Welcome to database system!\ndb > For any help type \"Help\"\n\ndb > ";

	string input;
	cin >> input;

	while (input != "Exit")
	{
		if (input == "Describe")
		{
			DescribeTableFunc();
		}
		else if (input == "Showtables")
		{
			PrintTables();
		}
		else if (input == "Load")
		{
			LoadTableSwitchFunc(input);
		}
		else if (input == "CreateTable")
		{
			CreateTableSwitchFunc();
		}
		else if (input == "DeleteTable")
		{
			DeleteTableSwitchFunc();
		}
		else if (input == "Save")
		{
			SaveTableSwitchFunc();
		}
		else if (input == "Print")
		{
			PrintPreviewSwitchFunc(input);
		}
		else if (input == "Select")
		{
			SelectSwitchFunc();
		}
		else if (input == "AddColumn")
		{
			AddColumnSwitchFunc();
		}
		else if (input == "Update")
		{
			UpdateSwitchFunc();
		}
		else if(input == "Delete")
		{
			DeleteSwitchFunc();
		}
		else if (input == "Insert")
		{
			InsertSwitchFunc();
		}
		else if (input == "InnerJoin")
		{
			InnerJoinSwitchFunc();
		}
		else if (input == "LeftOuterJoin")
		{
			LeftOuterJoinSwitchFunc();
		}
		else if (input == "RightOuterJoin")
		{
			RightOuterJoinSwitchFunc();
		}
		else if (input == "Rename")
		{
			RenameTableSwitchFunc();
		}
		else if (input == "Count")
		{
			CountSwitchFunc();
		}
		else if (input == "Aggregate")
		{
			AggregateSwitchFunc();
		}
		else if (input == "SetColNullAcceptance")
		{
			SetColNullAcceptanceSwitchFunc();
		}
		else if (input == "SetForeignKey")
		{
			SetForeignKeySwitchFunc();
		}
		else if (input == "RemoveForeignKey")
		{
			RemoveForeignKeySwitchFunc();
		}
		else if (input == "MakeManyToManyTable")
		{
			MakeManyToManyTableSwitchFunc();
		}
		else if (input == "ShowRelationships")
		{
			PrintRelationshipSystem();
		}
		else if (input == "SaveRelationships")
		{
			SaveRelationshipsSwitchFunc();
		}
		else if (input == "LoadRelationships")
		{
			LoadRelationshipsSwitchFunc();
		}
		else if (input == "GetRandomRowId")
		{
			GetRandomRowIdSwitchFunc();
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

void ConsoleCommandHandler::GetRandomRowIdSwitchFunc() const
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);

		int tableIndex = GetTableIndex(tableName.GetValueAsString());
		if (tableIndex != -1)
		{
			int randomId = loadedTables[tableIndex]->GetRandomId();
			if (randomId == -1)
			{
				cout << "db > There are no table rows!\n";
			}
			else cout << "db > The random id is: " << randomId << ".\n";
		}
	}
	catch (const std::exception& e)
	{
		cout << "Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::LoadRelationshipsSwitchFunc()
{
	cin.ignore();

	try
	{
		Text inputText;
		inputText.DeSerialize(cin);
		string fileName = inputText.GetValueAsString();

		LoadRelationships(fileName.c_str());
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::SaveRelationshipsSwitchFunc()
{
	cin.ignore();

	try
	{
		Text inputText;
		inputText.DeSerialize(cin);
		string fileName = inputText.GetValueAsString();

		SerializeRelationships(fileName.c_str());
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::MakeManyToManyTableSwitchFunc()
{
	cin.ignore();

	try
	{
		Text firstTableName;
		firstTableName.DeSerialize(cin);
		cin.ignore();
		int firstTableIndex = GetTableIndex(firstTableName.GetValueAsString());

		if (firstTableIndex != -1)
		{
			Text secondTableName;
			secondTableName.DeSerialize(cin);
			int secondTableIndex = GetTableIndex(secondTableName.GetValueAsString());

			if (secondTableIndex != -1)
			{
				Table* manyToManyTable = new Table("ManyToMany: " + loadedTables[firstTableIndex]->GetName() +
					", " + loadedTables[secondTableIndex]->GetName());
				manyToManyTable->AddNewColumn(loadedTables[firstTableIndex]->GetName() + "Id", "Integer");
				manyToManyTable->AddNewColumn(loadedTables[secondTableIndex]->GetName() + "Id", "Integer");
				loadedTables.push_back(manyToManyTable);
				loadedTables[loadedTables.size() - 1]->SetColNullAcceptance(false, 1);
				loadedTables[loadedTables.size() - 1]->SetColNullAcceptance(false, 2);
				loadedTables[loadedTables.size() - 1]->SetForeignKey(1, loadedTables[firstTableIndex]);
				loadedTables[loadedTables.size() - 1]->SetForeignKey(2, loadedTables[secondTableIndex]);
				cout << "db > ManyToMany table was created with name: \"" << manyToManyTable->GetName() << "\".\n";
			}
			else
			{
				cout << "db > The second table is not valid!\n";
			}
		}
		else
		{
			cout << "db > The first table is not valid!\n";
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::RemoveForeignKeySwitchFunc()
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
			Integer column;
			column.DeSerialize(cin);

			loadedTables[tableIndex]->RemoveForeignKey(column.GetValueAsInt());
			cout << "db > ForeignKey removed successfully!\n";
		}
		else cout << "db > There is no such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::SetForeignKeySwitchFunc()
{
	cin.ignore();
	try
	{
		Text firstTable;
		firstTable.DeSerialize(cin);
		cin.ignore();
		int firstTableIndex = GetTableIndex(firstTable.GetValueAsString());

		if (firstTableIndex != -1)
		{
			Integer foreignKeyCol;
			foreignKeyCol.DeSerialize(cin);
			cin.ignore();

			Text secondTable;
			secondTable.DeSerialize(cin);
			int secondTableIndex = GetTableIndex(secondTable.GetValueAsString());
			if (secondTableIndex != -1)
			{
				loadedTables[firstTableIndex]->SetForeignKey(foreignKeyCol.GetValueAsInt(), loadedTables[secondTableIndex]);
				cout << "db > Relationship set successfully!\n";
			}
			else
			{
				cout << "db > The second table is not valid!\n";
			}
		}
		else cout << "db > The first table is not valid!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid command arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::SetColNullAcceptanceSwitchFunc()
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
			Integer columnToSet;
			columnToSet.DeSerialize(cin);
			cin.ignore();

			bool value;
			cin >> value;

			loadedTables[tableIndex]->SetColNullAcceptance(value, columnToSet.GetValueAsInt());
			cout << "db > Column NULL acceptance successfully set to: " << value << '\n';
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

void ConsoleCommandHandler::RightOuterJoinSwitchFunc()
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
			Table* rightOuterJoin = new Table();
			*rightOuterJoin = loadedTables[firstTableIndex]->RightOuterJoin(firstCol.GetValueAsInt(),
				*(loadedTables[secondTableIndex]), secondCol.GetValueAsInt());

			int innerJoinIndex = GetTableIndex(rightOuterJoin->GetName());
			if (innerJoinIndex == -1)
			{
				loadedTables.push_back(rightOuterJoin);
				cout << "db > Table made successfully. Its name is: \"" << rightOuterJoin->GetName() << "\".\n";
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

void ConsoleCommandHandler::LeftOuterJoinSwitchFunc()
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
			Table* leftOuterJoin = new Table();
			*leftOuterJoin = loadedTables[firstTableIndex]->LeftOuterJoin(firstCol.GetValueAsInt(),
				*(loadedTables[secondTableIndex]), secondCol.GetValueAsInt());

			int innerJoinIndex = GetTableIndex(leftOuterJoin->GetName());
			if (innerJoinIndex == -1)
			{
				loadedTables.push_back(leftOuterJoin);
				cout << "db > Table made successfully. Its name is: \"" << leftOuterJoin->GetName() << "\".\n";
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

void ConsoleCommandHandler::AggregateSwitchFunc() const
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
			Integer searchColumn;
			searchColumn.DeSerialize(cin);
			cin.ignore();

			string searchColType = loadedTables[tableIndex]->GetColType(searchColumn.GetValueAsInt());
			PointerWrapper<DbType> searchValue(db::DbTypeFactory::GetNewType(searchColType));
			searchValue->DeSerialize(cin);
			cin.ignore();

			Integer targetColumn;
			targetColumn.DeSerialize(cin);
			cin.ignore();

			string command;
			cin >> command;

			bool isValidCommand = true;
			string targetColumnType = loadedTables[tableIndex]->GetColType(targetColumn.GetValueAsInt());
			double result = AggregateSpecificCommand(command, targetColumnType, tableIndex,
				searchColumn, searchValue, targetColumn, isValidCommand);

			if (isValidCommand)
			{
				cout << "db > The result is: " << result << '\n';
			}
			else
			{
				cout << "db > Invalid operation!\n";
			}
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

void ConsoleCommandHandler::SerializeRelationships(const char * fileName) const
{
	ofstream outFile;
	try
	{
		outFile.open(fileName);
		size_t amountOfLoadedTables = loadedTables.size();

		vector<int> connectedTables;
		size_t amountOfForeignKeys = 0;
		for (size_t ind = 0; ind < amountOfLoadedTables; ind++)
		{
			size_t foreignKeys = loadedTables[ind]->GetAmountOfForeignKeys();
			if (foreignKeys != 0)
			{
				connectedTables.push_back(ind);
				amountOfForeignKeys += foreignKeys;
			}
			else if(loadedTables[ind]->GetAmountOfConnectedTables() != 0)
			{
				connectedTables.push_back(ind);
			}
		}

		outFile << connectedTables.size() << '\n';
		for (size_t ind = 0; ind < connectedTables.size(); ind++)
		{
			outFile << loadedTables[connectedTables[ind]]->GetName() << '\n';
		}
		if (amountOfForeignKeys != 0)
		{
			outFile << amountOfForeignKeys << '\n';
		}
		for (size_t ind = 0; ind < connectedTables.size(); ind++)
		{
			if (loadedTables[ind]->GetAmountOfForeignKeys() != 0)
			{
				loadedTables[connectedTables[ind]]->SerializeRelationships(outFile);
			}
		}
		
		cout << "db > Relationships saved successfully!\n";
	}
	catch (const std::exception&)
	{
		cout << "db > Problem with opening the file or saving the relationships!\n";
	}

	outFile.close();
}

void ConsoleCommandHandler::LoadRelationships(const char * fileName)
{
	ifstream inFile;

	try
	{
		inFile.open(fileName);

		int amountOfTables;
		inFile >> amountOfTables;
		inFile.ignore();
		if (amountOfTables > 0)
		{
			vector<string> tableNames;
			for (size_t ind = 0; ind < amountOfTables; ind++)
			{
				string input;
				std::getline(inFile, input);
				tableNames.push_back(input);
			}

			size_t tableIndexesCount = 0;
			for (size_t ind = 0; ind < amountOfTables; ind++)
			{
				int tableIndex = GetTableIndex(tableNames[ind]);
				if (tableIndex != -1)
				{
					++tableIndexesCount;
					cout << "db > Table successfully recognized: " << tableNames[ind] << '\n';
				}
				else
				{
					cout << "db > Missing table: " << tableNames[ind] << '\n';
				}
			}

			if (tableNames.size() != tableIndexesCount)
			{
				cout << "db > Some table are missing. Aborting command...\n";
			}
			else
			{
				int amountOfRelations;
				inFile >> amountOfRelations;
				inFile.ignore();
				if (amountOfRelations > 0)
				{
					for (size_t ind = 0; ind < amountOfRelations; ind++)
					{
						Text tableName;
						tableName.DeSerialize(inFile);
						int fistTableIndex = GetTableIndex(tableName.GetValueAsString());
						
						int column;
						inFile >> column;
						inFile.ignore();
						
						tableName.DeSerialize(inFile);
						inFile.ignore();
						int secondTableIndex = GetTableIndex(tableName.GetValueAsString());

						if (secondTableIndex == -1 || fistTableIndex == -1)
						{
							throw std::invalid_argument("Invalid relationships!");
						}

						loadedTables[fistTableIndex]->SetForeignKey(column, loadedTables[secondTableIndex]);
					}
					cout << "db > Relationships loaded successfully!\n";

				}
				else cout << "db > No relationships to load!\n";
			}
		}
		else
		{
			cout << "db > There are no relationships to load!\n";
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid relationship file. " << e.what() << "\n";
	}

	inFile.close();
}

double ConsoleCommandHandler::AggregateSpecificCommand(const string &command, const string &targetColumnType, int tableIndex,
	const Integer &searchColumn, PointerWrapper<db::DbType> &searchValue, const Integer &targetColumn, bool &isValidCommand) const
{
	double result = 0;
	if (command == "sum")
	{
		if (targetColumnType == "Decimal")
		{
			result = loadedTables[tableIndex]->Aggregate<double>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::SumOfNumbers, 0.0);
		}
		else
		{
			result = loadedTables[tableIndex]->Aggregate<int>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::SumOfNumbers, 0);
		}
	}
	else if (command == "product")
	{
		if (targetColumnType == "Decimal")
		{
			result = loadedTables[tableIndex]->Aggregate<double>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::ProductOfNumbers, 1.0);
		}
		else
		{
			result = loadedTables[tableIndex]->Aggregate<int>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::ProductOfNumbers, 1);
		}
	}
	else if (command == "maximum")
	{
		if (targetColumnType == "Decimal")
		{
			result = loadedTables[tableIndex]->Aggregate<double>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::MaximumOfNumbers, std::numeric_limits<double>::min());
		}
		else
		{
			result = loadedTables[tableIndex]->Aggregate<int>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::MaximumOfNumbers, std::numeric_limits<int>::min());
		}
	}
	else if (command == "minimum")
	{
		if (targetColumnType == "Decimal")
		{
			result = loadedTables[tableIndex]->Aggregate<double>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::MinimumOfNumbers, std::numeric_limits<double>::max());
		}
		else
		{
			result = loadedTables[tableIndex]->Aggregate<int>(searchColumn.GetValueAsInt(), searchValue.operator->(),
				targetColumn.GetValueAsInt(), db::TableUtilities::MinimumOfNumbers, std::numeric_limits<int>::max());
		}
	}
	else
	{
		isValidCommand = false;
	}

	return result;
}

void ConsoleCommandHandler::CountSwitchFunc() const
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
			Integer searchColumn;
			searchColumn.DeSerialize(cin);
			cin.ignore();

			string type = loadedTables[tableIndex]->GetColType(searchColumn.GetValueAsInt());
			PointerWrapper<DbType> valueToDeserialize(db::DbTypeFactory::GetNewType(type));
			valueToDeserialize->DeSerialize(cin);

			int counter = loadedTables[tableIndex]->CountCertainRows(searchColumn.GetValueAsInt(), valueToDeserialize.operator->());
			cout << "db > Amount of such rows: " << counter << '\n';
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

void ConsoleCommandHandler::DeleteTableSwitchFunc()
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);
		int tableIndex = GetTableIndex(tableName.GetValueAsString());

		if (tableIndex != -1)
		{
			size_t amountOfColumns = loadedTables[tableIndex]->GetAmountOfColumns();
			for (size_t ind = 1; ind < amountOfColumns; ind++)
			{
				if (loadedTables[tableIndex]->GetColType(ind) == "Integer")
				{
					loadedTables[tableIndex]->RemoveForeignKey(ind);
				}
			}

			delete loadedTables[tableIndex];
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

void ConsoleCommandHandler::CreateTableSwitchFunc()
{
	cin.ignore();

	try
	{
		Text tableName;
		tableName.DeSerialize(cin);

		if (GetTableIndex(tableName.GetValueAsString()) == -1)
		{
			loadedTables.push_back(new Table(tableName.GetValueAsString()));
			cout << "db > Table created successfully!\n";
		}
		else cout << "db > There is already such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::RenameTableSwitchFunc()
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
				loadedTables[tblIndex]->SetName(tableName.GetValueAsString());
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

void ConsoleCommandHandler::InnerJoinSwitchFunc()
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
			Table* innerJoin = new Table();
			*innerJoin = loadedTables[firstTableIndex]->InnerJoin(firstCol.GetValueAsInt(),
				*loadedTables[secondTableIndex], secondCol.GetValueAsInt());

			int innerJoinIndex = GetTableIndex(innerJoin->GetName());
			if (innerJoinIndex == -1)
			{
				loadedTables.push_back(innerJoin);
				cout << "db > Table made successfully. Its name is: \"" << innerJoin->GetName() << "\".\n";
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

void ConsoleCommandHandler::InsertSwitchFunc()
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
			size_t amountOfCols = loadedTables[tableIndex]->GetAmountOfColumns();

			vector<PointerWrapper<DbType>> valueTypes;
			for (size_t ind = 1; ind < amountOfCols; ind++)
			{
				valueTypes.push_back(
					PointerWrapper<DbType>(db::DbTypeFactory::GetNewType(loadedTables[tableIndex]->GetColType(ind)))); //dynamically allocated
			}

			Row valuesToGet;
			valuesToGet.Deserialize(cin, valueTypes);
			cin.putback('\n');
			loadedTables[tableIndex]->MakeNewRow(valuesToGet);

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

void ConsoleCommandHandler::DeleteSwitchFunc()
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

			PointerWrapper<DbType> valueToSearch(db::DbTypeFactory::GetNewType(
				loadedTables[tableIndex]->GetColType(searchCol.GetValueAsInt())));
			valueToSearch->DeSerialize(cin);

			loadedTables[tableIndex]->DeleteCertainRows(searchCol.GetValueAsInt(), valueToSearch.operator->());

			cout << "db > Rows were deleted successfully!\n";
		}
		else cout << "db > There is no such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::UpdateSwitchFunc()
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

			string searchColType = loadedTables[tableIndex]->GetColType(colToSearch.GetValueAsInt());
			PointerWrapper<DbType> valueToSearch(db::DbTypeFactory::GetNewType(searchColType));
			valueToSearch->DeSerialize(cin);
			cin.ignore();

			Integer colToChange;
			colToChange.DeSerialize(cin);
			cin.ignore();

			string typeToChange = loadedTables[tableIndex]->GetColType(colToChange.GetValueAsInt());
			PointerWrapper<DbType> valueToChangeWith(db::DbTypeFactory::GetNewType(typeToChange));

			valueToChangeWith->DeSerialize(cin);
			//no need of cin.ignore();

			loadedTables[tableIndex]->UpdateCertainRows(colToSearch.GetValueAsInt(), valueToSearch.operator->(),
				colToChange.GetValueAsInt(), valueToChangeWith.operator->());

			cout << "db > Table successfully updated!\n";
		}
		else cout << "db > There is no such table!\n";
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid arguments! " << e.what() << '\n';
	}
}

void ConsoleCommandHandler::AddColumnSwitchFunc()
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

void ConsoleCommandHandler::DescribeTableFunc() const
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

void ConsoleCommandHandler::LoadTableSwitchFunc(std::string &input)
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

void ConsoleCommandHandler::PrintPreviewSwitchFunc(std::string &input) const
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
			PrintTableInPreviewMode(tableName.GetValueAsString(), loadedTables[tableIndex]->GetRows());
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid file table name. " << e.what() << '\n';
		cin >> input;
	}
}

void ConsoleCommandHandler::SelectSwitchFunc() const
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
			else if (columnToSearch >= loadedTables[tableInd]->GetAmountOfColumns() || columnToSearch < 0)
			{
				cout << "db > Column is out of range!\n";
			}
			else
			{
				string columnType = loadedTables[tableInd]->GetColType(columnToSearch);
				PointerWrapper<DbType> typeToDeserialize(db::DbTypeFactory::GetNewType(columnType));
				typeToDeserialize->DeSerialize(cin);

				PrintTableInPreviewMode(tableName.GetValueAsString(),
					loadedTables[tableInd]->SelectCertainRows(columnToSearch, typeToDeserialize.operator->()));
			}
		}
	}
	catch (const std::exception& e)
	{
		cout << "db > Invalid Select command arguments! " << e.what() << '\n';
		cin.clear();
	}
}

void ConsoleCommandHandler::SaveTableSwitchFunc()
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
		"*** Id_auto column should not be entered. It is auto generated. You can NOT change or delete it!\n"
		"*** Self relationship is supported.\n"
		"*** If no relationship file is loaded database performs as there are none of them.\n"
		"*** Agrregate operations are: sum, product, maximum, minimum.\n\n" <<
		"Commands:\n"
		"1) Load \"file_name\" - loads a single table from a file\n"
		"2) Showtables - prints all the table names currently loaded\n"
		"3) Describe \"table_name\" - prints the description of a table in case it is loaded.\n"
		"4) Print \"table_name\" - prints the table content in a preview mode.\n"
		"5) Save \"table_name\" \"file_name\" - saves the table content in a file.\n"
		"6) Select column_number \"table_name\" <valueToSearch> - prints rows with the selected value at column column_number.\n"
		"7) AddColumn \"table_name\" \"column_name\" column_type - adds a new column to the selected table.\n"
		"8) Update \"table_name\" colToSearch_number <valueToSearch> colToChange_number <updateValue> - updates certain rows of the table.\n"
		"9) Delete \"table_name\" colToSearch_number <valueToSearch> - deletes certain rows of the table.\n"
		"10) Insert \"table name\" <column 1> ... <column n> - insert a row in the table with the entered values.\n"
		"11) InnerJoin \"first_table\" column_number \"second_table\" column_number - makes an inner join table.\n"
		"12) LeftOuterJoin \"first_table\" column_number \"second_table\" column_number - makes a left outer join table.\n"
		"13) RightOuterJoin \"first_table\" column_number \"second_table\" column_number - makes a right outer join table.\n"
		"14) Rename \"old_table_name\" \"new_name\" - renames the table in case the name is available.\n"
		"15) CreateTable \"table_name\" - creates a table with the entered name if it is available.\n"
		"16) DeleteTable \"table_name\" - deletes the table with entered name, if it exist.\n"
		"17) Count \"table_name\" colToSearch_number <valueToSearch> - counts the rows that contain <valueToSeach> at the entered colToSearch.\n"
		"18) Aggregate \"table_name\" colToSearch_number <valueToSearch> targetColumn_number <operation> - performs the selected operation on the rows with the valueToSearch.\n"
		"19) SetColNullAcceptance \"table_name\" column_number 0(or 1) - makes NULL an acceptable(or non acceptable: 1 -yes, 0 -no) value for the specific column.\n"
		"20) SetForeignKey \"table_name\" column_number \"foreignKeyTable_name\" - makes a MANY to ONE relationship between the tables.\n"
		"21) RemoveForeignKey \"table_name\" column_number - removes the foreign key and MANY to ONE relationship.\n"
		"22) MakeManyToManyTable \"firstTable_name\" \"secondTable_name\" - makes MANY to MANY table from the table ids.\n"
		"23) SaveRelationships \"fileName\" - saves the current relationships.\n"
		"24) LoadRelationships \"fileName\" - loads the relationships if it is possible.\n"
		"25) GetRandomRowId \"tableName\" - gets a random row id from the table.\n"
		"26) Exit - program exit.\n\n";
}

void ConsoleCommandHandler::PrintRelationshipSystem() const
{
	cout << "db > RelationShip system:\n";

	size_t amountOfTables = loadedTables.size();
	for (size_t ind = 0; ind < amountOfTables; ind++)
	{
		string result = loadedTables[ind]->GetRelationShipConnectionsAsString();
		if (result != "")
		{
			cout << result;
		} 
	}
}

void ConsoleCommandHandler::PrintTableDescription(const string & tableName) const
{
	int tableIndex = GetTableIndex(tableName);
	if (tableIndex == -1)
	{
		cout << "db > No such table found!\n";
		return;
	}

	cout << "db > " << loadedTables[tableIndex]->GetDescription() << '\n';
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

	vector<string> tableHeaders = loadedTables[tableInd]->GetColHeaders();
	vector<size_t> setWSizes = loadedTables[tableInd]->GetColumnsMaxLengths();
	
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

	vector<size_t> sizes = loadedTables[tableIndex]->GetColumnsMaxLengths();
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

		Table* tableToGet = new Table();
		inFile >> *tableToGet;

		int tableIndex = GetTableIndex(tableToGet->GetName());

		if (tableIndex == -1)
		{
			loadedTables.push_back(tableToGet);
			cout << "db > Table successfully loaded!\n";
		}
		else
		{
			cout << "db > Such tables with name \"" << tableToGet->GetName() << "\" is already loaded!\n";
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
			outFile << *(loadedTables[tableIndex]);
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

	loadedTables[tableIndex]->AddNewColumn(colName, type);
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
		cout << "\"" << loadedTables[ind]->GetName() << "\", ";
	}

	cout << "\"" << loadedTables[tablesAmount - 1]->GetName() << "\"\n";

}

int ConsoleCommandHandler::GetTableIndex(const string & tableName) const
{
	size_t len = loadedTables.size();
	bool isFound = false;

	size_t ind = 0;
	while (ind < len && !isFound)
	{
		if (tableName == loadedTables[ind]->GetName())
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

ConsoleCommandHandler::~ConsoleCommandHandler()
{
	size_t amountOfTables = loadedTables.size();

	for (size_t ind = 0; ind < amountOfTables; ind++)
	{
		delete loadedTables[ind];
	}
}
