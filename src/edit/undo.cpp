#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// MultiUndo
//
//////////////////////////////////////////////////////////////////////////////

class MultiUndoImpl : public MultiUndo {
private:
    TList< TRef<IUndo> > m_list;

public:
    bool Add(IUndo* pundo)
    {
        if (pundo != NULL) {
            m_list.PushFront(pundo);
            return true;
        } else {
            return false;
        }
    }

    TRef<IUndo> Apply()
    {
        TRef<MultiUndo> pmundo = new MultiUndoImpl();

        TList< TRef<IUndo> >::Iterator iter(m_list);

        while (!iter.End()) {
            pmundo->Add(iter.Value()->Apply());
            iter.Next();
        }

        return pmundo;
    }
};

TRef<MultiUndo> MultiUndo::Create()
{
    return new MultiUndoImpl();
}
