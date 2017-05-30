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
	void PrintHelp() const;
	void PrintTableDescription(const string& tableName) const;
	void PrintTableInPreviewMode(const string& tableName) const;
	void PrintSpecificRows(const vector<Row>& rows, size_t startingIndex, size_t amount,
		const vector<string>& headerCols, const vector<size_t>& setWSizes, size_t currPage, size_t amountOfPages) const;
	size_t GetAmountOfRowsToPrint(size_t tableIndex) const;
	void LoadTableFromFile(const char* fileName);
	void PrintTables() const;
	int GetTableIndex(const string& tableName) const;

private:

	vector<Table> loadedTables;
};

#endif // !CONSOLE_COMMAND_HANDLER
