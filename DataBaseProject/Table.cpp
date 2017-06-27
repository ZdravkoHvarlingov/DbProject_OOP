#include "Table.h"
#include "NoHeaderRowException.h"
#include "InconsistentTypesException.h"
#include "NullException.h"
#include "OutOfRangeException.h"
#include <iostream>
#include <iomanip>
#include "DbTypeFactory.h"
#include "PointerWrapper.h"
#include "RelationshipException.h"
#include "Integer.h"
#include <ctime>
#include <cstdlib>

using db::DbTypeFactory;
using db::NoHeaderRowException;
using db::OutOfRangeException;
using db::NullException;
using db::RelationshipException;
using db::Integer;

db::Table::Table(string _name)
	: autoIncrement(0)
{
	headerCols.push_back(HeaderCol{
		"Id_auto",
		"Integer",
		false
	});

	SetName(_name);
}

const string& db::Table::GetName() const
{
	return name;
}

string db::Table::GetDescription() const
{
	string result = "";
	size_t len = headerCols.size();

	for (size_t ind = 0; ind < len; ind++)
	{
		result += "\"" + headerCols[ind].headerName + "\" - " + headerCols[ind].headerType + ", Accept null: ";
		if (headerCols[ind].canBeNull)
		{
			result += "YES";
		}
		else result += "NO";

		if (ind != len - 1)
		{
			result += "; ";
		}
	}

	return result;
}

vector<string> db::Table::GetColHeaders() const
{
	vector<string> result;
	size_t colsLength = headerCols.size();

	for (size_t ind = 0; ind < colsLength; ind++)
	{
		result.push_back(headerCols[ind].headerName);
	}

	return result;
}

const string & db::Table::GetColType(size_t col) const
{
	if (col >= headerCols.size() || col < 0)
	{
		throw OutOfRangeException("Search column is out of range!");
	}
	return headerCols[col].headerType;
}

size_t db::Table::GetAmountOfColumns() const
{
	return headerCols.size();
}

const vector<Row>& db::Table::GetRows() const
{
	return rows;
}

vector<size_t> db::Table::GetColumnsMaxLengths() const
{
	vector<size_t> result;
	size_t colsAmount = headerCols.size();
	size_t rowsAmount = rows.size();

	for (size_t col = 0; col< colsAmount; col++)
	{
		size_t maxSize = headerCols[col].headerName.length();
		for (size_t row = 0; row < rowsAmount; row++)
		{
			if (rows[row][col]->GetValueLength() > maxSize)
			{
				maxSize = rows[row][col]->GetValueLength();
			}
		}
		result.push_back(maxSize);
	}

	return result;
}

bool db::Table::DoesSuchIdExist(int id) const
{
	bool result = BinarySearchId(id, 0, rows.size() - 1);

	return result;
}

string db::Table::GetRelationShipConnectionsAsString() const
{
	string result = "";
	size_t amountOfCols = headerCols.size();

	for (size_t ind = 1; ind < amountOfCols; ind++)
	{
		if (headerCols[ind].foreignKeyTable != nullptr)
		{
			result += "\"" + this->GetName() + "\", foreignKey col: " + std::to_string(ind) + ", \"" +
				headerCols[ind].foreignKeyTable->GetName() + "\" -> MANY to ONE\n";
		}
	}

	return result;
}

string db::Table::SerializeRelationships(std::ostream& outStr) const
{
	string result = "";
	size_t amountOfCols = headerCols.size();

	for (size_t ind = 1; ind < amountOfCols; ind++)
	{
		if (headerCols[ind].foreignKeyTable != nullptr)
		{		
			Text tableName(GetName());
			tableName.Serialize(outStr);
			outStr << " " << ind << " ";
			tableName.SetStringValue(headerCols[ind].foreignKeyTable->GetName());
			tableName.Serialize(outStr);
			outStr << '\n';
		}
	}

	return result;
}

size_t db::Table::GetAmountOfForeignKeys() const
{
	size_t amountOfCols = headerCols.size();
	size_t foreignKeysCounter = 0;

	for (size_t ind = 1; ind < amountOfCols; ind++)
	{
		if (headerCols[ind].foreignKeyTable != nullptr)
		{
			++foreignKeysCounter;
		}
	}

	return foreignKeysCounter;
}

