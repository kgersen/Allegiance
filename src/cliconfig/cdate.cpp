//-----------------------------------------------------------------------------
// @doc
//
// @module cdate.cpp : implementation file for <c CDate> |
//
// This file implements the functionality for a date processing
// class, as well as parsing functionality and an example of one
// way to implement a state machine in C++.  
//
// Copyright <cp> 1995 Microsoft Corporation, All Rights Reserved
//
// Mark Snyder (MarkSn), Software Design Engineer/Test
//

#include "stdafx.h"
#include "cdate.h"
#include "ctype.h"

// Parsing functions
int Parse99NTor9NT(const char* lpChar, rcParse& rcp);
int Parse99or9(const char* lpChar, rcParse& rcp);
int Parse9999or99(const char* lpChar, rcParse& rcp);
int ParseMonthName(const char* lpChar, rcParse& rcp);
int Parse99Slashor9Slash(const char* lpChar, rcParse& rcp);
int Parse99Colonor9Colon(const char* lpChar, rcParse& rcp);
int Parse99Commaor9Comma(const char* lpChar, rcParse& rcp);
int Parse9999Commaor99Comma(const char* lpChar, rcParse& rcp);
int ParseDash(const char* lpChar, rcParse& rcp);
int ParseEOM(const char* lpChar, rcParse& rcp);

int Parse99NTor9NT(const char* lpChar, rcParse& rcp)
{
  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) && *(lpChar+2)=='N' && *(lpChar+3)=='T')
  {
    char szToken[3];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[2];
    szToken[2]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 4;
    return 1;
  }

  if (isdigit(*lpChar) && *(lpChar+1)=='N' && *(lpChar+2)=='T')
  {
    char szToken[2];
    szToken[0]=lpChar[0];
    szToken[1]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 3;
    return 1;
  }

  return 0;
}

int Parse99or9(const char* lpChar, rcParse& rcp)
{
  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) )
  {
    char szToken[3];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 2;
    return 1;
  }

  if (isdigit(*lpChar))
  {
    char szToken[2];
    szToken[0]=lpChar[0];
    szToken[1]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 1;
    return 1;
  }

  return 0;
}

int Parse9999or99(const char* lpChar, rcParse& rcp)
{
  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) && isdigit(*(lpChar+2)) && isdigit(*(lpChar+3)))
  {
    char szToken[5];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]=lpChar[2];
    szToken[3]=lpChar[3];
    szToken[4]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 4;
    return 1;
  }

  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) )
  {
    char szToken[3];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]='\0';
    rcp.nVal = atoi(szToken)+1900;
    rcp.nBump = 2;
    return 1;
  }

  return 0;
}

int ParseMonthName(const char* lpChar, rcParse& rcp)
{
  static char Months[12][12] =
  {
    "january","february","march","april","may","june","july","august","september","october","november","december"
  };
  char target[12];

  // move the string that is supposed to be a month abbreviation to target[]
  for (int i=0;i<12;i++)
  {
    if( isalpha(*(lpChar+i)) )
      target[i] = tolower(*(lpChar+i));
    else
    {
      target[i]='\0';
      break;
    }
  }

  // if they didn't enter at least 3 characters, it's an error!
  if (lstrlen(target)<3)
    return 0;

  // see if target[] is an abbreviation for one of the month names
  for (i=0;i<12;i++)
  {
    if ( strstr(Months[i],target) )
    {
      rcp.nBump = lstrlen(target);
      rcp.nVal = i+1;
      return 1;
    }
  }

  // the string was not an abbreviation for a month, so...
  return 0;
}

int Parse99Slashor9Slash(const char* lpChar, rcParse& rcp)
{
  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) && *(lpChar+2)=='/' )
  {
    char szToken[3];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 3;
    return 1;
  }

  if (isdigit(*lpChar) && *(lpChar+1)=='/' )
  {
    char szToken[2];
    szToken[0] = lpChar[0];
    szToken[1]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 2;
    return 1;
  }

  return 0;
}

int Parse99Colonor9Colon(const char* lpChar, rcParse& rcp)
{
  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) && *(lpChar+2)==':' )
  {
    char szToken[3];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 3;
    return 1;
  }

  if (isdigit(*lpChar) && *(lpChar+1)==':' )
  {
    char szToken[2];
    szToken[0]=lpChar[0];
    szToken[1]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 2;
    return 1;
  }

  return 0;
}

int Parse99Commaor9Comma(const char* lpChar, rcParse& rcp)
{
  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) && *(lpChar+2)==',' )
  {
    char szToken[3];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 3;
    return 1;
  }

  if (isdigit(*lpChar) && *(lpChar+1)==',' )
  {
    char szToken[2];
    szToken[0]=lpChar[0];
    szToken[1]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 2;
    return 1;
  }

  return 0;
}

int Parse9999Commaor99Comma(const char* lpChar, rcParse& rcp)
{
  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) && isdigit(*(lpChar+2)) && isdigit(*(lpChar+3)) && *(lpChar+4)==',' )
  {
    char szToken[5];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]=lpChar[2];
    szToken[3]=lpChar[3];
    szToken[4]='\0';
    rcp.nVal = atoi(szToken);
    rcp.nBump = 5;
    return 1;
  }

  if (isdigit(*lpChar) && isdigit(*(lpChar+1)) && *(lpChar+2)==',' )
  {
    char szToken[3];
    szToken[0]=lpChar[0];
    szToken[1]=lpChar[1];
    szToken[2]='\0';
    rcp.nVal = atoi(szToken)+1900;
    rcp.nBump = 3;
    return 1;
  }

  return 0;
}

