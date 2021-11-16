#include <iostream>
#include <string>
#include "ConnectToSQLServer.h"

int main()
{
	SQLHANDLE sql_env_handle = NULL;
	SQLHANDLE sql_connection_handle = NULL;
	SQLHANDLE sql_statement_handle = NULL;
	SQLRETURN ret_code = 0;

	std::string sql_query = "select top 10 * \
from [YourDatabase].[dbo].[YourTable] with(nolock);";

	// Connect to SQL Server and execute query
	if (sql_server::Connect(sql_env_handle, sql_connection_handle, sql_statement_handle, ret_code))
	{
		if (!sql_server::Execute((char*)sql_query.c_str(), sql_statement_handle))
		{
			std::cout << "Failed to execute query.  Press Enter to exit." << std::endl;
			getchar();
			return 1;
		}
	}
	else
	{
		std::cout << "Failed to connect to SQL server.  Press Enter to exit." << std::endl;
		getchar();
		return 1;
	}

	// Fetch results and do something with the data
	// Create variables that will store the output of the select query
	char field1[16];
	char field2[128];
	while (SQLFetch(sql_statement_handle) == SQL_SUCCESS)
	{
		// Use SQL_C_DEFAULT to try to match field type or SQL_CHAR to convert everything to char
		SQLGetData(sql_statement_handle, 1, SQL_C_DEFAULT, &field1, std::size(field1), NULL);
		SQLGetData(sql_statement_handle, 2, SQL_CHAR, &field2, std::size(field2), NULL);
		std::cout << field1 << " " << field2 << std::endl;
	}

	// Close connection
	sql_server::Disconnect(sql_env_handle, sql_connection_handle, sql_statement_handle);

	std::cout << "Done.  Press Enter to exit." << std::endl;
	getchar();

	return 0;
}