size_t db::Table::GetAmountOfConnectedTables() const
{
	return connectedTables.size();
}

void db::Table::SetName(string _name)
{
	name = _name;
}

void db::Table::MakeNewRow()
{
	if (headerCols.size() == 1)
	{
		throw NoHeaderRowException("Missing table header row!");
	}

	Integer autoCol;
	autoCol.SetIntValue(autoIncrement);

	Row rowToAdd;
	rowToAdd.AddColumn(&autoCol);
	
	size_t len = headerCols.size() - 1;
	for (size_t ind = 0; ind < len; ind++)
	{
		if (headerCols[ind + 1].canBeNull == false)
		{
			throw NullException("Can not make an empty row because NULL is not acceptable!");
		}
		rowToAdd.AddNullColumn(headerCols[ind + 1].headerType);
	}

	++autoIncrement;
	rows.push_back(rowToAdd);
}

void db::Table::MakeNewRow(const Row& _rowToAdd)
{
	if (headerCols.size() == 1 || _rowToAdd.GetColmSize() != headerCols.size() - 1)
	{
		throw NoHeaderRowException("Missing such table header row!");
	}

	Row rowToEnter = _rowToAdd;
	for (size_t ind = 0; ind < headerCols.size() - 1; ind++)
	{
		if (rowToEnter[ind]->GetType() != headerCols[ind + 1].headerType)
		{
			throw InconsistentTypesException("Can not convert types");
		}
		if (headerCols[ind + 1].canBeNull == false && rowToEnter[ind]->CheckIfValueIsNull())
		{
			throw NullException("Can not enter NULL cell, because it is not an acceptable value!");
		}
		if (headerCols[ind + 1].foreignKeyTable != nullptr && rowToEnter[ind]->CheckIfValueIsNull() == false &&
			headerCols[ind + 1].foreignKeyTable->DoesSuchIdExist(rowToEnter[ind]->GetValueAsInt()) == false)
		{
			throw RelationshipException("There is no such Id in the foreignKey table that you try to enter!");
		}
	}

	Integer autoCol;
	autoCol.SetIntValue(autoIncrement);

	rowToEnter.AddColumn(&autoCol, 0);
	++autoIncrement;

	rows.push_back(rowToEnter);
}

void db::Table::AddNewColumn(const string& _colName, const  string& _colType)  //, bool _canBeNull = true)
{
	HeaderCol headerColToAdd = {
		_colName,
		_colType,
		true,
		nullptr
	};

	headerCols.push_back(headerColToAdd);

	size_t rowsCount = rows.size();
	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		rows[ind].AddNullColumn(_colType);
	}
}

void db::Table::DeleteRow(size_t rowIndex)
{
	int idToDelete = rows[rowIndex][0]->GetValueAsInt();
	size_t amountOfCols = headerCols.size();

	for (size_t ind = 0; ind < amountOfCols; ind++)
	{
		if (headerCols[ind].foreignKeyTable != nullptr)
		{
			map<int, int>::iterator relatedRowToDel = headerCols[ind].relatedRows.find(idToDelete);
			if (relatedRowToDel != headerCols[ind].relatedRows.end())
			{
				headerCols[ind].relatedRows.erase(relatedRowToDel);
			}
		}
	}
	rows.erase(rows.begin() + rowIndex);
}

bool db::Table::BinarySearchId(int id, int startIndex, int endIndex) const
{
	if (startIndex > endIndex)
	{
		return false;
	}

	int middle = (startIndex + endIndex) / 2;
	if (rows[middle][0]->GetValueAsInt() == id)
	{
		return true;
	}

	if (rows[middle][0]->GetValueAsInt() > id)
	{
		return BinarySearchId(id, startIndex, middle - 1);
	}
	else return BinarySearchId(id, middle + 1, endIndex);
}

bool db::Table::DoesAMappedIdExist(map<int, int> relatedRows, int mappedId) const
{
	map<int, int>::iterator it;
	for (it = relatedRows.begin(); it != relatedRows.end(); ++it)
	{
		if (it->second == mappedId)
				return true;
	}

	return false;
}

