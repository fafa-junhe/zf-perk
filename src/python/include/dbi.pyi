from typing import Any, list, Callable, Union
from .handles import *


class DBBindType:
    """Describes binding types."""
    DBBind_Float: int = ...
    DBBind_Int: int = ...
    DBBind_String: int = ...


class DBPriority:
    """Threading priority level."""
    DBPrio_High: int = ...
    DBPrio_Low: int = ...
    DBPrio_Normal: int = ...


class DBResult:
    """Describes a database field fetch status."""
    DBVal_Data: int = ...
    DBVal_Error: int = ...
    DBVal_Null: int = ...
    DBVal_TypeMismatch: int = ...


SQLTxnSuccess = Union[
    Callable[[Any, Any, int, list[Any], list[Any]], None],
    Callable[[Any, Any, int, list[Any], list[Any]], None]
]
def GetIdentifier(ident: str, maxlength: int) -> None:
    pass
def GetProduct(product: str, maxlength: int) -> None:
    pass
def FetchMoreResults() -> bool:
    pass
def get() -> Any:
    pass
def FieldNumToName(field: int, name: str, maxlength: int) -> None:
    pass
def FieldNameToNum(name: str, field: int) -> bool:
    pass
def FetchRow() -> bool:
    pass
def Rewind() -> bool:
    pass
def FetchString(field: int, buffer: str, maxlength: int, result: DBResult = ...) -> int:
    pass
def FetchFloat(field: int, result: DBResult = ...) -> float:
    pass
def FetchInt(field: int, result: DBResult = ...) -> int:
    pass
def IsFieldNull(field: int) -> bool:
    pass
def FetchSize(field: int) -> int:
    pass
def Transaction() -> Any:
    pass
def AddQuery(query: str, data: Any = ...) -> int:
    pass
def BindInt(param: int, number: int, signed: bool = ...) -> None:
    pass
def BindFloat(param: int, value: float) -> None:
    pass
def BindString(param: int, value: str, copy: bool) -> None:
    pass
def SetCharset(charset: str) -> bool:
    pass
def Escape(string: str, buffer: str, maxlength: int, written: int = ...) -> bool:
    pass
def Format(buffer: str, maxlength: int, format: str, _0: Any, *args: Any) -> int:
    pass
def IsSameConnection(other: Any) -> bool:
    pass
def Query(callback: Any, query: str, data: Any = ..., prio: DBPriority = ...) -> None:
    pass
def Execute(txn: Any, onSuccess: SQLTxnSuccess = ..., onError: Any = ..., data: Any = ..., priority: DBPriority = ...) -> None:
    pass
def SQL_Connect(confname: str, persistent: bool, error: str, maxlength: int) -> Any:
    """Creates an SQL connection from a named configuration.

@param confname      Named configuration.
@param persistent    True to re-use a previous persistent connection if
                     possible, false otherwise.
@param error         Error buffer.
@param maxlength     Maximum length of the error buffer.
@return              A database connection Handle, or INVALID_HANDLE on failure."""
    pass
def SQL_DefConnect(error: str, maxlength: int, persistent: bool = ...) -> Any:
    """Creates a default SQL connection.

@param error         Error buffer.
@param maxlength     Maximum length of the error buffer.
@param persistent    True to re-use a previous persistent connection
                     if possible, false otherwise.
@return              A database connection Handle, or INVALID_HANDLE on failure.
                     On failure the error buffer will be filled with a message."""
    pass
def SQL_ConnectCustom(keyvalues: Any, error: str, maxlength: int, persistent: bool) -> Any:
    """Connects to a database using key value pairs containing the database info.
The key/value pairs should match what would be in databases.cfg.

I.e. "driver" should be "default" or a driver name (or omitted for 
the default).  For SQLite, only the "database" parameter is needed in addition.
For drivers which require external connections, more of the parameters may be 
needed.

In general it is discouraged to use this function.  Connections should go through 
databases.cfg for greatest flexibility on behalf of users.

@param keyvalues     Key/value pairs from a KeyValues handle, describing the connection.
@param error         Error buffer.
@param maxlength     Maximum length of the error buffer.
@param persistent    True to re-use a previous persistent connection if
                     possible, false otherwise.
@return              A database connection Handle, or INVALID_HANDLE on failure.
                     On failure the error buffer will be filled with a message.
@error               Invalid KeyValues handle."""
    pass
