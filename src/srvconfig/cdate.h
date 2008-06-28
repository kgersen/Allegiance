#ifndef _CDATE_H
#define _CDATE_H

//-----------------------------------------------------------------------------
// @doc
// @module cdate.h : definitions for the CDate class |
//
// This class encapsulates date and time and many useful date/time manipulation
// routines.
//
// Copyright <cp> 1995 Microsoft Corporation, All Rights Reserved
//
// Mark Snyder, Software Design Engineer

#ifndef _INC_TIME
#include "time.h"
#endif
//#ifndef _STDTIME_H
//#include "stdtime.h"
//#endif

// data structure used during parsing of date strings
struct rcParse { int nBump; int nVal; };

class TValue
{                                         
protected:
  int value;                              
public:
  TValue(int i=0) : value(i) {};

  operator int() const { return value; }
  operator + ( const int increment ) const { return value + increment; }
  operator - ( const int increment ) const { return value - increment; }
//  operator +=( const int increment ) { value += increment; return value; }
//  operator -=( const int increment ) { value -= increment; return value; }
  operator ++( const int increment ) { value++; return value; }
  operator --( const int increment ) { value--; return value; }
};

class  CYear : public TValue { public: CYear(int i=0) : TValue(i) {} };
class  CMon  : public TValue { public: CMon (int i=0) : TValue(i) {} };
class  CDay  : public TValue { public: CDay (int i=0) : TValue(i) {} };
class  CHour : public TValue { public: CHour(int i=0) : TValue(i) {} };
class  CMin  : public TValue { public: CMin (int i=0) : TValue(i) {} };
class  CSec  : public TValue { public: CSec (int i=0) : TValue(i) {} };

//------------------------------------------------------------------------------
//
// @class   This class is used to contain a date and time object which can 
//          manipulated and used to keep track of a date/time value.  Other 
//          services provided allow for querying the day of week, day of year, 
//          etc.
//
class CDate 
{
public:
    // This enumeration is used to parse the Windows int date format
    enum DateToken
    {
        DateToken_m,
        DateToken_mm,
        DateToken_d,
        DateToken_dd,
        DateToken_yy,
        DateToken_yyyy,
        DateTokenUnrecognized,
    };

private:
    CYear  year;   // Year         (calendar year; i.e., 1989)
    CMon   mon;    // Month        (1--12)
    CDay   mday;   // Day of month (1--31)
    CHour  hour;   // Hour         (0--23)
    CMin   minute; // Minutes      (0--59)
    CSec   sec;    // Seconds      (0--59)
    //ATime m_atm;

    // state transition functions for parsing a string containing a date/time
    int StateInit(const char*& lpChar, CDate& fDate );
    int State1(const char*& lpChar, CDate& fDate );
    int State2(const char*& lpChar, CDate& fDate );
    int State3(const char*& lpChar, CDate& fDate );
    int State4(const char*& lpChar, CDate& fDate );
    int State10(const char*& lpChar, CDate& fDate );
    int State11(const char*& lpChar, CDate& fDate );
    int State12(const char*& lpChar, CDate& fDate );

protected:
    int DivisibleUnits( int x, int y, int z ) const;
    int LeapYearsBetween( int x, int y ) const;
    int abs( int x ) const { return (x>0) ? x : -x; }
    void IncDay  ( int incday  = 1 );
    void IncMonth( int incmon  = 1 );
    void IncYear ( int incyear = 1 );
    void IncHour ( int inchour = 1 );
    void IncMin  ( int incmin  = 1 );
    void IncSec  ( int incsec  = 1 );
    void DecDay  ( int incday  = 1 );
    void DecMonth( int incmon  = 1 );
    void DecYear ( int incyear = 1 );
    void DecHour ( int inchour = 1 );
    void DecMin  ( int incmin  = 1 );
    void DecSec  ( int incsec  = 1 );

    // Method used for parsing the Windows int date format
    DateToken GetNextDateToken(LPCSTR & lpFormat);

public:
    CDate() { SetDateCurrent(); }
    CDate(CYear newyear, CMon newmonth, CDay newmday, CHour newhour, CMin newminute, CSec newsecond )
        : sec(newsecond), minute(newminute), hour(newhour), mday(newmday), mon(newmonth), year(newyear) {}
    //CDate(const ATime& a) 
    //    : year(a.Year()), mon(a.Month()), mday(a.Day()), hour(a.Hour()), minute(a.Minute()), sec(0) {}
    //CDate(const RTime&); 
    CDate(const CDate& d) 
        : year(d.year), mon(d.mon), mday(d.mday), hour(d.hour), minute(d.minute), sec(d.sec) {}
    CDate(const FILETIME ft);