size_t db::Table::CountCertainRows(size_t colToSearch, DbType * elementToSearch) const
{
	if (colToSearch < 0 || colToSearch >= headerCols.size())
	{
		throw OutOfRangeException("Search column is out of range!");
	}

	size_t rowsCount = rows.size();

	size_t counter = 0;

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch)) //throws exceptions if types are different
		{
			counter++;
		}
	}

	return counter;
}

void db::Table::DeleteCertainRows(size_t colToSearch, DbType * elementToSearch)
{
	if (colToSearch < 0 || colToSearch >= headerCols.size())
	{
		throw OutOfRangeException("Search column is out of range!");
	}

	size_t rowsCount = rows.size();
	vector<size_t> indexesToDelete;

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			indexesToDelete.push_back(ind);
		}
	}

	size_t amountOfIndexes = indexesToDelete.size();
	size_t amountOfConTables = connectedTables.size();
	
	for (size_t ind = 0; ind < amountOfIndexes; ind++)
	{
		for (size_t conInd = 0; conInd < amountOfConTables; conInd++)
		{
			RepairTableRelatedToThis(rows[indexesToDelete[ind]][0]->GetValueAsInt(), connectedTables[conInd]);
		}

		DeleteRow(indexesToDelete[ind]);
		for (size_t innerInd = ind + 1; innerInd < amountOfIndexes; innerInd++)
		{
			--indexesToDelete[innerInd];
		}
	}
}

void db::Table::RepairTableRelatedToThis(int idToDelete, Table * relatedTable)
{
	size_t amountOfColumns = relatedTable->headerCols.size();

	for (size_t ind = 1; ind < amountOfColumns; ind++)
	{
		if (relatedTable->headerCols[ind].foreignKeyTable == this)
		{
			if (DoesAMappedIdExist(relatedTable->headerCols[ind].relatedRows, idToDelete))
			{
				if (relatedTable->headerCols[ind].canBeNull == false)
				{
					string msg = "Can NOT delete line with ID: " + std::to_string(idToDelete) + " from table: " + this->GetName() +
						" because it is connected to table: " + relatedTable->GetName() + "! Deletion is aborted.";
					throw RelationshipException(msg);
				}
				else
				{					
					relatedTable->UpdateCertainRows(ind, &Integer(idToDelete), ind, &Integer());
				}
			}			
		}
	}
}

int db::Table::GetRandomId() const
{
	if (rows.size() == 0)
	{
		return -1;
	}

	int maxId = rows[rows.size() - 1][0]->GetValueAsInt();
	srand(time(NULL));
	int randomId = -1;
	while (!DoesSuchIdExist(randomId))
	{
		randomId = rand() % (maxId + 1);
	}

	return randomId;
}


void db::Table::UpdateCertainRows(size_t colToSearch, DbType * elementToSearch, size_t colToChange, DbType * valueToSet)
{
	if (colToSearch >= headerCols.size() || colToSearch < 0)
	{
		throw OutOfRangeException("Search column is out of range!");
	}
	if (colToChange >= headerCols.size() || colToChange < 1)
	{
		throw OutOfRangeException("Change column is out of range or not valid!");
	}
	if (headerCols[colToChange].canBeNull == false && valueToSet->CheckIfValueIsNull())
	{
		throw NullException("Can not update with NULL value, because it is not acceptable!");
	}
	if (valueToSet->CheckIfValueIsNull() == false && headerCols[colToChange].foreignKeyTable != nullptr &&
		headerCols[colToChange].foreignKeyTable->DoesSuchIdExist(valueToSet->GetValueAsInt()) == false)
	{
		throw RelationshipException("Can NOT update with that Id because it does NOT exist in the foreignKey table!");
	}

	size_t rowsCount = rows.size();

	for (size_t ind = 0; ind < rowsCount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			rows[ind][colToChange]->CopyValueFrom(valueToSet);
			if (headerCols[colToChange].foreignKeyTable != nullptr)
			{
				int idToUpdate = rows[ind][0]->GetValueAsInt();
				map<int, int>::iterator it = headerCols[colToChange].relatedRows.find(idToUpdate);
				if (valueToSet->CheckIfValueIsNull() && it != headerCols[colToChange].relatedRows.end())
				{
					headerCols[colToChange].relatedRows.erase(it);
				}
				else if(valueToSet->CheckIfValueIsNull() == false)
				{
					headerCols[colToChange].relatedRows[idToUpdate] = valueToSet->GetValueAsInt();
				}
			}
		}
	}
}

