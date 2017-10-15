//////////////////////////////////////////////////////////////////////////////
//
// FrameData
//
//////////////////////////////////////////////////////////////////////////////

class FrameData {
public:
    ZString m_strName;
    Vector  m_vecPosition;
    Vector  m_vecForward;
    Vector  m_vecUp;

    FrameData(){}

    FrameData(
        const ZString& strName,
        const Vector& vecPosition,
        const Vector& vecForward,
        const Vector& vecUp
    ) :
        m_strName(strName),
        m_vecPosition(vecPosition),
        m_vecForward(vecForward),
        m_vecUp(vecUp)
    {
    }

    FrameData(const FrameData& fd) :
        m_strName(fd.m_strName),
        m_vecPosition(fd.m_vecPosition),
        m_vecForward(fd.m_vecForward),
        m_vecUp(fd.m_vecUp)
    {
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// FrameDataListValue
//
//////////////////////////////////////////////////////////////////////////////

class FrameDataListValue : public Value {
public:
    typedef TList<FrameData, DefaultNoEquals> FrameList;

private:
    FrameList m_list;

public:
    FrameList& GetList() { return m_list; }

    ZString GetFunctionName()
    {
        return "FrameData";
    }

    ZString GetString(int indent);
    void Write(IMDLBinaryFile* pmdlFile);
};