def SQLite_UseDatabase(database: str, error: str, maxlength: int) -> Any:
    """Grabs a handle to an SQLite database, creating one if it does not exist.  

Unless there are extenuating circumstances, you should consider using "sourcemod-local" as the 
database name.  This provides some unification between plugins on behalf of users.

As a precaution, you should always create some sort of unique prefix to your table names so 
there are no conflicts, and you should never drop or modify tables that you do not own.

@param database      Database name.  
@param error         Error buffer.
@param maxlength     Maximum length of the error buffer.
@return              A database connection Handle, or INVALID_HANDLE on failure.
                     On failure the error buffer will be filled with a message."""
    pass
def SQL_ConnectEx(driver: Any, host: str, user: str, ___pass: str, database: str, error: str, maxlength: int, persistent: bool = ..., port: int = ..., maxTimeout: int = ...) -> Any:
    pass
def SQL_CheckConfig(name: str) -> bool:
    """Returns if a named configuration is present in databases.cfg.

@param name          Configuration name.
@return              True if it exists, false otherwise."""
    pass
def SQL_GetDriver(name: str = ...) -> Any:
    """Returns a driver Handle from a name string.

If the driver is not found, SourceMod will attempt
to load an extension named dbi.<name>.ext.[dll|so].

@param name          Driver identification string, or an empty
                     string to return the default driver.
@return              Driver Handle, or INVALID_HANDLE on failure."""
    pass
def SQL_ReadDriver(database: Any, ident: str = ..., ident_length: int = ...) -> Any:
    """Reads the driver of an opened database.

@param database      Database Handle.
@param ident         Option buffer to store the identification string.
@param ident_length  Maximum length of the buffer.
@return              Driver Handle."""
    pass
def SQL_GetDriverIdent(driver: Any, ident: str, maxlength: int) -> None:
    """Retrieves a driver's identification string.

Example: "mysql", "sqlite"

@param driver        Driver Handle, or INVALID_HANDLE for the default driver.
@param ident         Identification string buffer.
@param maxlength     Maximum length of the buffer.
@error               Invalid Handle other than INVALID_HANDLE."""
    pass
def SQL_GetDriverProduct(driver: Any, product: str, maxlength: int) -> None:
    """Retrieves a driver's product string.

Example: "MySQL", "SQLite"

@param driver        Driver Handle, or INVALID_HANDLE for the default driver.
@param product       Product string buffer.
@param maxlength     Maximum length of the buffer.
@error               Invalid Handle other than INVALID_HANDLE."""
    pass
def SQL_SetCharset(database: Any, charset: str) -> bool:
    """Sets the character set of the current connection. 
Like SET NAMES .. in mysql, but stays after connection problems.

Example: "utf8", "latin1"

@param database      Database Handle.
@param charset       The character set string to change to.
@return              True, if character set was changed, false otherwise."""
    pass
def SQL_GetAffectedRows(hndl: Any) -> int:
    """Returns the number of affected rows from the last query.

@param hndl          A database OR statement Handle.
@return              Number of rows affected by the last query.
@error               Invalid database or statement Handle."""
    pass
def SQL_GetInsertId(hndl: Any) -> int:
    """Returns the last query's insertion id.

@param hndl          A database, query, OR statement Handle.
@return              Last query's insertion id.
@error               Invalid database, query, or statement Handle."""
    pass
def SQL_GetError(hndl: Any, error: str, maxlength: int) -> bool:
    """Returns the error reported by the last query.

@param hndl          A database, query, OR statement Handle.
@param error         Error buffer.
@param maxlength     Maximum length of the buffer.
@return              True if there was an error, false otherwise.
@error               Invalid database, query, or statement Handle."""
    pass
def SQL_EscapeString(database: Any, string: str, buffer: str, maxlength: int, written: int = ...) -> bool:
    """Escapes a database string for literal insertion.  This is not needed
for binding strings in prepared statements.  

Generally, database strings are inserted into queries enclosed in 
single quotes (').  If user input has a single quote in it, the 
quote needs to be escaped.  This function ensures that any unsafe 
characters are safely escaped according to the database engine and 
the database's character set.

NOTE: SourceMod only guarantees properly escaped strings when the query
encloses the string in single quotes. While drivers tend to allow double
quotes (") instead, the string may be not be escaped (for example, on SQLite)!

@param database      A database Handle.
@param string        String to quote.
@param buffer        Buffer to store quoted string in.
@param maxlength     Maximum length of the buffer.
@param written       Optionally returns the number of bytes written.
@return              True on success, false if buffer is not big enough.
                     The buffer must be at least 2*strlen(string)+1.
@error               Invalid database or statement Handle."""
    pass