vector<Row> db::Table::SelectCertainRows(size_t colToSearch, DbType * elementToSearch) const
{
	if (colToSearch >= headerCols.size() || colToSearch < 0)
	{
		throw OutOfRangeException("Search column is out of range!");
	}

	size_t rowsAmount = rows.size();
	vector<Row> result;

	for (size_t ind = 0; ind < rowsAmount; ind++)
	{
		if (rows[ind][colToSearch]->AreEqual(elementToSearch))
		{
			result.push_back(rows[ind]);
		}
	}

	return result;
}

void db::Table::SetNullCell(size_t row, size_t col)
{
	if (headerCols[col].canBeNull)
	{
		rows[row][col]->SetNull();
	}
	else throw NullException("Can not set the cell to NULL, because it is not an acceptable value!");
}

void db::Table::SetColNullAcceptance(bool value, size_t columnInd)
{
	if (columnInd < 1 || columnInd >= headerCols.size())
	{
		throw OutOfRangeException("Column is not valid!");
	}

	if (!value)
	{
		bool hasNullRow = false;
		size_t index = 0;
		size_t amountOfRows = rows.size();

		while (index < amountOfRows && !hasNullRow)
		{
			if (rows[index][columnInd]->CheckIfValueIsNull())
			{
				hasNullRow = true;
			}

			index++;
		}

		if (!hasNullRow)
		{
			headerCols[columnInd].canBeNull = value;
		}
		else
		{
			throw NullException("Can't set null exceptance to false due to a NULL value in a certain row!");
		}
	}
	else
	{
		headerCols[columnInd].canBeNull = value;
	}
}

void db::Table::SetForeignKey(int columnIndex, Table * foreignKeyTable)
{
	if (columnIndex < 1 || columnIndex >= headerCols.size())
	{
		throw OutOfRangeException("The column is not valid for a foreign key!");
	}
	if (headerCols[columnIndex].headerType != "Integer")
	{
		throw RelationshipException("The column is not of type Integer so it can NOT be a foreign key!");
	}

	size_t rowsAmount = rows.size();

	for (size_t ind = 0; ind < rowsAmount; ind++)
	{
		if (rows[ind][columnIndex]->CheckIfValueIsNull() == false &&
			!foreignKeyTable->DoesSuchIdExist(rows[ind][columnIndex]->GetValueAsInt()))
		{
			headerCols[columnIndex].relatedRows.clear();

			throw RelationshipException(
				"You can NOT make that column a foreign key, because there are Id-s that does not exist in the foreignKey table!");
		}
		else if(rows[ind][columnIndex]->CheckIfValueIsNull() == false)
		{
			headerCols[columnIndex].relatedRows[rows[ind][0]->GetValueAsInt()] = rows[ind][columnIndex]->GetValueAsInt();
		}
	}

	headerCols[columnIndex].foreignKeyTable = foreignKeyTable;
	foreignKeyTable->connectedTables.push_back(this);
}

void db::Table::RemoveForeignKey(int columnIndex)
{
	if (columnIndex < 0 && columnIndex >= headerCols.size())
	{
		OutOfRangeException("Column is out of range!");
	}
	if (columnIndex == 0 && headerCols[columnIndex].headerType != "Integer")
	{
		throw RelationshipException("This is not a correct column! It can NOT be a foreign key.");
	}

	if (headerCols[columnIndex].foreignKeyTable != nullptr)
	{
		bool isFound = false;
		int index = 0;
		size_t amountOfRelatedTables = headerCols[columnIndex].foreignKeyTable->connectedTables.size();
		while (index < amountOfRelatedTables && !isFound)
		{
			if (headerCols[columnIndex].foreignKeyTable->connectedTables[index] == this)
			{
				headerCols[columnIndex].foreignKeyTable->connectedTables.erase(
					headerCols[columnIndex].foreignKeyTable->connectedTables.begin() + index);
				isFound = true;
			}

			++index;
		}

		headerCols[columnIndex].foreignKeyTable = nullptr;
		headerCols[columnIndex].relatedRows.clear();
	}
}

