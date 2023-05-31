# SQLServerExampleCPP
Sample C++ code to connect to a Microsoft SQL Server database, run a select query and print the output to console on Windows.

Before compiling:
  * Fill out the connection parameters in ConnectToSQLServer.h (lines 58 - 59)
  * Double-check the name of the ODBC driver (ConnectToSQLServer.h, line 62)
  * In main.cpp (line 12), edit in the actual SQL query that the program should run.  The name of the database in the query does not need to match the database specified in the connection parameters.
  * In main.cpp (lines 34 - 42), make sure that the number of fields and their types match what you are getting from the select query.
  
  The only purpose of password.h is to hide the user password as it is being entered in the console.