    int   GetDay() const { return mday; }
    int   GetMonth() const { return mon; }
    int   GetYear() const { return year; }
    int   GetHour() const { return hour; }
    int   GetMin() const { return minute; }
    int   GetSec() const { return sec; }
    int   GetDate( LPSTR szBuf, int nBufSize, LPCSTR szFmt );
    void  GetWinTime(CString& str);
    void  GetWinShortDate(CString& str);
    void  GetParseTime(CString& str);
    void  GetParseDate(CString& str) const;
    void  GetFiletime(FILETIME* ft) const;
    CDate GetDate() const { CDate t(*this); t.SetTime(0,0,0); return t; }
    CDate GetTime() const { CDate t(*this); t.SetDate(1,1,1900); return t; }
    int   GetDayOfWeek() const;
    int   GetDayOfYear() const;
    int   GetDaysInMonth() const;
    int   GetDaysInMonth( int i ) const;
    int   GetWeeksInMonth() const;
    void  SetDay  ( int mday );
    void  SetMonth( int mon );
    void  SetYear ( int year );
    void  SetHour ( int hour );
    void  SetMin  ( int minute );
    void  SetSec  ( int sec );
    void  SetTime ( int hour, int minute, int sec );
    void  SetDate ( int mon, int mday, int year );
    int   SetDate ( const char* lpszDate );
    void  SetDateCurrent();

    int   DayDiff( const CDate& ADate ) const;
    int   isLeapYear() const;

    BOOL  isLessThan(const CDate& testDate) const;
    BOOL  isEqualTo(const CDate& testDate) const;
    BOOL  operator <( const CDate& testDate )   const { return isLessThan(testDate); }
    BOOL  operator ==( const CDate& testDate )  const { return isEqualTo(testDate); }
    BOOL  operator !=( const CDate& testDate )  const { return !isEqualTo(testDate); }
    BOOL  operator <=( const CDate& testDate )  const { return isLessThan(testDate) || isEqualTo(testDate); }
    BOOL  operator >( const CDate& testDate )   const { return !(isLessThan(testDate) || isEqualTo(testDate)); }
    BOOL  operator >=( const CDate& testDate )  const { return !isLessThan(testDate); }
    CDate operator +( const CSec  sec_inc )  const;
    CDate operator +( const CMin  min_inc )  const;
    CDate operator +( const CHour hour_inc ) const;
    CDate operator +( const CDay  mday_inc ) const;
    CDate operator +( const CMon  mon_inc )  const;
    CDate operator +( const CYear year_inc ) const;
    CDate operator -( const CSec  sec_inc )  const;
    CDate operator -( const CMin  min_inc )  const;
    CDate operator -( const CHour hour_inc ) const;
    CDate operator -( const CDay  mday_inc ) const;
    CDate operator -( const CMon  mon_inc )  const;
    CDate operator -( const CYear year_inc ) const;
    void  operator =( const CDate& a ) { SetDate(a.GetMonth(), a.GetDay(), a.GetYear());  SetTime(a.GetHour(), a.GetMin(), a.GetSec()); }
    //void  operator =( const ATime& a ) { SetDate(a.Month(), a.Day(), a.Year());  SetTime(a.Hour(), a.Minute(), 0); }
    //void  operator =( const RTime& r ) { operator=((ATime)r); }
    void  operator =( const time_t time );
//    operator ATime () const;
    //operator RTime (void) const;

    virtual void  Serialize( CArchive& ar );
};

void DDX_CDate(CDataExchange* pDX, int nIDC, CDate& value);

CArchive& operator>> (CArchive& ar, CDate& d);
CArchive& operator<< (CArchive& ar, CDate& d);

// These templates are needed to allow us to add/sub any class to a CDate for which
//    CDate supplies the + and - that these use for valid math types, namely the
//    classes CMin, CSec, CHour, CMonth, CDay, and CYear.
#define protoPlusEquals(type)  \
  CDate operator+=(CDate& d,const type& t)
#define protoMinusEquals(type)  \
  CDate operator-=(CDate& d,const type& t)
  
protoPlusEquals(CYear);
protoMinusEquals(CYear);
protoPlusEquals(CMon);
protoMinusEquals(CMon);
protoPlusEquals(CDay);
protoMinusEquals(CDay);
protoPlusEquals(CHour);
protoMinusEquals(CHour);
protoPlusEquals(CMin);
protoMinusEquals(CMin);
protoPlusEquals(CSec);
protoMinusEquals(CSec);

class CDateRange
{
private:
    // State transition functions
    int StateInit(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State1(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State2(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State3(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State4(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State5(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State6(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State7(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State8(const char*& lpChar, CDate& fDate, CDate& lDate );
    int State9(const char*& lpChar, CDate& fDate, CDate& lDate );

public:
    CDate m_fDate, m_lDate;

    CDateRange() {}
    CDateRange(LPCSTR lpszDate) { SetRange(lpszDate); }
    CDateRange(CDate fDate, CDate lDate) { m_fDate = fDate; m_lDate = lDate; }
    int SetRange(LPCSTR lpszDate);
    void  GetParseDate(CString& str) const;
};

#endif
