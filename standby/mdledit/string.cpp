//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class StringItem : public IObject {
private:
    TRef<StringItem> m_pnext;
    ZString          m_str;

public:
    StringItem(const ZString& str, StringItem* pnext) :
        m_str(str),
        m_pnext(pnext)
    {
    }

    StringItem* GetNext()
    {
        return m_pnext;
    }

    const ZString& GetString()
    {
        return m_str;
    }
};

class StringList : public List {
private:
    TRef<EventSourceImpl> m_peventSource;
    TRef<StringItem>      m_pitem;
    int                   m_count;

public:
    StringList() :
        m_count(0)
    {
        m_peventSource = new EventSourceImpl();
    }

    void AddItem(const ZString& str)
    {
        m_count++;
        m_pitem = new StringItem(str, m_pitem);

        m_peventSource->Trigger();
    }

    //
    // IList
    //

    int GetCount()
    {
        return m_count;
    }

    ItemID GetItem(int index)
    {
        ZAssert(index >= 0);

        if (index < m_count) {
            StringItem* pitem = m_pitem;

            while (index > 0) {
                pitem = pitem->GetNext();
                index--;
            }

            return pitem;
        } else {
            return NULL;
        }
    }

    int GetIndex(ItemID pitemFind)
    {
        StringItem* pitem = m_pitem;
        int index = 0;

        while (pitem != NULL) {
            if ((ItemID)pitem == pitemFind) {
                return index;
            }
            pitem = pitem->GetNext();
            index++;
        }

        return -1;
    }

    ItemID GetNext(ItemID pitem)
    {
        return ((StringItem*)pitem)->GetNext();
    }

    IEventSource* GetChangedEvent()
    {
        return m_peventSource;
    }
};

class StringItemPainter : public ItemPainter {
public:
    int GetXSize()
    {
        return 64;
    }

    int GetYSize()
    {
        return 12;
    }

    void Paint(ItemID pitemArg, Surface* psurface, bool bSelected, bool bFocus)
    {
        StringItem* pitem = (StringItem*)pitemArg;

        if (bSelected) {
            psurface->FillRect(
                WinRect(0, 0, 64, 10),
                Color(0, 0, 0.5f)
            );
        }

        psurface->DrawString(
            WinPoint(0, 0),
            pitem->GetString()
        );
    }
};