def SQL_FormatQuery(database: Any, buffer: str, maxlength: int, format: str, _0: Any, *args: Any) -> int:
    """Formats a string according to the SourceMod format rules (see documentation).
All format specifiers are escaped (see SQL_EscapeString) unless the '!' flag is used.

@param database      A database Handle.
@param buffer        Destination string buffer.
@param maxlength     Maximum length of output string buffer.
@param format        Formatting rules.
@param ...           Variable number of format parameters.
@return              Number of cells written."""
    pass
def SQL_QuoteString(database: Any, string: str, buffer: str, maxlength: int, written: int = ...) -> bool:
    pass
def SQL_FastQuery(database: Any, query: str, len: int = ...) -> bool:
    """Executes a query and ignores the result set.

@param database      A database Handle.
@param query         Query string.
@param len           Optional parameter to specify the query length, in 
                     bytes.  This can be used to send binary queries that 
                     have a premature terminator.
@return              True if query succeeded, false otherwise.  Use
                     SQL_GetError to find the last error.
@error               Invalid database Handle."""
    pass
def SQL_Query(database: Any, query: str, len: int = ...) -> Any:
    """Executes a simple query and returns a new query Handle for
receiving the results.

@param database      A database Handle.
@param query         Query string.
@param len           Optional parameter to specify the query length, in 
                     bytes.  This can be used to send binary queries that 
                     have a premature terminator.
@return              A new Query Handle on success, INVALID_HANDLE
                     otherwise.  The Handle must be freed with CloseHandle().
@error               Invalid database Handle."""
    pass
def SQL_PrepareQuery(database: Any, query: str, error: str, maxlength: int) -> Any:
    """Creates a new prepared statement query.  Prepared statements can
be executed any number of times.  They can also have placeholder
parameters, similar to variables, which can be bound safely and
securely (for example, you do not need to quote bound strings).

Statement handles will work in any function that accepts a Query handle.

@param database      A database Handle.
@param query         Query string.
@param error         Error buffer.
@param maxlength     Maximum size of the error buffer.
@return              A new statement Handle on success, INVALID_HANDLE
                     otherwise.  The Handle must be freed with CloseHandle().
@error               Invalid database Handle."""
    pass
def SQL_FetchMoreResults(query: Any) -> bool:
    """Advances to the next set of results.

In some SQL implementations, multiple result sets can exist on one query.  
This is possible in MySQL with simple queries when executing a CALL 
query.  If this is the case, all result sets must be processed before
another query is made.

@param query         A query Handle.
@return              True if there was another result set, false otherwise.
@error               Invalid query Handle."""
    pass
def SQL_HasResultSet(query: Any) -> bool:
    """Returns whether or not a result set exists.  This will
return true even if 0 results were returned, but false
on queries like UPDATE, INSERT, or DELETE.

@param query         A query (or statement) Handle.
@return              True if there is a result set, false otherwise.
@error               Invalid query Handle."""
    pass
def SQL_GetRowCount(query: Any) -> int:
    """Retrieves the number of rows in the last result set.

@param query         A query (or statement) Handle.
@return              Number of rows in the current result set.
@error               Invalid query Handle."""
    pass
def SQL_GetFieldCount(query: Any) -> int:
    """Retrieves the number of fields in the last result set.

@param query         A query (or statement) Handle.
@return              Number of fields in the current result set.
@error               Invalid query Handle."""
    pass
def SQL_FieldNumToName(query: Any, field: int, name: str, maxlength: int) -> None:
    """Retrieves the name of a field by index.

@param query         A query (or statement) Handle.
@param field         Field number (starting from 0).
@param name          Name buffer.
@param maxlength     Maximum length of the name buffer.
@error               Invalid query Handle, invalid field index, or
                     no current result set."""
    pass
def SQL_FieldNameToNum(query: Any, name: str, field: int) -> bool:
    """Retrieves a field index by name.

@param query         A query (or statement) Handle.
@param name          Name of the field (case sensitive).
@param field         Variable to store field index in.
@return              True if found, false if not found.
@error               Invalid query Handle or no current result set."""
    pass
