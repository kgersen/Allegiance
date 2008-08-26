

//#include <windows.h>
#include "pch.h"
#include "SQLWrap.h"


CSQLWrap::CSQLWrap()
{
    mhSQLEnv = NULL;
    mhSQLConn = SQL_NULL_HDBC;
}


CSQLWrap::~CSQLWrap()
{
    this->Terminate();
}


SQLRETURN CSQLWrap::InitializeEnvironment()
{
    SQLRETURN SQLResult;

    if (NULL == mhSQLEnv)
    {
        SQLResult = SQLAllocHandle(SQL_HANDLE_ENV,
                                    SQL_NULL_HANDLE,
                                    &mhSQLEnv);

        if (SQL_SUCCESS == SQLResult)
        {
            //
            // Exhibit ODBC 2.x behavior.  Not sure why,
            // but this is what dev is doing.
            //
            SQLResult = SQLSetEnvAttr(mhSQLEnv,
                                        SQL_ATTR_ODBC_VERSION,
                                        (SQLPOINTER) SQL_OV_ODBC2,
                                        0);
            if (SQL_SUCCESS != SQLResult)
            {
                SQLFreeHandle(SQL_HANDLE_ENV, mhSQLEnv);
                mhSQLEnv = NULL;
            }
        } else
            mhSQLEnv = NULL;

    }
    return(SQLResult);
}


SQLRETURN CSQLWrap::Initialize(PCHAR szDSN)
{
    SQLRETURN SQLResult;

    SQLResult = this->InitializeEnvironment();

    //
    // Open a connection to the ODBC DSN.
    //
    if ((SQL_NULL_HDBC == mhSQLConn) && SQL_OK(SQLResult))
    {
        SQLResult = SQLAllocHandle(SQL_HANDLE_DBC,
                                    mhSQLEnv,
                                    &mhSQLConn);

        if (SQL_OK(SQLResult))
        {
            SQLResult = SQLConnectA(mhSQLConn,
                                    (SQLCHAR *) szDSN,
                                    SQL_NTS,
                                    (SQLCHAR *) "",
                                    SQL_NTS,
                                    (SQLCHAR *) "",
                                    SQL_NTS);
            if (!SQL_OK(SQLResult))
            {
                this->TrapError(mhSQLEnv, mhSQLConn, SQL_NULL_HSTMT);
                SQLFreeHandle(SQL_HANDLE_DBC, mhSQLConn);
                mhSQLConn = SQL_NULL_HDBC;
            }
        } else
            mhSQLConn = SQL_NULL_HDBC;
    }

    return(SQLResult);
}


SQLRETURN CSQLWrap::Initialize(PWCHAR szDSN)
{
    SQLRETURN SQLResult;

    SQLResult = this->InitializeEnvironment();

    //
    // Open a connection to the ODBC DSN.
    //
    if ((SQL_NULL_HDBC == mhSQLConn) && (SQL_SUCCESS == SQLResult))
    {
        SQLResult = SQLAllocHandle(SQL_HANDLE_DBC,
                                    mhSQLEnv,
                                    &mhSQLConn);

        if (SQL_OK(SQLResult))
        {
            SQLResult = SQLConnectW(mhSQLConn,
                                    (SQLWCHAR *) szDSN,
                                    SQL_NTS,
                                    (SQLWCHAR *) L"",
                                    SQL_NTS,
                                    (SQLWCHAR *) L"",
                                    SQL_NTS);
            if (!SQL_OK(SQLResult))
            {
                this->TrapError(mhSQLEnv, mhSQLConn, SQL_NULL_HSTMT);
                SQLFreeHandle(SQL_HANDLE_DBC, mhSQLConn);
                mhSQLConn = SQL_NULL_HDBC;
            }
        } else
            mhSQLConn = SQL_NULL_HDBC;
    }

    return(SQLResult);
}


SQLRETURN CSQLWrap::Terminate()
{
    SQLRETURN SQLResult;

    SQLResult = SQL_SUCCESS;
    if (SQL_NULL_HDBC != mhSQLConn)
    {
        SQLResult = SQLDisconnect(mhSQLConn);
        if (SQL_OK(SQLResult))
        {
            SQLResult = SQLFreeHandle(SQL_HANDLE_DBC,
                                        mhSQLConn);
            if (SQL_OK(SQLResult))
                mhSQLConn = SQL_NULL_HDBC;
            else
                this->TrapError(mhSQLEnv, mhSQLConn, SQL_NULL_HSTMT);
        } else
            this->TrapError(mhSQLEnv, mhSQLConn, SQL_NULL_HSTMT);
    }

    if ((NULL != mhSQLEnv) && SQL_OK(SQLResult))
    {
        SQLResult = SQLFreeHandle(SQL_HANDLE_ENV,
                                    mhSQLEnv);
        if (SQL_OK(SQLResult))
            mhSQLEnv = NULL;
        else
            this->TrapError(mhSQLEnv, SQL_NULL_HDBC, SQL_NULL_HSTMT);
    }

    return(SQLResult);
}


