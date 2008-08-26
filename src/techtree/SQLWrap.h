
#ifndef _SQLWRAP_H_
#define _SQLWRAP_H_

//
// Includes needed for SQL.
//
#include <sql.h>
#include <sqlext.h>

//
// The parameter list structure.  Arrays of this structure are passed to
// CSQLWrap::Execute and CSQLWrap::BindReturnParameters.
//
typedef struct _SQL_PARAMETER_LIST
{
    SQLSMALLINT     InputOutputType;
    SQLSMALLINT     ValueType;
    SQLSMALLINT     ParameterType;
    SQLUINTEGER     ColumnDigits;
    SQLPOINTER      ParameterValuePtr;
    SQLINTEGER      BufferLength;
    SQLINTEGER *    StrLen_or_IndPtr;
} SQL_PARAMETER_LIST, *PSQL_PARAMETER_LIST;

//
// A NULL SQL parameter (indicates end of parameter list).
//
#define SQL_PNULL                              \
    {           \
        0,      \
        0,      \
        0,      \
        0,      \
        NULL,   \
        0,      \
        NULL    \
    }

//
// Generic macros used by the parameters.
//
#define SQL_P_INT4(ParamInOut, Buffer, pSQLIntLen)          \
    {                           \
        ParamInOut,             \
        SQL_C_SLONG,            \
        SQL_INTEGER,            \
        0,                      \
        &(Buffer),              \
        4,                      \
        pSQLIntLen              \
    }

#define SQL_P_INT2(ParamInOut, Buffer, pSQLIntLen)          \
    {                           \
        ParamInOut,             \
        SQL_C_SSHORT,           \
        SQL_SMALLINT,           \
        0,                      \
        &(Buffer),              \
        2,                      \
        pSQLIntLen              \
    }

#define SQL_P_INT1(ParamInOut, Buffer, pSQLIntLen)          \
    {                           \
        ParamInOut,             \
        SQL_C_STINYINT,         \
        SQL_TINYINT,            \
        0,                      \
        &(Buffer),              \
        1,                      \
        pSQLIntLen              \
    }

#define SQL_P_FLOAT4(ParamInOut, Buffer, pSQLIntLen)        \
    {                           \
        ParamInOut,             \
        SQL_C_FLOAT,            \
        SQL_FLOAT,              \
        0,                      \
        &(Buffer),              \
        4,                      \
        pSQLIntLen              \
    }

#define SQL_P_CHAR(ParamInOut, Buffer, iLen, pSQLIntLen)    \
    {                           \
        ParamInOut,             \
        SQL_C_CHAR,             \
        SQL_CHAR,               \
        0,                      \
        Buffer,                 \
        (iLen),                 \
        pSQLIntLen              \
    }

#define SQL_P_WCHAR(ParamInOut, Buffer, iLen, pSQLIntLen)   \
    {                           \
        ParamInOut,             \
        SQL_C_WCHAR,            \
        SQL_WCHAR,              \
        0,                      \
        Buffer,                 \
        (iLen),                 \
        pSQLIntLen              \
    }

//
// Macros for converting from SQL types to C types on returned data.
//
#define SQL_PIN_INT4(Buf)           SQL_P_INT4(SQL_PARAM_INPUT,    Buf, NULL)
#define SQL_POUT_INT4(Buf, Len)     SQL_P_INT4(SQL_PARAM_OUTPUT,   Buf, &Len)
#define SQL_PIN_INT2(Buf)           SQL_P_INT2(SQL_PARAM_INPUT,    Buf, NULL)
#define SQL_POUT_INT2(Buf, Len)     SQL_P_INT2(SQL_PARAM_OUTPUT,   Buf, &Len)
#define SQL_PIN_INT1(Buf)           SQL_P_INT1(SQL_PARAM_INPUT,    Buf, NULL)
#define SQL_POUT_INT1(Buf, Len)     SQL_P_INT1(SQL_PARAM_OUTPUT,   Buf, &Len)
#define SQL_PIN_FLOAT4(Buf)         SQL_P_FLOAT4(SQL_PARAM_INPUT,  Buf, NULL)
#define SQL_POUT_FLOAT4(Buf, Len)   SQL_P_FLOAT4(SQL_PARAM_OUTPUT, Buf, &Len)
#define SQL_PIN_CHAR(Buf, iLen)         \
                                SQL_P_CHAR(SQL_PARAM_INPUT,   Buf, iLen, NULL)