ostream & db::operator<<(ostream & outStr, const Table & tableToDisplay)
{
	outStr << tableToDisplay.name << '\n';
	size_t hCount = tableToDisplay.headerCols.size();
	outStr << hCount << "\n";
	for (size_t ind = 0; ind < hCount; ind++)
	{
		Text hdr(tableToDisplay.headerCols[ind].headerName);
		hdr.Serialize(outStr);
		outStr << " " << tableToDisplay.headerCols[ind].headerType << " " << tableToDisplay.headerCols[ind].canBeNull << " ";
	}

	outStr << '\n' << tableToDisplay.rows.size() << '\n';

	size_t len = tableToDisplay.rows.size();
	for (size_t ind = 0; ind < len - 1; ind++)
	{
		tableToDisplay.rows[ind].Serialize(outStr);
		outStr << '\n';
	}
	if (len != 0)
	{
		tableToDisplay.rows[len - 1].Serialize(outStr);
	}

	return outStr;
}

istream & db::operator>>(istream & inStr, Table & tableToInit)
{
	tableToInit.headerCols.erase(tableToInit.headerCols.begin());
	string input;
	std::getline(inStr, input); //deserialize table name
	tableToInit.SetName(input);

	int inpNumber;
	inStr >> inpNumber;
	inStr.ignore();

	Text columnText;

	vector<PointerWrapper<DbType>> colTypes;
	for (size_t ind = 0; ind < inpNumber; ind++) //deserialize columns
	{
		columnText.DeSerialize(inStr);
		inStr >> input;
		inStr.ignore();
		bool canBeNull;
		inStr >> canBeNull;
		inStr.ignore();

		tableToInit.AddNewColumn(columnText.GetValueAsString(), input);
		tableToInit.headerCols[ind].canBeNull = canBeNull;
		colTypes.push_back(PointerWrapper<DbType>(DbTypeFactory::GetNewType(input)));
	}

	inStr >> inpNumber; //ignores the new line character
	inStr.ignore();

	for (size_t ind = 0; ind < inpNumber; ind++)
	{
		Row rowToAdd;
		rowToAdd.Deserialize(inStr, colTypes); //deserialize rows
		tableToInit.rows.push_back(rowToAdd);
	}

	tableToInit.autoIncrement = tableToInit.rows[inpNumber - 1][0]->GetValueAsInt() + 1;
	return inStr;
}

db::Table db::Table::InnerJoin(size_t firstCol, const db::Table & secondTable, size_t secondCol) const
{
	db::Table result("InnerJoin: " + GetName() + ", " + secondTable.GetName());

	size_t firstTableCols = headerCols.size();
	size_t secondTableCols = secondTable.headerCols.size();

	if (firstCol < 0 || firstCol >= firstTableCols || secondCol < 0 || secondCol >= secondTableCols)
	{
		throw OutOfRangeException("Invalid inner join columns!");
	}

	for (size_t ind = 0; ind < firstTableCols; ind++)
	{
		result.AddNewColumn("FTable:" + headerCols[ind].headerName, headerCols[ind].headerType);
	}

	for (size_t ind = 0; ind < secondTableCols; ind++)
	{
		result.AddNewColumn("STable:" + secondTable.headerCols[ind].headerName, secondTable.headerCols[ind].headerType);
	}

	size_t firstTableRows = rows.size();
	size_t secondTableRows = secondTable.rows.size();
	for (size_t fInd = 0; fInd < firstTableRows; fInd++)
	{
		for (size_t sInd = 0; sInd < secondTableRows; sInd++)
		{
			if (rows[fInd][firstCol]->AreEqual(secondTable.rows[sInd][secondCol]))
			{
				Row rowToAdd;
				for (size_t fCols = 0; fCols < firstTableCols; fCols++)
				{
					rowToAdd.AddColumn(rows[fInd][fCols]);
				}

				for (size_t sCols = 0; sCols < secondTableCols; sCols++)
				{
					rowToAdd.AddColumn(secondTable.rows[sInd][sCols]);
				}

				result.MakeNewRow(rowToAdd);
			}
		}
	}

	return result;
}

