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
	void PrintTableDescription(const string& tableName) const;
	void PrintTableInPreviewMode(const string& tableName) const;
	void LoadTableFromFile(const char* fileName);
	void PrintTables() const;

private:

	vector<Table> loadedTables;
};

#endif // !CONSOLE_COMMAND_HANDLER