SQLRETURN CSQLWrap::BindOutputParameter(SQLHSTMT hStatement,
                                  SQL_PARAMETER_LIST *pParameter,
                                  SQLUSMALLINT *pusmiColumns)
{
    SQLRETURN SQLResult;

    SQLResult = SQLBindCol(
                    hStatement,
                    ++(*pusmiColumns),
                    pParameter->ValueType,
                    pParameter->ParameterValuePtr,
                    pParameter->BufferLength,
                    pParameter->StrLen_or_IndPtr);

    return(SQLResult);
}


SQLRETURN CSQLWrap::FinishExecute(SQLHSTMT hStatement,
                                    SQL_PARAMETER_LIST *pParameter,
                                    BOOL *pfReturnParameters)
{
    SQLUSMALLINT usmiParameters, usmiColumns;
    SQLRETURN SQLResult;
    DWORD dwIndex;

    *pfReturnParameters = FALSE;

    if (NULL != pParameter)
    {
        usmiParameters = usmiColumns = 0;
        for(dwIndex = 0;
            NULL != pParameter[dwIndex].ParameterValuePtr;
            dwIndex++)
        {
            switch(pParameter[dwIndex].InputOutputType)
            {
                case SQL_PARAM_INPUT:
                    SQLResult = SQLBindParameter(
                                    hStatement,
                                    ++usmiParameters,
                                    SQL_PARAM_INPUT,
                                    pParameter[dwIndex].ValueType,
                                    pParameter[dwIndex].ParameterType,
                                    pParameter[dwIndex].ColumnDigits,
                      (SQLSMALLINT) pParameter[dwIndex].ColumnDigits,
                                    pParameter[dwIndex].ParameterValuePtr,
                                    pParameter[dwIndex].BufferLength,
                                    pParameter[dwIndex].StrLen_or_IndPtr);
                    break;
                case SQL_PARAM_OUTPUT:
                    SQLResult = this->BindOutputParameter(
                                    hStatement,
                                    &pParameter[dwIndex],
                                    &usmiColumns);
                    *pfReturnParameters = TRUE;
                    break;
                default:
                    //
                    // BUGBUG: Set an error here.
                    //
                    break;
            }

            if (!SQL_OK(SQLResult))
                goto END_LABEL;
        }
    }

    //
    // Execute the statement.
    //

    SQLResult = SQLExecute(hStatement);

END_LABEL:

    if (!SQL_OK(SQLResult))
        this->TrapError(mhSQLEnv, mhSQLConn, hStatement);

    return(SQLResult);
}


SQLRETURN CSQLWrap::Execute(PCHAR szStatement, SQL_PARAMETER_LIST *pParameter,
                            SQLHSTMT * phStatement)
{
    SQLRETURN SQLResult;
    SQLHSTMT hStatement;
    BOOL fReturnParameters;

    if (NULL != phStatement)
        *phStatement = SQL_NULL_HSTMT;
    hStatement = SQL_NULL_HSTMT;

    //
    // Need to create a statement.
    //
    SQLResult = SQLAllocHandle(SQL_HANDLE_STMT, mhSQLConn, &hStatement);
    if (!SQL_OK(SQLResult))
        goto ERROR_LABEL;

    //
    // Need to prepare the statement.
    //
    SQLResult = SQLPrepareA(hStatement,
                            (SQLCHAR *) szStatement,
                            SQL_NTS);
    if (!SQL_OK(SQLResult))
        goto ERROR_LABEL;

    //
    // Time to add parameters to the statement.
    //
    SQLResult = this->FinishExecute(hStatement,
                                    pParameter,
                                    &fReturnParameters);
    if (!SQL_OK(SQLResult))
        goto ERROR_LABEL;

    goto END_LABEL;

ERROR_LABEL:
    this->TrapError(mhSQLEnv, mhSQLConn, hStatement);

    if (SQL_NULL_HSTMT != hStatement)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStatement);
        hStatement = SQL_NULL_HSTMT;
    }

END_LABEL:
    if (NULL != phStatement)
        *phStatement = hStatement;
    else if (SQL_NULL_HSTMT != hStatement)
        SQLFreeHandle(SQL_HANDLE_STMT, hStatement);

    return(SQLResult);
}


