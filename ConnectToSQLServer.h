#pragma once

#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include "password.h"

namespace sql_server
{
// Capture errors
void ShowError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLWCHAR sql_state[1024]; // array of characters
	SQLWCHAR message[1024];
	if (SQLGetDiagRec(handleType, handle, 1, sql_state, NULL, message, 1024, NULL) == SQL_SUCCESS)
	{
		std::wcout << "SQL driver message: " << message <<
			"\nSQL state: " << sql_state << std::endl;
	}
}

void GetUserInfo(std::string& user_id, std::string& pwd)
{
	std::cin.clear();
	std::cout << "Enter SQL Server user ID: " << std::endl;
	std::getline(std::cin, user_id);
	std::cin.clear();
	std::cout << "Enter password: " << std::endl;
	pwd = password::GetPassword();
}

bool Connect(SQLHANDLE& sql_env_handle,
	SQLHANDLE& sql_connection_handle,
	SQLHANDLE& sql_statement_handle,
	SQLRETURN& ret_code)
{
	do
	{
		// Allocate environment
		if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sql_env_handle) != SQL_SUCCESS)
			break;

		// Set environment attributes
		if (SQLSetEnvAttr(sql_env_handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS)
			break;

		// Allocate connection (store in sql_connection_handle)
		if (SQLAllocHandle(SQL_HANDLE_DBC, sql_env_handle, &sql_connection_handle) != SQL_SUCCESS)
			break;

		// Set connection timeout attribute
		if (SQLSetConnectAttr(sql_connection_handle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0) != SQL_SUCCESS)
			break;

		// Connect to SQL Server
		SQLCHAR ret_con_string[1024];
		std::string server = "YOUR_SERVER.com";
		std::string database = "DOESNT_REALLY_MATTER";
		std::string user_id, pwd;
		GetUserInfo(user_id, pwd);
		std::string conn_params = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=" +
			server + ";DATABASE=" + database +
			";UID=NOANT\\" + user_id + ";PWD=" + pwd +
			";Trusted_Connection=yes;";

		switch (SQLDriverConnectA(sql_connection_handle, NULL, (SQLCHAR*)conn_params.c_str(),
			SQL_NTS, ret_con_string, 1024, NULL, SQL_DRIVER_NOPROMPT))
		{
		case SQL_SUCCESS:
			break;
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_NO_DATA_FOUND:
			std::cout << "SQL_NO_DATA_FOUND" << std::endl;
			ShowError(SQL_HANDLE_DBC, sql_connection_handle);
			ret_code = -1;
			break;
		case SQL_INVALID_HANDLE:
			std::cout << "SQL_INVALID_HANDLE" << std::endl;
			ShowError(SQL_HANDLE_DBC, sql_connection_handle);
			ret_code = -1;
			break;
		case SQL_ERROR:
			std::cout << "SQL_ERROR" << std::endl;
			ShowError(SQL_HANDLE_DBC, sql_connection_handle);
			ret_code = -1;
			break;
		default:
			break;
		}
		
		if (ret_code == -1)
			break;

		if (SQLAllocHandle(SQL_HANDLE_STMT, sql_connection_handle, &sql_statement_handle) != SQL_SUCCESS)
		{
			std::cout << "SQLAllocHandle() failed." << std::endl;
			break;
		}

		return true;

	} while (false);

	// Only happens if we broke out of the loop
	return false;
}

bool Execute(char* sql_query, SQLHANDLE& sql_statement_handle)
{
	std::cout << "Executing SQL query..." << std::endl;
	if (SQLExecDirectA(sql_statement_handle, (SQLCHAR*)sql_query, SQL_NTS) != SQL_SUCCESS)
	{
		ShowError(SQL_HANDLE_STMT, sql_statement_handle);
		return false;
	}
	return true;
}

void Disconnect(
	SQLHANDLE& sql_env_handle,
	SQLHANDLE& sql_connection_handle,
	SQLHANDLE& sql_statement_handle
)
{
	// Free all resources and disconnect
	SQLFreeHandle(SQL_HANDLE_STMT, sql_statement_handle);
	SQLDisconnect(sql_connection_handle);
	SQLFreeHandle(SQL_HANDLE_DBC, sql_connection_handle);
	SQLFreeHandle(SQL_HANDLE_ENV, sql_env_handle);
}

} // namespace sql
