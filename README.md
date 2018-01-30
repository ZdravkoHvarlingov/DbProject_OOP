# DbProject_OOP
Database project

Database project - basic database operations.

*** All commands start with a capital letter without quotes.
*** All table names or text(string) cells should be typed with quotes!
*** Database types are: Text, Integer, Decimal. Type them without quotes!
*** Cells can be null - null is typed NULL in the terminal without quotes.
*** Characters \" and \\ should be escaped in the table names and text(string) cells with backslash \\ !
*** Id_auto column should not be entered. It is auto generated. You can NOT change or delete it!
"*** Self relationship is supported.
*** If no relationship file is loaded database performs as there are none of them.
*** Agrregate operations are: sum, product, maximum, minimum.

Commands:
	1) Load \"file_name\" - loads a single table from a file
	2) Showtables - prints all the table names currently loaded
	3) Describe \"table_name\" - prints the description of a table in case it is loaded.
	4) Print \"table_name\" - prints the table content in a preview mode.
	5) Save \"table_name\" \"file_name\" - saves the table content in a file.
	6) Select column_number \"table_name\" <valueToSearch> - prints rows with the selected value at column column_number.
	7) AddColumn \"table_name\" \"column_name\" column_type - adds a new column to the selected table.
	8) Update \"table_name\" colToSearch_number <valueToSearch> colToChange_number <updateValue> - updates certain rows of the table.
	9) Delete \"table_name\" colToSearch_number <valueToSearch> - deletes certain rows of the table.
	10) Insert \"table name\" <column 1> ... <column n> - insert a row in the table with the entered values.
	11) InnerJoin \"first_table\" column_number \"second_table\" column_number - makes an inner join table.
	12) LeftOuterJoin \"first_table\" column_number \"second_table\" column_number - makes a left outer join table.
	13) RightOuterJoin \"first_table\" column_number \"second_table\" column_number - makes a right outer join table.
	14) Rename \"old_table_name\" \"new_name\" - renames the table in case the name is available.
	15) CreateTable \"table_name\" - creates a table with the entered name if it is available.
	16) DeleteTable \"table_name\" - deletes the table with entered name, if it exist.
	17) Count \"table_name\" colToSearch_number <valueToSearch> - counts the rows that contain <valueToSeach> at the entered colToSearch.
	18) Aggregate \"table_name\" colToSearch_number <valueToSearch> targetColumn_number <operation> - performs the selected operation on the rows with the valueToSearch.
	19) SetColNullAcceptance \"table_name\" column_number 0(or 1) - makes NULL an acceptable(or non acceptable: 1 -yes, 0 -no) value for the specific column.
	20) SetForeignKey \"table_name\" column_number \"foreignKeyTable_name\" - makes a MANY to ONE relationship between the tables.
	21) RemoveForeignKey \"table_name\" column_number - removes the foreign key and MANY to ONE relationship.
	22) MakeManyToManyTable \"firstTable_name\" \"secondTable_name\" - makes MANY to MANY table from the table ids.
	23) SaveRelationships \"fileName\" - saves the current relationships.
	24) LoadRelationships \"fileName\" - loads the relationships if it is possible.
	25) GetRandomRowId \"tableName\" - gets a random row id from the table.
	26) DeleteColumn \"tableName\" column_number - deletes a certain column in the specified table.
	27) Exit - program exit.