int ParseDash(const char* lpChar, rcParse& rcp)
{
  if (*lpChar=='-' )
  {
    rcp.nVal = 0;
    rcp.nBump = 1;
    return 1;
  }

  return 0;
}

int ParseEOM(const char* lpChar, rcParse& rcp)
{
  if (*lpChar=='\0' )
  {
    rcp.nVal = 0;
    rcp.nBump = 1;
    return 1;
  }

  return 0;
}

enum { State_Init=0, State_1, State_2, State_3, State_4, State_5,
       State_6, State_7, State_8, State_9, State_10, State_11, State_12,
       State_End, State_Error};

// These templates are needed to allow us to add/sub any class to a CDate for which
// CDate supplies the + and - that these use for valid math types, namely the
// classes CMin, CSec, CHour, CMonth, CDay, and CYear.
#define definePlusEquals(type)  \
  CDate operator+=(CDate& d,const type& t) { d = d + type(t); return d; }
#define defineMinusEquals(type)  \
  CDate operator-=(CDate& d,const type& t) { d = d - type(t); return d; }
definePlusEquals(CYear)
defineMinusEquals(CYear)
definePlusEquals(CMon)
defineMinusEquals(CMon)
definePlusEquals(CDay)
defineMinusEquals(CDay)
definePlusEquals(CHour)
defineMinusEquals(CHour)
definePlusEquals(CMin)
defineMinusEquals(CMin)
definePlusEquals(CSec)
defineMinusEquals(CSec)

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the object to the current date/time.
//
//  @rdesc    None.
//
void CDate::SetDateCurrent()
{
    time_t timer = time(NULL);
    *this = timer;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Tests two date objects for equality for use in making all the
//            cobject sortable.
//
//  @rdesc    TRUE if the specified date is equal.
//
BOOL CDate::isEqualTo(const CDate& testDate) const
{
  // compare the first 6 int's to see if the dates/times are equal
  return ( sec  == testDate.GetSec()  &&
           minute  == testDate.GetMin()  &&
           hour == testDate.GetHour() &&
           mday == testDate.GetDay()  &&
           mon  == testDate.GetMonth()  &&
           year == testDate.GetYear() );
}

//------------------------------------------------------------------------------
//
//  @mfunc    Tests two object for less than to allow the object to be
//            sortable.
//
//  @rdesc    TRUE if the specified date is less than this.
//
BOOL CDate::isLessThan(const CDate& testDate) const
{
  // see if our date/time is less than the passed date/time
  if ( year > testDate.GetYear() )
    return 0;
  else if ( year < testDate.GetYear() )
    return 1;
  if ( mon > testDate.GetMonth() )
    return 0;
  else if ( mon < testDate.GetMonth() )
    return 1;
  if ( mday > testDate.GetDay() )
    return 0;
  else if ( mday < testDate.GetDay() )
    return 1;
  if ( hour > testDate.GetHour() )
    return 0;
  else if ( hour < testDate.GetHour() )
    return 1;
  if ( minute > testDate.GetMin() )
    return 0;
  else if ( sec < testDate.GetSec() )
    return 1;
  return 0;
}
 
//------------------------------------------------------------------------------
//
//  @mfunc    This function calculates the number of units between x and y
//            which are divisible by z.  Whichever is smaller is inclusive,
//            larger one non-inclusive.  This means that if the smaller value
//            is a leap year, it counts, but if the larger one is a leap year,
//            it doesn't count.
//
//  @rdesc    The number of units calculated.
//
int CDate::DivisibleUnits( int x, int y, int z ) const
{
  // get the number of units between x and y div. by z
  int nDiv = abs(x-y)/z;

  // if the lower operand of the two is divisible by z, add another
  int lower = x<y ? x : y;
  nDiv += (lower%z) ? 0 : 1;

  return nDiv;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Calculates the number of leap years between the two years spec'd.
//
//  @rdesc    An integer number of years.
//
int CDate::LeapYearsBetween( int x, int y ) const
{
  return DivisibleUnits(x,y,4) - DivisibleUnits(x,y,100) + DivisibleUnits(x,y,400);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Calculates the number of days in the current month.
//
//  @rdesc    An integer number of months.
//
int CDate::GetDaysInMonth() const
{
  return GetDaysInMonth( mon );
}

int CDate::GetDaysInMonth( int i ) const
{
  int md[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  return (i==2 && isLeapYear()) ? 29 : md[i-1];
}

//------------------------------------------------------------------------------
//
//  @mfunc    Calculates the day of year of the currently held date.
//
//  @rdesc    A one-based integer day number within the year.
//
int CDate::GetDayOfYear() const
{
  int md[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
  int i = md[mon-1];
  i += mday;
  i += (mon>2 && isLeapYear()) ? 1 : 0;

  return i;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Calculates the number of days in the current month.
//
//  @rdesc    An integer number of weeks.
//
int CDate::GetWeeksInMonth() const
{
  // Create a CDate for the first day of the selected month
  CDate date(GetYear(), GetMonth(), 1, 0, 0, 0);

  // Compute the number of weeks the selected month spans
  int nDaySpan = date.GetDayOfWeek() + date.GetDaysInMonth();
  return (nDaySpan + 6) / 7;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Determines if the year is a leap year.  The rule specifies that
//            any given year is a leap year if it is divisible by four, unless
//            it is divisible by 100, unless it is divisible by 400.
//
//  @rdesc    TRUE if the current year is a leap year, otherwise FALSE.
//
int CDate::isLeapYear() const
{
  // if it's a leap year, then this will return 1.  Otherwise, 0.
  return LeapYearsBetween( year, year );
}

//------------------------------------------------------------------------------
//
//  @mfunc    Calculates the day of week for the current date.
//
//  @rdesc    0 for Sunday, 1 for Monday, thru 6 for Saturday.
//
int CDate::GetDayOfWeek() const
{
  // Start at Sunday Jan 1, 1989

  // Compute day of week for January 1st of the target year.
  int wday  = ( year > 1989 ) ? ( year-1989 ) : (1989-year);

  // Adjust for the leap years between 1989 and the target year.  This adds a day for each leap year
  //   after 1989, and subtracts a day for every leap year prior to 1989.
  wday += ( year > 1989 ) ? LeapYearsBetween( 1989, year ) : -LeapYearsBetween( 1989, year );

  // Now adjust for the current day of year of the target date.
  wday  = ( wday + GetDayOfYear() - 1 ) % 7;

  return wday;
}

//------------------------------------------------------------------------------
//
//  @mfunc    This function calculates the number of days between two dates.
//
//  @rdesc    The number of days between this date and the specified date.
//
int CDate::DayDiff( const CDate& ADate ) const
{
  CDate d1 = min(*this,ADate);
  CDate d2 = max(*this,ADate);

  // set both working dates to 1/1 of the year
  int dx = d2.GetDayOfYear() - d1.GetDayOfYear();
  d1.SetDate(1,1,d1.GetYear());
  d2.SetDate(1,1,d2.GetYear());

  int j = (d2.GetYear()-d1.GetYear())*365;
  dx += j ? j + LeapYearsBetween(d1.GetYear(),d2.GetYear()) : j;

  return dx;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Formats a character representation of this object in the format
//            specified by the tokens in the format string.  See the strftime
//            function for more details on the format of szFmt.
//
//  @rdesc    The number of characters copied to the buffer.
//
int CDate::GetDate( LPSTR szBuf, int nBufSize, LPCSTR szFmt )
{
  struct tm strDateTime;

  strDateTime.tm_sec   = GetSec();        /* Seconds */
  strDateTime.tm_min   = GetMin();        /* Minutes */
  strDateTime.tm_hour  = GetHour();       /* Hour (0--23) */
  strDateTime.tm_mday  = GetDay();        /* Day of month (1--31) */
  strDateTime.tm_mon   = GetMonth()-1;    /* Month (0--11) */
  strDateTime.tm_year  = GetYear()-1900;  /* Year (calendar year minus 1900) */

  return int(strftime( szBuf, nBufSize, szFmt, &strDateTime));
}

//------------------------------------------------------------------------
//
//  @mfunc    Creates a text string representation of the time value.
//
//  @rdesc    None.
//
void CDate::GetWinTime(CString& str)
{
  // Read the Windows international time format settings
  LPCSTR szSection = "INTL";
  BOOL   iTime;
  char   sTime[2];
  char   s1159[10];
  char   s2359[10];
  BOOL   iTLZero;
  iTime = GetProfileInt(szSection, "iTime", 0);
  GetProfileString(szSection, "sTime", ":", sTime, sizeof(sTime));
  GetProfileString(szSection, "s1159", "AM", s1159, sizeof(s1159));
  GetProfileString(szSection, "s2359", "PM", s2359, sizeof(s2359));
  iTLZero = GetProfileInt(szSection, "iTLZero", 0);

  // Create the text strings based on WIN.INI International settings
  char szString[sizeof(s1159)];
  UINT uHour = GetHour();
  if (!iTime)
  {
    if (GetHour() == 0)
      uHour = 12;
    else if (GetHour() > 12)
      uHour -= 12;
    lstrcpy(szString, (GetHour() < 12) ? s1159 : s2359);
  }
  else
    lstrcpy(szString, s2359);

  char szTime[20];
  wsprintf(szTime, iTLZero ? "%02u%c%02u%c%02u%s" : "%u%c%02u%c%02u%s",
           uHour, sTime[0], GetMin(), sTime[0], GetSec(), szString);

  str = szTime;
}

//------------------------------------------------------------------------
//
//  @mfunc    Creates a text string representation of the date value.
//
//  @rdesc    None.
//
void CDate::GetWinShortDate(CString& str)
{
  // Get the short date format string from the WIN.INI file [Intl] section
  char sShortDate[20];
  GetProfileString("INTL", "sShortDate", "MM/DD/YY", sShortDate, sizeof(sShortDate));

  // Format a date string based on the Windows short date format
  LPSTR szDate = new char[lstrlen(sShortDate) * 2 + 1];
  *szDate = '\0';
  LPCSTR lpFormat = sShortDate;
  char   szTemp[10];
  while (*lpFormat)
  {
    LPCSTR lpToken = lpFormat;
    switch (GetNextDateToken(lpFormat))
    {
      case DateToken_m:
        wsprintf(szTemp, "%u", GetMonth());
        break;
      case DateToken_mm:
        wsprintf(szTemp, "%02u", GetMonth());
        break;
      case DateToken_d:
        wsprintf(szTemp, "%u", GetDay());
        break;
      case DateToken_dd:
        wsprintf(szTemp, "%02u", GetDay());
        break;
      case DateToken_yy:
        wsprintf(szTemp, "%02u", GetYear()-1900);
        break;
      case DateToken_yyyy:
        wsprintf(szTemp, "%04u", GetYear());
        break;
      case DateTokenUnrecognized:
      {
        LPSTR lpTemp = szTemp;
        while (lpToken != lpFormat)
          *lpTemp++ = *lpToken++;
        *lpTemp = '\0';
        break;
      }
    }
    lstrcat(szDate, szTemp);
  }

  str = szDate;
}

//------------------------------------------------------------------------
//
//  @mfunc    Creates a text string representation of the date value that
//            matches the format that is parsed for in SetDate.
//
//  @rdesc    None.
//
void CDate::GetParseDate(CString& str) const
{
  char szDate[64];

  wsprintf( szDate, "%02d/%02d/%02d, %02d:%02d", GetMonth(), GetDay(), GetYear(), GetHour(), GetMin());

  str = szDate;
}

//------------------------------------------------------------------------
//
//  @mfunc    Creates a text string representation of the time value only
//            which matches the format that is parsed for in SetDate.
//
//  @rdesc    None.
//
void CDate::GetParseTime(CString& str)
{
  char szDate[64];

  wsprintf( szDate, "%02d:%02d", GetHour(), GetMin());

  str = szDate;
}

//------------------------------------------------------------------------
//
//  @mfunc    Creates a text string representation of the time value only
//            which matches the format that is parsed for in SetDate.
//
//  @rdesc    None.
//
void CDate::GetFiletime(FILETIME* ft) const
{
    SYSTEMTIME systime;
    CTime t(GetYear(), GetMonth(), GetDay(), GetHour(), GetMin(), GetSec());
    t.GetAsSystemTime(systime);
    SystemTimeToFileTime(&systime, ft);
}

//------------------------------------------------------------------------
//
//  @mfunc    Determines the date token that the specified pointer is
//            pointing to and increments the pointer past the token.
//
//  @rdesc    The type of token pointed to by the specified pointer.
//
CDate::DateToken CDate::GetNextDateToken(LPCSTR & lpFormat)
{
  // Determine which date token, if any, is being pointed to
  int nCount;
  switch (*lpFormat)
  {
    case '\0':
      return DateTokenUnrecognized;

    case 'm':
    case 'M':
      // Count the number of times this character occurs
      nCount = 1;
      while (*(++lpFormat) == 'm' || *lpFormat == 'M')
        ++nCount;
      switch (nCount)
      {
        case 1:
          return DateToken_m;
        case 2:
          return DateToken_mm;
      }
      return DateTokenUnrecognized;

    case 'd':
    case 'D':
      // Count the number of times this character occurs
      nCount = 1;
      while (*(++lpFormat) == 'd' || *lpFormat == 'D')
        ++nCount;
      switch (nCount)
      {
        case 1:
          return DateToken_d;
        case 2:
          return DateToken_dd;
      }
      return DateTokenUnrecognized;

    case 'y':
    case 'Y':
      // Count the number of times this character occurs
      nCount = 1;
      while (*(++lpFormat) == 'y' || *lpFormat == 'Y')
        ++nCount;
      switch (nCount)
      {
        case 2:
          return DateToken_yy;
        case 4:
          return DateToken_yyyy;
      }
      return DateTokenUnrecognized;

    default:
      // Bump the pointer to the next token
      while (*(++lpFormat) != 'm' && *lpFormat != 'M' &&
                *lpFormat  != 'd' && *lpFormat != 'D' &&
                *lpFormat  != 'y' && *lpFormat != 'Y');
      return DateTokenUnrecognized;
  }
}

//------------------------------------------------------------------------------
//
//  @mfunc    Increments the second field by the specified number.
//
//  @rdesc    None.
//
void CDate::IncSec( int sec_inc )
{
  if ( sec_inc < 0 )
  {
    DecSec( -sec_inc );
    return;
  }

  sec = sec + sec_inc;
  int m_inc = sec / 60;
  sec = sec % 60;

  if (m_inc)
    IncMin(m_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Increments the minute field by the specified number.
//
//  @rdesc    None.
//
void CDate::IncMin( int min_inc )
{
  if ( min_inc < 0 )
  {
    DecMin( -min_inc );
    return;
  }

  minute = minute + min_inc;
  int h_inc = minute / 60;
  minute = minute % 60;

  if (h_inc)
    IncHour(h_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Increments the hour field by the specified number.
//
//  @rdesc    None.
//
void CDate::IncHour( int hour_inc )
{
  if ( hour_inc < 0 )
  {
    DecHour( -hour_inc );
    return;
  }

  hour = hour + hour_inc;
  int d_inc = hour / 24;
  hour = hour % 24;

  if (d_inc)
    IncDay(d_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Increments the day field by the specified number.
//
//  @rdesc    None.
//
void CDate::IncDay( int day_inc )
{
  if ( day_inc < 0 )
  {
    DecDay( -day_inc );
    return;
  }

  while (day_inc)
  {
    int dim = GetDaysInMonth();
    int DaysLeftInMonth = dim - mday;

    if ( day_inc <= DaysLeftInMonth )
    {
      mday = mday + day_inc;
      day_inc = 0;
    }
    else
    {
      day_inc -= DaysLeftInMonth;
      IncMonth();
      mday = 0;
    }
  }
}

//------------------------------------------------------------------------------
//
//  @mfunc    Increments the month field by the specified number.
//
//  @rdesc    None.
//
void CDate::IncMonth( int mon_inc )
{
  if ( mon_inc < 0 )
  {
    DecMonth( -mon_inc );
    return;
  }

  mon = mon + mon_inc;
  int y_inc = (mon-1) / 12;
  mon = (mon-1) % 12 + 1;

  if (y_inc)
    IncYear(y_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Increments the year field by the specified number.
//
//  @rdesc    None.
//
void CDate::IncYear( int year_inc )
{
  if ( year_inc < 0 )
  {
    DecYear( -year_inc );
    return;
  }

  year = year + year_inc;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Decrements the second field by the specified number.
//
//  @rdesc    None.
//
void CDate::DecSec( int sec_inc )
{
  if ( sec_inc < 0 )
  {
    IncSec( -sec_inc );
    return;
  }

  int m_inc = sec_inc / 60;
  sec_inc %= 60;
  if (sec_inc > sec)
  {
    m_inc++;
    sec_inc-=60;
  }
  sec = sec - sec_inc;

  if (m_inc)
    DecMin(m_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Decrements the minute field by the specified number.
//
//  @rdesc    None.
//
void CDate::DecMin( int min_inc )
{
  if ( min_inc < 0 )
  {
    IncMin( -min_inc );
    return;
  }

  int h_inc = min_inc / 60;
  min_inc %= 60;
  if (min_inc > minute)
  {
    h_inc++;
    min_inc-=60;
  }
  minute = minute - min_inc;

  if (h_inc)
    DecHour(h_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Decrements the hour field by the specified number.
//
//  @rdesc    None.
//
void CDate::DecHour( int hour_inc )
{
  if ( hour_inc < 0 )
  {
    IncHour( -hour_inc );
    return;
  }

  int d_inc = hour_inc / 24;
  hour_inc %= 24;
  if (hour_inc > hour)
  {
    d_inc++;
    hour_inc-=24;
  }
  hour = hour - hour_inc;

  if (d_inc)
    DecDay(d_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Decrements the day field by the specified number.
//
//  @rdesc    None.
//
void CDate::DecDay( int day_inc )
{
  if ( day_inc < 0 )
  {
    IncDay( -day_inc );
    return;
  }

  while (day_inc)
  {
    if ( day_inc < mday )
    {
      mday = mday - day_inc;
      day_inc = 0;
    }
    else
    {
      DecMonth();
      day_inc -= mday;
      mday = GetDaysInMonth();
    }
  }
}

//------------------------------------------------------------------------------
//
//  @mfunc    Decrements the month field by the specified number.
//
//  @rdesc    None.
//
void CDate::DecMonth( int mon_inc )
{
  if ( mon_inc < 0 )
  {
    IncMonth( -mon_inc );
    return;
  }

  int y_inc = mon_inc / 12;
  mon_inc %= 12;
  if (mon_inc > mon - 1)
  {
    y_inc++;
    mon_inc-=12;
  }
  mon = mon - mon_inc;

  if (y_inc)
    DecYear(y_inc);
}

//------------------------------------------------------------------------------
//
//  @mfunc    Decrements the year field by the specified number.
//
//  @rdesc    None.
//
void CDate::DecYear( int year_inc )
{
  if ( year_inc < 0 )
  {
    IncYear( -year_inc );
    return;
  }

  year = year - year_inc;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the second field to the specified value.
//
//  @rdesc    None.
//
void CDate::SetSec( int secNew )
{
  if (secNew < 0 || secNew > 59)
    return;
  sec  = secNew;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the minute field to the specified value.
//
//  @rdesc    None.
//
void CDate::SetMin( int minNew )
{
  if (minNew < 0 || minNew > 59)
    return;
  minute  = minNew;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the hour field to the specified value.
//
//  @rdesc    None.
//
void CDate::SetHour( int hourNew )
{
  if (hourNew < 0 || hourNew > 23)
    return;
  hour = hourNew;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the day field to the specified value.
//
//  @rdesc    None.
//
void CDate::SetDay( int mdayNew )
{
  if (mdayNew < 1 || mdayNew > GetDaysInMonth(mon))
    return;
  mday = mdayNew;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the month field to the specified value.
//
//  @rdesc    None.
//
void CDate::SetMonth( int monNew )
{
  if (monNew < 1 || monNew > 12)
    return;
  mon  = monNew;
  if (mday > GetDaysInMonth(mon))
    mday = GetDaysInMonth();
}

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the year field to the specified value.
//
//  @rdesc    None.
//
void CDate::SetYear( int yearNew )
{
  if (yearNew < 1900 || yearNew > 3000)
    return;
  year = yearNew;
}


//------------------------------------------------------------------------------
//
//  @mfunc    Sets the time fields to the specified values.
//
//  @rdesc    None.
//
void CDate::SetTime( int hourNew, int minNew, int secNew )
{
  SetHour( hourNew );
  SetMin( minNew );
  SetSec( secNew );
}

//------------------------------------------------------------------------------
//
//  @mfunc    Sets the date fields to the specified value.
//
//  @rdesc    None.
//
void CDate::SetDate( int monNew, int mdayNew, int yearNew )
{
  SetYear( yearNew );
  SetMonth( monNew );
  SetDay( mdayNew );
}

int CDate::SetDate( const char* lpszDate )
{
  int nState = 0;
  char szChar[28];
  int j=0;
  for (unsigned int i=0; i<min(lstrlen(lpszDate),sizeof(szChar)); i++)
  {
    if (*(lpszDate+i)!=' ')
    {
      szChar[j]=*(lpszDate+i);
      j++;
    }
  }
  szChar[j] = '\0';

  CDate fDate;
  const char* lpszChar = szChar;
  while ( 1 )
  {
    switch( nState )
    {
      case State_Init:  nState = StateInit( lpszChar, fDate );
      break;
      case State_1:     nState = State1( lpszChar, fDate );
      break;
      case State_2:     nState = State2( lpszChar, fDate );
      break;
      case State_3:     nState = State3( lpszChar, fDate );
      break;
      case State_4:     nState = State4( lpszChar, fDate );
      break;
      case State_10:    nState = State10( lpszChar, fDate );
      break;
      case State_11:    nState = State11( lpszChar, fDate );
      break;
      case State_12:    nState = State12( lpszChar, fDate );
      break;

      // This state means that parsing was successful
      case State_End:   *this = fDate; return 0;

      // This case means that a syntax error was found during parsing
      case State_Error: return nState;
    }
  }
}

int CDate::StateInit(const char*& lpChar, CDate& fDate )
{
  rcParse rcp;
  int NextState;

  if ( ParseMonthName(lpChar,rcp) )
  {
    fDate.SetMonth(rcp.nVal);
    NextState = State_2;
  }
  else if ( Parse99Slashor9Slash(lpChar, rcp) )
  {
    fDate.SetMonth(rcp.nVal);
    NextState = State_2;
  }
  else if ( Parse99Colonor9Colon(lpChar, rcp) )
  {
    fDate.SetHour(rcp.nVal);
    NextState = State_10;
  }
  else if ( Parse99or9(lpChar, rcp) )
  {
    fDate.SetDay(rcp.nVal);
    NextState = State_1;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDate::State1(const char*& lpChar, CDate& fDate )
{
  rcParse rcp;
  int NextState;

  if ( ParseMonthName(lpChar,rcp) )
  {
    fDate.SetMonth(rcp.nVal);
    NextState = State_3;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDate::State2(const char*& lpChar, CDate& fDate )
{
  rcParse rcp;
  int NextState;

  if ( Parse99Slashor9Slash(lpChar, rcp) )
  {
    fDate.SetDay(rcp.nVal);
    NextState = State_3;
  }
  else if ( Parse99Commaor9Comma(lpChar, rcp) )
  {
    fDate.SetDay(rcp.nVal);
    NextState = State_12;
  }
  else if ( Parse99or9(lpChar, rcp) )
  {
    fDate.SetDay(rcp.nVal);
    NextState = State_4;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDate::State3(const char*& lpChar, CDate& fDate )
{
  rcParse rcp;
  int NextState;

  if ( Parse9999Commaor99Comma(lpChar, rcp) )
  {
    fDate.SetYear(rcp.nVal);
    NextState = State_12;
  }
  else if ( Parse9999or99(lpChar, rcp) )
  {
    fDate.SetYear(rcp.nVal);
    NextState = State_4;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDate::State4(const char*& lpChar, CDate& )
{
  rcParse rcp;
  int NextState;

  if ( ParseEOM(lpChar, rcp) )
    NextState = State_End;
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDate::State10(const char*& lpChar, CDate& fDate)
{
  rcParse rcp;
  int NextState;

  if ( Parse99Colonor9Colon(lpChar, rcp) )
  {
    fDate.SetMin(rcp.nVal);
    NextState = State_11;
  }
  else if ( Parse99or9(lpChar, rcp) )
  {
    fDate.SetMin(rcp.nVal);
    NextState = State_4;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDate::State11(const char*& lpChar, CDate& fDate)
{
  rcParse rcp;
  int NextState;

  if ( Parse99or9(lpChar, rcp) )
  {
    fDate.SetSec(rcp.nVal);
    NextState = State_4;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDate::State12(const char*& lpChar, CDate& fDate)
{
  rcParse rcp;
  int NextState;

  if ( Parse99Colonor9Colon(lpChar, rcp) )
  {
    fDate.SetHour(rcp.nVal);
    NextState = State_10;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Provides the ability to add to the second field.
//
//  @rdesc    The resulting CDate value.
//
CDate CDate::operator + ( const CSec inc ) const
{
  CDate mD(*this);
  mD.IncSec(inc);
  return mD;
}

CDate CDate::operator + ( const CMin inc ) const
{
  CDate mD(*this);
  mD.IncMin(inc);
  return mD;
}

CDate CDate::operator + ( const CHour inc ) const
{
  CDate mD(*this);
  mD.IncHour(inc);
  return mD;
}

CDate CDate::operator + ( const CDay inc ) const
{
  CDate mD(*this);
  mD.IncDay(inc);
  return mD;
}

CDate CDate::operator + ( const CMon inc ) const
{
  CDate mD(*this);
  mD.IncMonth(inc);
  return mD;
}

CDate CDate::operator + ( const CYear inc ) const
{
  CDate mD(*this);
  mD.IncYear(inc);
  return mD;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Provides the ability to subtract from the second field.
//
//  @rdesc    The resulting CDate value.
//
CDate CDate::operator - ( const CSec inc ) const
{
  CDate mD(*this);
  mD.DecSec(inc);
  return mD;
}

CDate CDate::operator - ( const CMin inc ) const
{
  CDate mD(*this);
  mD.DecMin(inc);
  return mD;
}

CDate CDate::operator - ( const CHour inc ) const
{
  CDate mD(*this);
  mD.DecHour(inc);
  return mD;
}

CDate CDate::operator - ( const CDay inc ) const
{
  CDate mD(*this);
  mD.DecDay(inc);
  return mD;
}

CDate CDate::operator - ( const CMon inc ) const
{
  CDate mD(*this);
  mD.DecMonth(inc);
  return mD;
}

CDate CDate::operator - ( const CYear inc ) const
{
  CDate mD(*this);
  mD.DecYear(inc);
  return mD;
}

//------------------------------------------------------------------------------
//
//  @mfunc    Provides the ability to set the date/time from a time_t.
//
//  @rdesc    None.
//
void CDate::operator = ( const time_t time )
{
  tm* t  = localtime(&(time));

  sec    = CSec (t->tm_sec);
  minute = CMin (t->tm_min);
  hour   = CHour(t->tm_hour);
  mday   = CDay (t->tm_mday);
  mon    = CMon (t->tm_mon + 1);      // our day of month is 1-based, not 0-based like dos
  year   = CYear(t->tm_year + 1900);  // dos routines only handle post-1900 dates
}

//------------------------------------------------------------------------------
//
//  @mfunc    Provides the ability to construct the date/time from a system
//            clock value.
//
//  @rdesc    None.
//

CDate::CDate(const FILETIME ft)
{
    CTime c(ft);
    struct tm t;
    c.GetGmtTm(&t);

    sec    = CSec (t.tm_sec);
    minute = CMin (t.tm_min);
    hour   = CHour(t.tm_hour);
    mday   = CDay (t.tm_mday);
    mon    = CMon (t.tm_mon + 1);      // our day of month is 1-based, not 0-based like dos
    year   = CYear(t.tm_year + 1900);  // dos routines only handle post-1900 dates
}

//------------------------------------------------------------------------------
//
//  @mfunc    Provides the ability to get the date/time in the form of an ATime.
//
//  @rdesc    None.
//

#if 0
CDate::operator ATime (void) const
{
    ATime atime;
    atime.SetDate(year, mon, mday);
    atime.SetTime(hour, minute);
    return atime;
}

CDate::CDate(const RTime &rtm)
{
	ATime atm(rtm);

	year = atm.Year();
	mon = atm.Month();
	mday = atm.Day();
	hour = atm.Hour();
	minute = atm.Minute();
	sec = 0;
}

CDate::operator RTime (void) const
{
	ATime atm;
	atm.SetDate(year, mon, mday);
    atm.SetTime(hour, minute);
	RTime rtm(atm);

	return(rtm);
}
#endif

//------------------------------------------------------------------------------
//
//  @mfunc    Provides the ability to make the CDate persistent.
//
//  @rdesc    None.
//
void CDate::Serialize( CArchive& ar )
{
    if (ar.IsStoring())
    {
        ar << BYTE(39);  // version number--started at 38 for improved error-catching

         // serialize out the data specific to this class only
        ar << (WORD) GetYear();
        ar << (BYTE) GetMonth();
        ar << (BYTE) GetDay();
        ar << (BYTE) GetHour();
        ar << (BYTE) GetMin();
        ar << (BYTE) GetSec();
    }
    else
    {
        BYTE bVer;
        ar >> bVer;        // Read in the version.
        switch( bVer )
        {
        case 39:
            {
                // serialize in the data specific to this class only
                WORD wTemp;
                BYTE bTemp;

                ar >> wTemp; SetYear(wTemp);
                ar >> bTemp; SetMonth(bTemp);
                ar >> bTemp; SetDay(bTemp);
                ar >> bTemp; SetHour(bTemp);
                ar >> bTemp; SetMin(bTemp);
                ar >> bTemp; SetSec(bTemp);
            }
            break;
        default:
            // fail the load!!!  invalid binder file!!!
            AfxThrowArchiveException(CArchiveException::generic);
            ASSERT(FALSE);
        }
    }
}

CArchive& operator>> (CArchive& ar, CDate& d)
{
    d.Serialize(ar);
    return ar;
}

CArchive& operator<< (CArchive& ar, CDate& d)
{
    d.Serialize(ar);
    return ar;
}


//------------------------------------------------------------------------------
//
//  @mfunc    Sets the range for the CDateRange object.  Return code zero
//            means that the range was set properly based on the specified
//            input string.
//
//  @rdesc    None.
//
int CDateRange::SetRange(LPCSTR lpszDateRange)
{
  int nState = 0;
  const char* lpChar = lpszDateRange;
  while ( 1 )
  {
    switch( nState )
    {
      case State_Init:  nState = StateInit( lpChar, m_fDate, m_lDate );
      break;
      case State_1:     nState = State1( lpChar, m_fDate, m_lDate );
      break;
      case State_2:     nState = State2( lpChar, m_fDate, m_lDate );
      break;
      case State_3:     nState = State3( lpChar, m_fDate, m_lDate );
      break;
      case State_4:     nState = State4( lpChar, m_fDate, m_lDate );
      break;
      case State_5:     nState = State5( lpChar, m_fDate, m_lDate );
      break;
      case State_6:     nState = State6( lpChar, m_fDate, m_lDate );
      break;
      case State_7:     nState = State7( lpChar, m_fDate, m_lDate );
      break;
      case State_8:     nState = State8( lpChar, m_fDate, m_lDate );
      break;
      case State_9:     nState = State9( lpChar, m_fDate, m_lDate );
      break;
      case State_End:   return 0;
      case State_Error: return nState;
    }
  }
}

int CDateRange::StateInit(const char*& lpChar, CDate& fDate, CDate& )
{
  rcParse rcp;
  int NextState;

  if ( ParseMonthName(lpChar,rcp) )
  {
    fDate.SetMonth(rcp.nVal);
    NextState = State_2;
  }
  else if ( Parse99Slashor9Slash(lpChar, rcp) )
  {
    fDate.SetMonth(rcp.nVal);
    NextState = State_2;
  }
  else if ( Parse99or9(lpChar, rcp) )
  {
    fDate.SetDay(rcp.nVal);
    NextState = State_1;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State1(const char*& lpChar, CDate& fDate, CDate& )
{
  rcParse rcp;
  int NextState;

  if ( ParseMonthName(lpChar,rcp) )
  {
    fDate.SetMonth(rcp.nVal);
    NextState = State_4;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State2(const char*& lpChar, CDate& fDate, CDate& )
{
  rcParse rcp;
  int NextState;

  if ( Parse99Slashor9Slash(lpChar, rcp) )
  {
    fDate.SetDay(rcp.nVal);
    NextState = State_3;
  }
  else if ( Parse99or9(lpChar, rcp) )
  {
    fDate.SetDay(rcp.nVal);
    NextState = State_4;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State3(const char*& lpChar, CDate& fDate, CDate& )
{
  rcParse rcp;
  int NextState;

  if ( Parse9999or99(lpChar, rcp) )
  {
    fDate.SetYear(rcp.nVal);
    NextState = State_4;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State4(const char*& lpChar, CDate& fDate, CDate& lDate )
{
  rcParse rcp;
  int NextState;

  if ( ParseDash(lpChar, rcp) )
  {
    NextState = State_5;
  }
  else if ( ParseEOM(lpChar, rcp) )
  {
    lDate = fDate;
    NextState = State_End;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State5(const char*& lpChar, CDate& fDate, CDate& lDate )
{
  rcParse rcp;
  int NextState;

  if ( Parse99NTor9NT(lpChar,rcp) )
  {
    lDate = fDate;
    lDate += CDay(rcp.nVal);
    NextState = State_9;
  }
  else if ( ParseMonthName(lpChar,rcp) )
  {
    lDate.SetMonth(rcp.nVal);
    NextState = State_7;
  }
  else if ( Parse99Slashor9Slash(lpChar, rcp) )
  {
    lDate.SetMonth(rcp.nVal);
    NextState = State_7;
  }
  else if ( Parse99or9(lpChar, rcp) )
  {
    lDate.SetDay(rcp.nVal);
    NextState = State_6;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State6(const char*& lpChar, CDate&, CDate& lDate )
{
  rcParse rcp;
  int NextState;

  if ( ParseMonthName(lpChar,rcp) )
  {
    lDate.SetMonth(rcp.nVal);
    NextState = State_9;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State7(const char*& lpChar, CDate&, CDate& lDate )
{
  rcParse rcp;
  int NextState;

  if ( Parse99Slashor9Slash(lpChar, rcp) )
  {
    lDate.SetDay(rcp.nVal);
    NextState = State_8;
  }
  else if ( Parse99or9(lpChar, rcp) )
  {
    lDate.SetDay(rcp.nVal);
    NextState = State_9;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State8(const char*& lpChar, CDate&, CDate& lDate )
{
  rcParse rcp;
  int NextState;

  if ( Parse9999or99(lpChar, rcp) )
  {
    lDate.SetYear(rcp.nVal);
    NextState = State_9;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

int CDateRange::State9(const char*& lpChar, CDate&, CDate& lDate )
{
  rcParse rcp;
  int NextState;

  if ( ParseEOM(lpChar, rcp) )
  {
    lDate = lDate;
    NextState = State_End;
  }
  else
    return State_Error;

  lpChar += rcp.nBump;
  return NextState;
}

//------------------------------------------------------------------------
//
//  @mfunc    Creates a text string representation of the date value that
//            matches the format that is parsed for in SetDate.
//
//  @rdesc    None.
//
void CDateRange::GetParseDate(CString& str) const
{
    m_fDate.GetParseDate(str);
    str += "-";
    CString strTemp;
    m_lDate.GetParseDate(strTemp);
    str += strTemp;
}

void DDX_CDate(CDataExchange* pDX, int nIDC, CDate& value)
{
    HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
    CString strWndText;
    if (pDX->m_bSaveAndValidate)
    {
        int nLen = ::GetWindowTextLength(hWndCtrl);
        ::GetWindowText(hWndCtrl, strWndText.GetBufferSetLength(nLen), nLen+1);
        strWndText.ReleaseBuffer();
        if (value.SetDate(strWndText))
            pDX->Fail();
    }
    else
    {
        value.GetParseDate(strWndText);
        int nNewLen = strWndText.GetLength();
        TCHAR szOld[256];
        // fast check to see if text really changes (reduces flash in controls)
        if (nNewLen > sizeof(szOld) ||
            ::GetWindowText(hWndCtrl, szOld, sizeof(szOld)) != nNewLen ||
            lstrcmp(szOld, strWndText) != 0)
        {
            // change it
            ::SetWindowText(hWndCtrl, strWndText);
        }
    }
}