SQLRETURN CSQLWrap::Execute(PWCHAR szStatement, SQL_PARAMETER_LIST *pParameter,
                            SQLHSTMT * phStatement)
{
    SQLRETURN SQLResult;
    SQLHSTMT hStatement;
    BOOL fReturnParameters;

    if (NULL != phStatement)
        *phStatement = SQL_NULL_HSTMT;
    hStatement = SQL_NULL_HSTMT;

    //
    // Need to create a statement.
    //
    SQLResult = SQLAllocHandle(SQL_HANDLE_STMT, mhSQLConn, &hStatement);
    if (!SQL_OK(SQLResult))
        goto ERROR_LABEL;

    //
    // Need to prepare the statement.
    //
    SQLResult = SQLPrepareW(hStatement,
                            (SQLWCHAR *) szStatement,
                            SQL_NTS);
    if (!SQL_OK(SQLResult))
        goto ERROR_LABEL;

    //
    // Time to add parameters to the statement.
    //
    SQLResult = this->FinishExecute(hStatement,
                                    pParameter,
                                    &fReturnParameters);
    if (!SQL_OK(SQLResult))
        goto ERROR_LABEL;

    goto END_LABEL;

ERROR_LABEL:
    this->TrapError(mhSQLEnv, mhSQLConn, hStatement);

    if (SQL_NULL_HSTMT != hStatement)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStatement);
        hStatement = SQL_NULL_HSTMT;
    }

END_LABEL:
    if (NULL != phStatement)
        *phStatement = hStatement;
    else if (SQL_NULL_HSTMT != hStatement)
        SQLFreeHandle(SQL_HANDLE_STMT, hStatement);

    return(SQLResult);

}


SQLRETURN CSQLWrap::BindReturnParameters(SQLHSTMT hStatement,
                                        SQL_PARAMETER_LIST *pParameter)
{
    SQLRETURN SQLResult;
    DWORD dwIndex;
    SQLUSMALLINT usmiColumns;
    
    SQLResult = SQL_SUCCESS;
    usmiColumns = 0;

    for(dwIndex = 0;
        NULL != pParameter[dwIndex].ParameterValuePtr;
        dwIndex++)
    {
        if (pParameter[dwIndex].InputOutputType != SQL_PARAM_OUTPUT)
        {
            SQLResult = SQL_ERROR;
            goto END_LABEL;
        }

        SQLResult = this->BindOutputParameter(hStatement,
                                            &pParameter[dwIndex],
                                            &usmiColumns);
        if (!SQL_OK(SQLResult))
            goto END_LABEL;
    }

END_LABEL:
    if (!SQL_OK(SQLResult))
        this->TrapError(mhSQLEnv, mhSQLConn, hStatement);

    return(SQLResult);
}


SQLRETURN CSQLWrap::FetchRow(SQLHSTMT hStatement)
{
    SQLRETURN SQLResult;

    SQLResult = SQLFetch(hStatement);
    if (!SQL_OK(SQLResult))
        this->TrapError(mhSQLEnv, mhSQLConn, hStatement);

    return(SQLResult);
}


SQLRETURN CSQLWrap::EndStatement(SQLHSTMT hStatement)
{
    SQLRETURN SQLResult;

    SQLResult = SQLFreeHandle(SQL_HANDLE_STMT, hStatement);
    return(SQLResult);
}


VOID CSQLWrap::TrapError(SQLHENV hEnv, SQLHDBC hDbc, SQLHSTMT hStmt)
{
    SWORD swLen;

    SQLErrorW(hEnv,
                hDbc,
                hStmt,
                mwszErrorState,
                &msdwErrorCode,
                mwszErrorMessage,
                SQLWRAP_ERROR_MSG_LENGTH,
                &swLen);
}


VOID CSQLWrap::GetError(WCHAR * szError, DWORD cbLength)
{
    DWORD cchLength;

    cchLength = (cbLength / sizeof(WCHAR)) - 1;

    wcsncpy(szError, mwszErrorMessage, cchLength);
    szError[cchLength] = L'\0';
}


VOID CSQLWrap::GetError(CHAR * szError, DWORD cbLength)
{
    INT i;

    i = WideCharToMultiByte(CP_ACP,
                            0,
                            mwszErrorMessage,
                            -1,
                            szError,
                            cbLength,
                            NULL,
                            NULL);

    szError[cbLength - 1] = '\0';
}


INT CSQLWrap::TerminateString(CHAR * szString, INT iLength)
{
    if (iLength <= 0)
        iLength = 0;
    szString[iLength] = '\0';
    return(iLength + 1);
}


INT CSQLWrap::TerminateString(WCHAR * szString, INT iLength)
{
    if (iLength <= 0)
        iLength = 0;
    szString[iLength] = L'\0';
    return(iLength + 1);
}


