#include "ztime.h"
#include "zassert.h"

tlsDWORD Time::s_dwPauseStart;
tlsDWORD Time::s_dwNegativeOffset;

#ifdef _DEBUG_TRAINING
  tlsDWORD Time::s_dwLastTime;
  tlsDWORD Time::s_dwAccumulatedTime;
  tlsINT   Time::s_iShift;
  tlsDWORD Time::s_dwLastClockTime;
#endif

void    Time::Pause (void)
{
    assert (! IsPaused ());

  #ifdef _DEBUG_TRAINING
    // save the current time as the clock
    s_dwPauseStart = s_dwAccumulatedTime;
  #else
    // save the current time as the clock
    s_dwPauseStart = timeGetTime ();
  #endif
}

bool    Time::IsPaused (void)
{
    // the clock is paused if s_dwPauseStart is non-zero
    return (s_dwPauseStart != 0) ? true : false;
}

void    Time::Continue (void)
{
    assert (IsPaused ());

  #ifdef _DEBUG_TRAINING
    // get the current time to see how long the system was paused,
    // and accumulate that into the clock offset.
    s_dwNegativeOffset += s_dwAccumulatedTime - s_dwPauseStart;
  #else
    // get the current time to see how long the system was paused,
    // and accumulate that into the clock offset.
    s_dwNegativeOffset += timeGetTime () - s_dwPauseStart;
  #endif

    // clear the pause counter
    s_dwPauseStart = 0;
}

#ifdef _DEBUG_TRAINING
int     Time::GetShift (void)
{
    // return the current shift value
    return s_iShift;
}

void    Time::SetShift (int iShift)
{
    // limit the amount of speed change
    if (iShift > 3)
        iShift = 3;
    else if (iShift < -3)
        iShift = -3;

    // set the shift value
    s_iShift = iShift;
}
#endif