#define SQL_POUT_CHAR(Buf, iLen, oLen)  \
                                SQL_P_CHAR(SQL_PARAM_OUTPUT,  Buf, iLen, &oLen)
#define SQL_PIN_WCHAR(Buf, iLen)        \
                                SQL_P_WCHAR(SQL_PARAM_INPUT,  Buf, iLen, NULL)
#define SQL_POUT_WCHAR(Buf, iLen, oLen) \
                                SQL_P_WCHAR(SQL_PARAM_OUTPUT, Buf, iLen, &oLen)


/*
#define SQL_POUT_INT4(Buffer, SQLIntLen)        \
    {                           \
        SQL_PARAM_OUTPUT,       \
        SQL_C_SLONG,            \
        0,                      \
        0,                      \
        &(Buffer),              \
        4,                      \
        &(SQLIntLen)            \
    }

#define SQL_POUT_INT2(Buffer, SQLIntLen)        \
    {                           \
        SQL_PARAM_OUTPUT,       \
        SQL_C_SSHORT,           \
        0,                      \
        0,                      \
        &(Buffer),              \
        2,                      \
        &(SQLIntLen)            \
    }

#define SQL_POUT_INT1(Buffer, SQLIntLen)        \
    {                           \
        SQL_PARAM_OUTPUT,       \
        SQL_C_STINYINT,         \
        0,                      \
        0,                      \
        &(Buffer),              \
        1,                      \
        &(SQLIntLen)            \
    }

#define SQL_POUT_FLOAT4(Buffer, SQLIntLen)      \
    {                           \
        SQL_PARAM_OUTPUT,       \
        SQL_C_FLOAT,            \
        0,                      \
        0,                      \
        &(Buffer),              \
        4,                      \
        &(SQLIntLen)            \
    }

#define SQL_POUT_CHAR(Buffer, iLen, SQLIntLen)  \
    {                           \
        SQL_PARAM_OUTPUT,       \
        SQL_C_CHAR,             \
        0,                      \
        0,                      \
        Buffer,                 \
        iLen,                   \
        &(SQLIntLen)            \
    }
*/


#define SQL_OK(SQLRet)          (SQL_SUCCEEDED(SQLRet))

#define SQLWRAP_ERROR_MSG_LENGTH        2048

class CSQLWrap  
{
    private:
        SQLHENV mhSQLEnv;
        SQLHDBC mhSQLConn;

        WCHAR  mwszErrorState[SQLWRAP_ERROR_MSG_LENGTH + 2];
        WCHAR  mwszErrorMessage[SQLWRAP_ERROR_MSG_LENGTH + 2];
        SDWORD msdwErrorCode;
        
        
        SQLRETURN InitializeEnvironment();
        SQLRETURN BindOutputParameter(SQLHSTMT hStatement,
                                    SQL_PARAMETER_LIST *pParameter,
                                    SQLUSMALLINT *pusmiColumns);
        SQLRETURN FinishExecute(SQLHSTMT hStatement,
                                    SQL_PARAMETER_LIST *pParameter,
                                    BOOL *pfReturnParameters);
        VOID TrapError(SQLHENV hEnv, SQLHDBC hDbc, SQLHSTMT hStmt);

    public:
        CSQLWrap();
        virtual ~CSQLWrap();

        SQLRETURN Initialize(PCHAR szDSN);
        SQLRETURN Initialize(PWCHAR szDSN);

        SQLRETURN Terminate();

        SQLRETURN Execute(PCHAR szStatement, SQL_PARAMETER_LIST *pParameter,
                            SQLHSTMT * phStatement);
        SQLRETURN Execute(PWCHAR szStatement, SQL_PARAMETER_LIST *pParameter,
                            SQLHSTMT * phStatement);
        SQLRETURN BindReturnParameters(SQLHSTMT hStatement,
                                        SQL_PARAMETER_LIST *pParameter);
        SQLRETURN FetchRow(SQLHSTMT hStatement);
        SQLRETURN EndStatement(SQLHSTMT hStatement);

        VOID GetError(WCHAR * szError, DWORD cbLength);
        VOID GetError(CHAR  * szError, DWORD cbLength);

        INT TerminateString(CHAR  * szString, INT iLength);
        INT TerminateString(WCHAR * szString, INT iLength);

};


