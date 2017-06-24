#ifndef CONSOLE_COMMAND_HANDLER
#define CONSOLE_COMMAND_HANDLER

#include <vector>
#include "Table.h"

using db::Table;
using std::vector;

class ConsoleCommandHandler
{
public:

	void StartListening();

	void RightOuterJoinSwitchFunc();

	void LeftOuterJoinSwitchFunc();

	double AggregateSpecificCommand(const string &command, const string &targetColumnType, int tableIndex, const Integer &searchColumn,
		PointerWrapper<db::DbType> &searchValue, const Integer &targetColumn, bool &isValidCommand);

	void PrintHelp() const;
	void PrintTableDescription(const string& tableName) const;
	void PrintTableInPreviewMode(const string& tableName, const vector<Row>& tableRows) const;
	void PrintSpecificRows(const vector<Row>& rows, size_t startingIndex, size_t amount,
		const vector<string>& headerCols, const vector<size_t>& setWSizes, size_t currPage, size_t amountOfPages) const;
	size_t GetAmountOfRowsToPrint(size_t tableIndex) const;
	void LoadTableFromFile(const char* fileName);
	void SaveTableToFile(const char* fileName, const string& tableName);
	void AddColumn(const string& tableName, const string& colName, const string& type);
	void PrintTables() const;
	int GetTableIndex(const string& tableName) const;

private:

	void AggregateSwitchFunc();
	void CountSwitchFunc();
	void DeleteTableSwitchFunc();
	void CreateTableSwitchFunc();
	void RenameTableSwitchFunc();
	void InnerJoinSwitchFunc();
	void InsertSwitchFunc();
	void DeleteSwitchFunc();
	void UpdateSwitchFunc();
	void AddColumnSwitchFunc();
	void SaveTableSwitchFunc();
	void DescribeTableFunc();
	void LoadTableSwitchFunc(std::string &input);
	void PrintPreviewSwitchFunc(std::string &input);
	void SelectSwitchFunc();

	vector<Table> loadedTables;
};

#endif // !CONSOLE_COMMAND_HANDLER
