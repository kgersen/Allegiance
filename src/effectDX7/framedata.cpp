#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// FrameData
//
//////////////////////////////////////////////////////////////////////////////

ZString GetString(int indent, const FrameData& data)
{
    return
          "("
               + ::GetString(indent + 1, data.m_strName)
        + ", " + ::GetString(indent + 1, data.m_vecPosition)
        + ", " + ::GetString(indent + 1, data.m_vecForward)
        + ", " + ::GetString(indent + 1, data.m_vecUp)
        + ")";
}

ZString FrameDataListValue::GetString(int indent)
{
    ZString str = "FrameData([\n";

    FrameList::Iterator iter(m_list);

    while (!iter.End()) {
        str += Value::Indent(indent + 1) + ::GetString(indent, iter.Value());
        iter.Next();
        if (!iter.End()) {
            str += ",\n";
        } else {
            str += "\n";
        }
    }

    return str + Value::Indent(indent) + "])";
}

void FrameDataListValue::Write(IMDLBinaryFile* pmdlFile)
{
    pmdlFile->WriteReference("FrameData");
    TRef<ZFile> pfile = pmdlFile->WriteBinary();

    pfile->Write(m_list.GetCount());

    FrameList::Iterator iter(m_list);

    while (!iter.End()) {
        FrameData& data = iter.Value();

        pfile->WriteAlignedString(data.m_strName);
        pfile->Write((void*)&data.m_vecPosition, 3 * sizeof(Vector));

        iter.Next();
    }
}