//
// Method documentation follows:
//

//
// SQLRETURN CSQLWrap::Initialize(LPTSTR szDSN)
//
// Description:
//      This method initializes the CSQLWrap class.
//
// Parameters:
//      szDSN [IN] - Name of the system DSN to connect to (ASCII or Unicode).
//
// Returns:
//      SQL_SUCCESS or SQL_SUCCESS_WITH_INFO on success.
//      SQL specific error code on failure.
//

//
// SQLRETURN CSQLWrap::Terminate()
//
// Description:
//      This method cleans up all internal data being used by the
//      CSQLWrap class.
//
// Returns:
//      SQL_SUCCESS or SQL_SUCCESS_WITH_INFO on success.
//      SQL specific error code on failure.
//

//
// SQLRETURN CSQLWrap::Execute(LPTSTR szStatement,
//                              SQL_PARAMETER_LIST *pParameter,
//                              SQLHSTMT * phStatement)
//
// Description:
//      This method executes a SQL query or statement using the passed
//      parameters and returns a SQLHSTMT for the statement.
//
// Parameters:
//      szStatement [IN]  - String containing the SQL query of statement
//                          (ASCII or Unicode).
//      pParameter  [IN]  - Array of parameters to use in statement.
//      phStatement [OUT] - SQL HSTMT returned for statement.
//
// Returns:
//      SQL_SUCCESS or SQL_SUCCESS_WITH_INFO on success.
//      SQL specific error code on failure.
//
// Notes:
//      The SQL_PIN_* and SQL_POUT_* macros provided above should be
//      used when defining an array of parameters to pass to this
//      function.  The delcaration should look like:
//          INT i, j;
//          INT Len[2];
//
//          SQL_PARAMETER_LIST List[] =
//          {
//              SQL_POUT_INT4(i, Len[0]),
//              SQL_POUT_INT4(j, Len[1]),
//              SQL_PNULL
//          } ;
//

//
// SQLRETURN BindReturnParameters(SQLHSTMT hStatement,
//                                  SQL_PARAMETER_LIST *pParameter)
//
// Description:
//      This method binds the return parameters for the given SQL statement.
//
// Parameters:
//      hStatement [IN] - HSTMT of an existing SQL statement.
//      pParameter [IN] - Array of parameters to use in statement.
//
// Returns:
//      SQL_SUCCESS or SQL_SUCCESS_WITH_INFO on success.
//      SQL specific error code on failure.
// 
// Notes:
//      This function need not be called if the SQL out parameters have
//      been specified in the call to CSQLWrap::Execute.  This method
//      simply provides a way to change the parameter list between
//      row entries (for filling an array for example).
//

//
// SQLRETURN FetchRow(SQLHSTMT hStatement)
//
// Description:
//      This method fetches the next row for the given statement,
//      automatically putting any bound parameters into their destintation
//      buffers.
//
// Parameters:
//      hStatement [IN] - HSTMT of an existing SQL statement.
//
// Returns:
//      SQL_SUCCESS or SQL_SUCCESS_WITH_INFO on success.
//      SQL_NO_DATA if no rows remain.
//      SQL specific error code on failure.
// 

//
// SQLRETURN EndStatement(SQLHSTMT hStatement)
//
// Description:
//      This method frees the SQL HSTMT and ends the statement.
//
// Parameters:
//      hStatement [MODIFY] - HSTMT of an existing SQL statement.
//
// Returns:
//      SQL_SUCCESS or SQL_SUCCESS_WITH_INFO on success.
//      SQL specific error code on failure.
//

//
// VOID GetError(LPTSTR szError, DWORD cbLength)
//
// Description:
//      This function puts the last SQL error message into the given
//      buffer.  If the buffer is not long enough, the message is
//      truncated to fit.
//
// Parameters:
//      szError [MODIFY] - Buffer to place the error message
//                          (ASCII or Unicode).
//      cbLength    [IN] - Size of the buffer in bytes.
// 

//
// INT TerminateString(LPTSTR szString, INT iLength)
//
// Description:
//      This helper function NULL terminates the given string based on the
//      length during the CSQLWrap::FetchRow() method.
//
// Parameters:
//      szString [MODIFY] - String to terminate (ASCII or Unicode).
//      iLength      [IN] - Length of the string returned by SQL.
//
// Returns:
//      The length of the string in characters including the trailing NULL.
//

#endif

