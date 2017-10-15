#include "pch.h"

#define _UNALIGNED

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class MDLBinaryNameSpace :
    public INameSpace,
    public IBinaryReaderSite
{
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // types
    //
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    //
    // data members
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString                    m_strName;
    TRef<Modeler>              m_pmodeler;
    TRef<ZFile>                m_pfile;
    BYTE*                      m_pdata;
    TVector<TRef<INameSpace> > m_pnsImports;
    TVector<ZString>           m_strExportNames;
    TVector<TRef<IObject> >    m_pobjectImports;
    TVector<TRef<IObject> >    m_pobjects;

    //////////////////////////////////////////////////////////////////////////////
    //
    // methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool Error(const ZString& str)
    {
        ZError(str);
        return false;
    }

    DWORD ReadDWORD()
    {
        ZAssert(m_pdata);
        m_pdata += sizeof(DWORD);

        return *(_UNALIGNED DWORD*)(m_pdata - sizeof(DWORD));
    }

    float ReadFloat()
    {
        ZAssert(m_pdata);
        m_pdata += sizeof(float);

        return *(_UNALIGNED float*)(m_pdata - sizeof(float));
    }

    ZString ReadString()
    {
        ZAssert(m_pdata);

        //  : this will cause an exception if the NameSpace is freed before
        //             All of the objects that reference the string
        //
        // ZString str((PCC)m_pdata, true);

        ZString str((PCC)m_pdata);

        int length       = str.GetLength() + 1;
        int paddedlength = (length + 3) & (~0x3);

        m_pdata += paddedlength;
        return str;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IObject> ReadList()
    {
        TRef<IObjectList> plist = new IObjectList();
        DWORD count = ReadDWORD();

        for (DWORD index = 0; index < count; index++) {
            TRef<IObject> pobject = ReadObject();
            plist->PushEnd(pobject);
        }

        return plist;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IObject> ReadApply(ObjectStack& stack)
    {
        TRef<IFunction> pfunction; CastTo(pfunction, stack.Pop());

        return pfunction->Apply(stack);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IObject> ReadBinary(ObjectStack& stack)
    {
        TRef<IFunction> pfunction; CastTo(pfunction, stack.Pop());

        return pfunction->Read(this, stack);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IObject> ReadPair(ObjectStack& stack)
    {
        TRef<IObject> pobject1 =  stack.Pop();
        TRef<IObject> pobject2 =  stack.Pop();

        return new IObjectPair(pobject1, pobject2);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IObject> ReadObject()
    {
        ObjectStack stack;

        while (true) {
            DWORD token = GetDWORD();

            switch (token) {
                case ObjectFloat:
                    stack.Push(new Number(ReadFloat()));
                    break;

                case ObjectString:
                    stack.Push(new StringValue(ReadString()));
                    break;

                case ObjectTrue:
                    stack.Push(new Boolean(true));
                    break;

                case ObjectFalse:
                    stack.Push(new Boolean(false));
                    break;

                case ObjectList:
                    stack.Push(ReadList());
                    break;

                case ObjectApply:
                    stack.Push(ReadApply(stack));
                    break;

                case ObjectBinary:
                    stack.Push(ReadBinary(stack));
                    break;

                case ObjectReference:
                    stack.Push(m_pobjects[ReadDWORD()]);
                    break;

                case ObjectImport:
                    stack.Push(m_pobjectImports[ReadDWORD()]);
                    break;

                case ObjectPair:
                    stack.Push(ReadPair(stack));
                    break;

                case ObjectEnd:
                    ZAssert(stack.GetCount() == 1);
                    return stack.Pop();

                default:
                    ZError("ReadObject: Bad token");
                    break;
            }
        };
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadFile()
    {
        DWORD index;
        DWORD magic            = ReadDWORD();
        DWORD version          = ReadDWORD();
        DWORD countNameSpaces  = ReadDWORD();
        DWORD countImports     = ReadDWORD();
        DWORD countExports     = ReadDWORD();
        DWORD countDefinitions = ReadDWORD();

        if (magic != MDLMagic) {
            return Error("Invalid file type");
        }

        if (version != MDLVersion) {
            return Error("Invalid version number");
        }

        //
        // Imported NameSpaces
        //

        m_pnsImports.SetCount(countNameSpaces);

        for(index = 0; index < countNameSpaces; index++) {
            ZString str = ReadString();
            TRef<INameSpace> pns = m_pmodeler->GetNameSpace(str);
            ZAssert(pns != NULL);

            m_pnsImports.Set(index, pns);
        }

        //
        // Imports
        //

        m_pobjectImports.SetCount(countImports);
        for(index = 0; index < countImports; index++) {
            DWORD indexLibrary = ReadDWORD();
            ZString str = ReadString();

            ZAssert(indexLibrary < countNameSpaces);
            
            TRef<IObject> pobject = m_pnsImports[indexLibrary]->FindMemberLocal(str);

            ZAssert(pobject != NULL);

            m_pobjectImports.Set(index, pobject);
        }

        //
        // Exports
        //

        m_pobjects.SetCount(countExports + countDefinitions);
        m_strExportNames.SetCount(countExports);

        for(index = 0; index < countExports; index++) {
            m_strExportNames.Set(index, ReadString());
        }

        //
        // Definitions
        //

        for(index = 0; index < countExports + countDefinitions; index++) {
            DWORD indexObject = ReadDWORD();
            m_pobjects.Set(indexObject, ReadObject());
        }

        //
        // Set the namespace info's on any values
        //
        // !!! this is a total hack, only do this on Debug builds since it
        //     requires dynamic casting.  MDLC is always built debug so it's
        //     ok.
        //

        #if defined(_DEBUG)
            for(index = 0; index < countExports; index++) {
                Value* pvalue = dynamic_cast<Value*>((IObject*)m_pobjects[index]);

                if (pvalue != NULL) {
                    pvalue->SetNameSpaceInfo(CreateNameSpaceInfo(this, m_strExportNames[index]));
                }
            }
        #endif

        return true;
    }

public:
    MDLBinaryNameSpace(const ZString& strName, Modeler* pmodeler, ZFile* pfile) :
        m_pmodeler(pmodeler),
        m_strName(strName)
    {
        m_pfile = pfile;
        m_pdata = pfile->GetPointer(false, false);
        ReadFile();
        m_pdata = NULL;
        pfile = NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // INameSpace members
    //
    //////////////////////////////////////////////////////////////////////////////

    const ZString& GetName()
    {
        return m_strName;
    }

    void AddMember(const ZString& strName, IObject* pobject)
    {
        ZUnimplemented();
    }

    IObject* FindMemberAndNameSpace(const ZString& strName, INameSpace*& pns)
    {
        IObject* pobject = FindMemberLocal(strName);

        if (pobject != NULL) {
            pns = this;
            return pobject;
        }

        int count = m_pnsImports.GetCount();

        for(int index = 0; index < count; index++) {
            pobject = m_pnsImports[index]->FindMemberAndNameSpace(strName, pns);

            if (pobject != NULL) {
                return pobject;
            }
        }

        return NULL;
    }

    IObject* FindMember(const ZString& strName)
    {
        INameSpace* pns;

        return FindMemberAndNameSpace(strName, pns);
    }

    IObject* FindMemberLocal(const ZString& strName)
    {
        int count = m_strExportNames.GetCount();

        for(int index = 0; index < count; index++) {
            if (m_strExportNames[index] == strName) {
                return m_pobjects[index];
            }
        }

        return NULL;
    }

    void AddType(const ZString& strName, MDLType* ptype)
    {
        ZUnimplemented();
    }

    MDLType* FindType(const ZString& strName)               
    {
        ZUnimplemented();
        return NULL;
    }

    MDLType* FindTypeLocal(const ZString& strName)          
    {
        ZUnimplemented();
        return NULL;
    }

    void WriteTypeHeader(ZFile* pfile)
    {
        ZUnimplemented();
    }

    void WriteToTextFile(int indent, ZFile* pfile)
    {
        ZUnimplemented();
    }

    void WriteToBinaryFile(ZFile* pfile)
    {
        ZUnimplemented();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IBinaryReaderSite methods
    //
    //////////////////////////////////////////////////////////////////////////////

    ZString GetString()
    {
        return ReadString();
    }

	ZString GetCurrentFile()
	{
		return m_strName;
	}

    DWORD GetDWORD()
    {
        return ReadDWORD();
    }

    BYTE* GetPointer()
    {
        ZAssert(m_pdata);
        return m_pdata;
    }

    void  MovePointer(int count)
    {
        ZAssert(m_pdata);
        m_pdata += count;
    }

    IObject* GetMemoryObject()
    {
        ZAssert(m_pfile);
        return m_pfile;
    }

    IObject* GetObject()
    {
        return m_pobjects[ReadDWORD()];
    }
};

TRef<INameSpace> CreateBinaryNameSpace(const ZString& strName, Modeler* pmodeler, ZFile* pfile)
{
    return new MDLBinaryNameSpace(strName, pmodeler, pfile);
}