def SQL_FetchRow(query: Any) -> bool:
    """Fetches a row from the current result set.  This must be 
successfully called before any results are fetched.

If this function fails, SQL_MoreRows() can be used to
tell if there was an error or the result set is finished.

@param query         A query (or statement) Handle.
@return              True if a row was fetched, false otherwise.
@error               Invalid query Handle."""
    pass
def SQL_MoreRows(query: Any) -> bool:
    """Returns if there are more rows.

@param query         A query (or statement) Handle.
@return              True if there are more rows, false otherwise.
@error               Invalid query Handle."""
    pass
def SQL_Rewind(query: Any) -> bool:
    """Rewinds a result set back to the first result.

@param query         A query (or statement) Handle.
@return              True on success, false otherwise.
@error               Invalid query Handle or no current result set."""
    pass
def SQL_FetchString(query: Any, field: int, buffer: str, maxlength: int, result: DBResult = ...) -> int:
    """Fetches a string from a field in the current row of a result set.  
If the result is NULL, an empty string will be returned.  A NULL 
check can be done with the result parameter, or SQL_IsFieldNull().

@param query         A query (or statement) Handle.
@param field         The field index (starting from 0).
@param buffer        String buffer.
@param maxlength     Maximum size of the string buffer.
@param result        Optional variable to store the status of the return value.
@return              Number of bytes written.
@error               Invalid query Handle or field index, invalid
                     type conversion requested from the database,
                     or no current result set."""
    pass
def SQL_FetchFloat(query: Any, field: int, result: DBResult = ...) -> float:
    """Fetches a float from a field in the current row of a result set.  
If the result is NULL, a value of 0.0 will be returned.  A NULL 
check can be done with the result parameter, or SQL_IsFieldNull().

@param query         A query (or statement) Handle.
@param field         The field index (starting from 0).
@param result        Optional variable to store the status of the return value.
@return              A float value.
@error               Invalid query Handle or field index, invalid
                     type conversion requested from the database,
                     or no current result set."""
    pass
def SQL_FetchInt(query: Any, field: int, result: DBResult = ...) -> int:
    """Fetches an integer from a field in the current row of a result set.  
If the result is NULL, a value of 0 will be returned.  A NULL 
check can be done with the result parameter, or SQL_IsFieldNull().

@param query         A query (or statement) Handle.
@param field         The field index (starting from 0).
@param result        Optional variable to store the status of the return value.
@return              An integer value.
@error               Invalid query Handle or field index, invalid
                     type conversion requested from the database,
                     or no current result set."""
    pass
def SQL_IsFieldNull(query: Any, field: int) -> bool:
    """Returns whether a field's data in the current row of a result set is 
NULL or not.  NULL is an SQL type which means "no data."

@param query         A query (or statement) Handle.
@param field         The field index (starting from 0).
@return              True if data is NULL, false otherwise.
@error               Invalid query Handle or field index, or no
                     current result set."""
    pass
def SQL_FetchSize(query: Any, field: int) -> int:
    """Returns the length of a field's data in the current row of a result
set.  This only needs to be called for strings to determine how many
bytes to use.  Note that the return value does not include the null
terminator.

@param query         A query (or statement) Handle.
@param field         The field index (starting from 0).
@return              Number of bytes for the field's data size.
@error               Invalid query Handle or field index or no
                     current result set."""
    pass
def SQL_BindParamInt(statement: Any, param: int, number: int, signed: bool = ...) -> None:
    """Binds a parameter in a prepared statement to a given integer value.

@param statement     A statement (prepared query) Handle.
@param param         The parameter index (starting from 0).
@param number        The number to bind.
@param signed        True to bind the number as signed, false to 
                     bind it as unsigned.
@error               Invalid statement Handle or parameter index, or
                     SQL error."""
    pass
def SQL_BindParamFloat(statement: Any, param: int, value: float) -> None:
    """Binds a parameter in a prepared statement to a given float value.

@param statement     A statement (prepared query) Handle.
@param param         The parameter index (starting from 0).
@param value         The float number to bind.
@error               Invalid statement Handle or parameter index, or
                     SQL error."""
    pass
def SQL_BindParamString(statement: Any, param: int, value: str, copy: bool) -> None:
    """Binds a parameter in a prepared statement to a given string value.

@param statement     A statement (prepared query) Handle.
@param param         The parameter index (starting from 0).
@param value         The string to bind.
@param copy          Whether or not SourceMod should copy the value
                     locally if necessary.  If the string contents
                     won't change before calling SQL_Execute(), this
                     can be set to false for optimization.
@error               Invalid statement Handle or parameter index, or
                     SQL error."""
    pass