db::Table db::Table::LeftOuterJoin(size_t firstCol, const Table & secondTable, size_t secondCol) const
{
	db::Table result("LeftOuterJoin: " + GetName() + ", " + secondTable.GetName());

	size_t firstTableCols = headerCols.size();
	size_t secondTableCols = secondTable.headerCols.size();

	if (firstCol < 0 || firstCol >= firstTableCols || secondCol < 0 || secondCol >= secondTableCols)
	{
		throw OutOfRangeException("Invalid join columns!");
	}

	for (size_t ind = 0; ind < firstTableCols; ind++)
	{
		result.AddNewColumn("FTable:" + headerCols[ind].headerName, headerCols[ind].headerType);
	}

	for (size_t ind = 0; ind < secondTableCols; ind++)
	{
		result.AddNewColumn("STable:" + secondTable.headerCols[ind].headerName, secondTable.headerCols[ind].headerType);
	}

	size_t firstTableRows = rows.size();
	size_t secondTableRows = secondTable.rows.size();
	for (size_t fInd = 0; fInd < firstTableRows; fInd++)
	{
		bool hasFoundMatch = false;
		for (size_t sInd = 0; sInd < secondTableRows; sInd++)
		{
			if (rows[fInd][firstCol]->AreEqual(secondTable.rows[sInd][secondCol]))
			{
				hasFoundMatch = true;

				Row rowToAdd;
				for (size_t fCols = 0; fCols < firstTableCols; fCols++)
				{
					rowToAdd.AddColumn(rows[fInd][fCols]);
				}

				for (size_t sCols = 0; sCols < secondTableCols; sCols++)
				{
					rowToAdd.AddColumn(secondTable.rows[sInd][sCols]);
				}

				result.MakeNewRow(rowToAdd);
			}
		}

		if (!hasFoundMatch)
		{
			Row rowToAdd;
			for (size_t fCols = 0; fCols < firstTableCols; fCols++)
			{
				rowToAdd.AddColumn(rows[fInd][fCols]);
			}

			for (size_t sCols = 0; sCols < secondTableCols; sCols++)
			{
				rowToAdd.AddNullColumn(secondTable.headerCols[sCols].headerType);
			}

			result.MakeNewRow(rowToAdd);
		}
	}

	return result;
}

db::Table db::Table::RightOuterJoin(size_t firstCol, const Table & secondTable, size_t secondCol) const
{
	db::Table result("RightOuterJoin: " + GetName() + ", " + secondTable.GetName());

	size_t firstTableCols = headerCols.size();
	size_t secondTableCols = secondTable.headerCols.size();

	if (firstCol < 0 || firstCol >= firstTableCols || secondCol < 0 || secondCol >= secondTableCols)
	{
		throw OutOfRangeException("Invalid join columns!");
	}

	for (size_t ind = 0; ind < firstTableCols; ind++)
	{
		result.AddNewColumn("FTable:" + headerCols[ind].headerName, headerCols[ind].headerType);
	}

	for (size_t ind = 0; ind < secondTableCols; ind++)
	{
		result.AddNewColumn("STable:" + secondTable.headerCols[ind].headerName, secondTable.headerCols[ind].headerType);
	}

	size_t firstTableRows = rows.size();
	size_t secondTableRows = secondTable.rows.size();
	for (size_t fInd = 0; fInd < secondTableRows; fInd++)
	{
		bool hasFoundMatch = false;
		for (size_t sInd = 0; sInd < firstTableRows; sInd++)
		{
			if (secondTable.rows[fInd][secondCol]->AreEqual(rows[sInd][firstCol]))
			{
				hasFoundMatch = true;

				Row rowToAdd;
				for (size_t fCols = 0; fCols < firstTableCols; fCols++)
				{
					rowToAdd.AddColumn(rows[sInd][fCols]);
				}

				for (size_t sCols = 0; sCols < secondTableCols; sCols++)
				{
					rowToAdd.AddColumn(secondTable.rows[fInd][sCols]);
				}

				result.MakeNewRow(rowToAdd);
			}
		}

		if (!hasFoundMatch)
		{
			Row rowToAdd;
			for (size_t fCols = 0; fCols < firstTableCols; fCols++)
			{
				rowToAdd.AddNullColumn(headerCols[fCols].headerType);
			}

			for (size_t sCols = 0; sCols < secondTableCols; sCols++)
			{
				rowToAdd.AddColumn(secondTable.rows[fInd][sCols]);
			}

			result.MakeNewRow(rowToAdd);
		}
	}

	return result;
}
