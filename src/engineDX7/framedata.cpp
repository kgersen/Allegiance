#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// FrameData
//
//////////////////////////////////////////////////////////////////////////////

ZString GetString(int indent, const FrameData& data)
{
    return
          "FrameData("
               + ::GetString(indent + 1, data.m_strName)
        + ", " + ::GetString(indent + 1, data.m_vecPosition)
        + ", " + ::GetString(indent + 1, data.m_vecForward)
        + ", " + ::GetString(indent + 1, data.m_vecUp)
        + ")";
}