def SQL_Execute(statement: Any) -> bool:
    """Executes a prepared statement.  All parameters must be bound beforehand.

@param statement     A statement (prepared query) Handle.
@return              True on success, false on failure.
@error               Invalid statement Handle."""
    pass
def SQL_LockDatabase(database: Any) -> None:
    """Locks a database so threading operations will not interrupt.

If you are using a database Handle for both threading and non-threading,
this MUST be called before doing any set of non-threading DB operations.
Otherwise you risk corrupting the database driver's memory or network
connection.

Leaving a lock on a database and then executing a threaded query results
in a dead lock! Make sure to call SQL_UnlockDatabase()!

If the lock cannot be acquired, the main thread will pause until the 
threaded operation has concluded.

Care should be taken to not lock an already-locked database. Internally,
lock calls are nested recursively and must be paired with an equal amount
of unlocks to be undone. This behaviour should not be relied on.

@param database      A database Handle.
@error               Invalid database Handle."""
    pass
def SQL_UnlockDatabase(database: Any) -> None:
    """Unlocks a database so threading operations may continue.

@param database      A database Handle.
@error               Invalid database Handle."""
    pass
def SQL_IsSameConnection(hndl1: Any, hndl2: Any) -> bool:
    """Tells whether two database handles both point to the same database 
connection.

@param hndl1         First database Handle.
@param hndl2         Second database Handle.
@return              True if the Handles point to the same 
                     connection, false otherwise.
@error               Invalid Handle."""
    pass
def SQL_TConnect(callback: Any, name: str = ..., data: Any = ...) -> None:
    """Connects to a database via a thread.  This can be used instead of
SQL_Connect() if you wish for non-blocking functionality.

It is not necessary to use this to use threaded queries.  However, if you 
don't (or you mix threaded/non-threaded queries), you should see 
SQL_LockDatabase().

@param callback      Callback; new Handle will be in hndl, owner is the driver.
                     If no driver was found, the owner is INVALID_HANDLE.
@param name          Database name.
@param data          Extra data value to pass to the callback."""
    pass
def SQL_TQuery(database: Any, callback: Any, query: str, data: Any = ..., prio: DBPriority = ...) -> None:
    """Executes a simple query via a thread.  The query Handle is passed through
the callback.

The database Handle returned through the callback is always a new Handle,
and if necessary, SQL_IsSameConnection() should be used to test against
other connections.

The query Handle returned through the callback is temporary and destroyed 
at the end of the callback.  If you need to hold onto it, use CloneHandle().

@param database      A database Handle.
@param callback      Callback; database is in "owner" and the query Handle
                     is passed in "hndl".
@param query         Query string.
@param data          Extra data value to pass to the callback.
@param prio          Priority queue to use.
@error               Invalid database Handle."""
    pass
def SQL_CreateTransaction() -> Any:
    """Creates a new transaction object. A transaction object is a list of queries
that can be sent to the database thread and executed as a single transaction.

@return              A transaction handle."""
    pass
def SQL_AddQuery(txn: Any, query: str, data: Any = ...) -> int:
    """Adds a query to a transaction object.

@param txn           A transaction handle.
@param query         Query string.
@param data          Extra data value to pass to the final callback.
@return              The index of the query in the transaction's query list.
@error               Invalid transaction handle."""
    pass
def SQL_ExecuteTransaction(db: Any, txn: Any, onSuccess: SQLTxnSuccess = ..., onError: Any = ..., data: Any = ..., priority: DBPriority = ...) -> None:
    """Sends a transaction to the database thread. The transaction handle is
automatically closed. When the transaction completes, the optional
callback is invoked.

@param db            A database handle.
@param txn           A transaction handle.
@param onSuccess     An optional callback to receive a successful transaction.
@param onError       An optional callback to receive an error message.
@param data          An optional value to pass to callbacks.
@param prio          Priority queue to use.
@error               An invalid handle."""
    pass
SQLTxnFailure: Any = ...
SQLConnectCallback: Any = ...
SQLQueryCallback: Any = ...
prio: DBPriority = ...
priority: DBPriority = ...
kv: Any = ...
db: Any = ...
maxTimeout: int = ...
SQLTCallback: Any = ...