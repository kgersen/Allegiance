//////////////////////////////////////////////////////////////////////////////
//
// Editor
//
//////////////////////////////////////////////////////////////////////////////

#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// The main entry point
//
//////////////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////////////
//
// Forward Declarations
//
//////////////////////////////////////////////////////////////////////////////

class Item;

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class Font : public IObject {
private:
    HFONT    m_hfont;
    WinPoint m_size;

public:
    Font(HDC hdc, const ZString& strFace, int height)
    {
        m_hfont =
            ::CreateFont(
                11, //height,
                0,
                0,
                0,
                FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN,
                strFace
            );
        //
        //  , this assumes this is a fixed pitch font
        //

        HFONT hfontOld;
        SIZE size;

        ZVerify(hfontOld = (HFONT)::SelectObject(hdc, m_hfont));
        ZVerify(::GetTextExtentPoint(hdc, "a", 1, &size));
        ZVerify(::SelectObject(hdc, hfontOld));

        m_size = WinPoint(size.cx, size.cy);
    }

    HFONT GetHFONT()
    {
        return m_hfont;
    }

    WinPoint GetStringExtent(const ZString& str)
    {
        return WinPoint(m_size.X() * str.GetLength(), m_size.Y());
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class Context2D : public IObject {
private:
    TRef<Font> m_pfont;
    HDC        m_hdc;
    HFONT      m_hfontOld;
    WinPoint   m_pointOrigin;

public:
    Context2D(HDC hdc, Font* pfont) :
        m_hdc(hdc),
        m_pfont(pfont),
        m_pointOrigin(0, 0)
    {
        ZVerify(m_hfontOld = (HFONT)::SelectObject(m_hdc, m_pfont->GetHFONT()));
    }

    ~Context2D()
    {
        ZVerify(::SelectObject(m_hdc, m_hfontOld));
    }

    void SetOrigin(const WinPoint& point)
    {
        m_pointOrigin = point;
    }

    void SetBackColor(COLORREF color)
    {
        ZVerify(::SetBkColor(m_hdc, color) != CLR_INVALID);
    }

    void SetTextColor(COLORREF color)
    {
        ZVerify(::SetTextColor(m_hdc, color) != CLR_INVALID);
    }

    void SetFont(Font* pfont)
    {
        if (m_pfont != pfont) {
            m_pfont = pfont;
            ZVerify(::SelectObject(m_hdc, m_pfont->GetHFONT()));
        }
    }

    void DrawString(const ZString& str, const WinPoint& point = WinPoint(0, 0))
    {
        ZVerify(::TextOut(
            m_hdc,
            m_pointOrigin.X() + point.X(),
            m_pointOrigin.Y() + point.Y(),
            str,
            str.GetLength()
        ));
    }

    void FillRect(const WinRect& rect, COLORREF color)
    {
        ::FillSolidRect(
            m_hdc, 
            m_pointOrigin.X() + rect.XMin(), 
            m_pointOrigin.Y() + rect.YMin(), 
            rect.XSize(), 
            rect.YSize(), 
            color
        );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class Item : public IObject {
private:
    ZString    m_str;
    TRef<Item> m_pnext;
    Item*      m_pprev;

public:
    Item(const ZString& str = ZString()) :
        m_pprev(NULL)
    {
        int index;

        index = str.Find(0x0d);

        if (index == -1) {
            m_str = str;
        } else {
            m_str = str.Left(index);
            SetNext(new Item(str.RightOf(index + 2)));
        }
    }

    TRef<Item> Copy()
    {
        TRef<Item> pitem = new Item(m_str);

        if (m_pnext) {
            pitem->SetNext(m_pnext->Copy());
        }

        return pitem;
    }

    void SetNext(Item* pnext)
    {
        TRef<Item> pitemSave = m_pnext;

        if (m_pnext != NULL) {
            m_pnext->m_pprev = NULL;
        }
        m_pnext = pnext;

        if (pnext != NULL) {
            if (pnext->m_pprev != NULL) {
                pnext->m_pprev->m_pnext = NULL;
            }

            pnext->m_pprev = this;
        }
    }

    void SetString(const ZString& str)
    {
        m_str = str;
    }

    const ZString& GetString()
    {
        return m_str;
    }

    int GetLength()
    {
        return m_str.GetLength();
    }

    Item* GetNext()
    {
        return m_pnext;
    }

    Item* GetPrev()
    {
        return m_pprev;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class Marker : public IObject {
public:
    typedef TList<Marker*> List;

private:
    TRef<Item>      m_pitem;
    int             m_index;
    List*           m_plistMarkers;

public:
    Marker(List* plistMarkers, Item* pitem, int index) :
        m_plistMarkers(plistMarkers),
        m_pitem(pitem),
        m_index(index)
    {
        ZAssert(index >= 0);

        if (m_plistMarkers != NULL) {
            m_plistMarkers->PushFront(this);
        }
    }

    Marker(const Marker& marker) :
        m_plistMarkers(marker.m_plistMarkers),
        m_pitem(marker.m_pitem),
        m_index(marker.m_index)
    {
        if (m_plistMarkers != NULL) {
            m_plistMarkers->PushFront(this);
        }
    }

    ~Marker()
    {
        if (m_plistMarkers != NULL) {
            m_plistMarkers->Remove(this);
        }
    }

    Item* GetItem()
    {
        return m_pitem;
    }

    int GetIndex()
    {
        return m_index;
    }

    void SetItem(Item* pitem)
    {
        m_pitem = pitem;
    }

    void SetIndex(int index)
    {
        ZAssert(index >= 0);

        m_index = index;
    }

    void Set(Item* pitem, int index)
    {
        ZAssert(index >= 0);

        m_pitem = pitem;
        m_index = index;
    }

    void Set(const Marker& marker)
    {
        m_pitem = marker.m_pitem;
        m_index = marker.m_index;
    }

    friend bool operator<(const Marker& markerStart, const Marker& markerEnd)
    {
        Item* pitemStart = markerStart.m_pitem;
        Item* pitemEnd   = markerEnd.m_pitem;

        if (pitemStart == pitemEnd) {
            return markerStart.m_index < markerEnd.m_index;
        }

        while (pitemStart != NULL) {
            if (pitemStart == pitemEnd) {
                return true;
            }
            pitemStart = pitemStart->GetNext();
        }

        return false;
    }

    friend bool operator>=(const Marker& markerStart, const Marker& markerEnd)
    {
        return !(markerStart < markerEnd);
    }

    friend bool operator==(const Marker& marker1, const Marker& marker2)
    {
        return marker1.m_pitem == marker2.m_pitem && marker1.m_index == marker2.m_index;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class Document : public IObject {
private:
    //
    // Replace Undo
    //

    class ReplaceUndo : public IUndo {
        Document*    m_pdocument;
        TRef<Marker> m_pmarkerStart;
        TRef<Marker> m_pmarkerEnd;
        TRef<Item>   m_pitemReplace;

    public:
        ReplaceUndo(
            Document* pdocument,
            Marker* pmarkerStart,
            Marker* pmarkerEnd,
            Item* pitemReplace
        ) :
            m_pdocument(pdocument),
            m_pmarkerStart(pmarkerStart),
            m_pmarkerEnd(pmarkerEnd),
            m_pitemReplace(pitemReplace)
        {
        }

        TRef<IUndo> Apply()
        {
            m_pdocument->SetSelection(m_pmarkerStart, m_pmarkerEnd);
            return m_pdocument->ReplaceSelection(m_pitemReplace);
        }

        void AddCharacter()
        {
            ZAssert(m_pmarkerStart->GetItem() == m_pmarkerEnd->GetItem());

            m_pmarkerEnd->SetIndex(m_pmarkerEnd->GetIndex() + 1);
        }

        void RemoveCharacter(char ch)
        {
            ZAssert(m_pmarkerStart->GetItem() == m_pmarkerEnd->GetItem());

            if (m_pmarkerEnd->GetIndex() > m_pmarkerStart->GetIndex()) {
                m_pmarkerEnd->SetIndex(m_pmarkerEnd->GetIndex() - 1);
            } else {
                TRef<Item> pitem = new Item(ZString(&ch, 1));

                if (m_pitemReplace) {
                    m_pdocument->InsertAtEnd(pitem, m_pitemReplace);
                }

                m_pitemReplace = pitem;

                m_pmarkerStart->SetIndex(m_pmarkerStart->GetIndex() - 1);
                m_pmarkerEnd->SetIndex(m_pmarkerEnd->GetIndex() - 1);
            }
        }
    };

    //
    // Members
    //

    Marker::List  m_listMarkers;

    TRef<Item>    m_pitemHead;
    TRef<Marker>  m_pmarkerStart;
    TRef<Marker>  m_pmarkerEnd;
    TRef<Marker>  m_pmarkerCurrent;
    TRef<Marker>  m_pmarkerTop;

    ZString       m_strFind;
    ZString       m_strReplace;

    TRef<Item>    m_pitemClipboard;

    TList<TRef<IUndo> > m_listUndos;
    TList<TRef<IUndo> > m_listRedos;
    TRef<ReplaceUndo>   m_pcharacterUndo;

    ZString       m_strFilename;
    bool          m_bInsert;

    TRef<Font>    m_pfont;
    WinPoint      m_sizeChar;
    int           m_pageHeight;
    int           m_pageWidth;

    COLORREF      m_colorBack;
    COLORREF      m_colorText;
    COLORREF      m_colorInfoBack;
    COLORREF      m_colorInfoText;
    COLORREF      m_colorSelectionBack;
    COLORREF      m_colorSelectionText;
    COLORREF      m_colorCurrent;

public:
    Document(Font* pfont, const ZString strFilename, ZFile* pfile) :
        m_pfont(pfont),
        m_strFilename(strFilename),
        m_colorBack(RGB(192, 192, 192)),
        m_colorText(RGB(0, 0, 0)),
        m_colorSelectionBack(RGB(0, 0, 128)),
        m_colorSelectionText(RGB(255, 255, 255)),
        m_colorInfoBack(RGB(0, 0, 128)),
        m_colorInfoText(RGB(255, 255, 255)),
        m_pageHeight(0),
        m_pageWidth(0),
        m_bInsert(true)
    {
        m_sizeChar = m_pfont->GetStringExtent("a");

        m_pitemHead  = new Item();

        if (pfile) {
            m_pitemHead->SetNext(
                new Item(ZString((PCC)pfile->GetPointer(), pfile->Length()))
            );
        } else {
            m_pitemHead->SetNext(new Item());
        }

        m_pmarkerStart   = CreateMarker(GetFirstItem(), 0);
        m_pmarkerEnd     = CreateMarker(GetFirstItem(), 0);
        m_pmarkerTop     = CreateMarker(GetFirstItem(), 0);
        m_pmarkerCurrent = m_pmarkerEnd;
    }

    Item* GetHeadItem()
    {
        return m_pitemHead;
    }

    Item* GetFirstItem()
    {
        return m_pitemHead->GetNext();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Page size
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetSize(const WinPoint& size)
    {
        m_pageWidth  = (size.X() + m_sizeChar.X() - 1) / m_sizeChar.X();
        m_pageHeight = ((size.Y() + m_sizeChar.Y() - 1) / m_sizeChar.Y()) - 2;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Markers
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Marker> CreateMarker(Item* pitem, int index)
    {
        return new Marker(&m_listMarkers, pitem, index);
    }

    void RemoveMarker(Marker* pmarker)
    {
        m_listMarkers.Remove(pmarker);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Undo
    //
    //////////////////////////////////////////////////////////////////////////////

    void AddUndo(IUndo* pundo)
    {
        if (pundo != m_pcharacterUndo) {
            m_pcharacterUndo = NULL;
        }

        m_listUndos.PushFront(pundo);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Join two items
    //
    //////////////////////////////////////////////////////////////////////////////

    void JoinItems(Item* pitem, int index)
    {
        Item* pitemNext = pitem->GetNext();

        if (pitemNext) {
            int lengthOld = pitem->GetLength();

            if (index >= lengthOld) {
                pitem->SetString(pitem->GetString() + pitemNext->GetString());
            } else {
                const ZString& str = pitem->GetString();

                int indexSearch;
                for (indexSearch = lengthOld - 1; indexSearch >= index; indexSearch--) {
                    if (str[indexSearch] != ' ') 
                        break;
                }

                pitem->SetString(pitem->GetString().Left(indexSearch + 1) + pitemNext->GetString());
            }

            pitem->SetNext(pitemNext->GetNext());

            //
            // Fix up the markers
            //

            Marker::List::Iterator iter(m_listMarkers);

            while (!iter.End()) {
                Marker* pmarker = iter.Value();

                if (pmarker->GetItem() == pitemNext) {
                    pmarker->Set(pitem, lengthOld + pmarker->GetIndex());
                }

                iter.Next();
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Split an item
    //
    //////////////////////////////////////////////////////////////////////////////

    void SplitItem(Marker* pmarker)
    {
        Item* pitem = pmarker->GetItem();
        int   index = pmarker->GetIndex();

        //
        // Split the item into two items
        //

        TRef<Item> pitemNext = new Item(pitem->GetString().RightOf(index));
        pitem->SetString(pitem->GetString().Left(index));

        pitemNext->SetNext(pitem->GetNext());
        pitem->SetNext(pitemNext);

        //
        // Fix up the markers
        //

        Marker::List::Iterator iter(m_listMarkers);

        while (!iter.End()) {
            Marker* pmarker = iter.Value();

            if (pmarker->GetItem() == pitem && pmarker->GetIndex() >= index) {
                pmarker->Set(pitemNext, pmarker->GetIndex() - index);
            }

            iter.Next();
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Delete a range of items
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Item> Delete(Marker* pmarkerStart, Marker* pmarkerEnd)
    {
        //
        // Don't delete anything if the two markers point to the same place
        //

        bool bJoin = false;

        if (pmarkerStart->GetItem() == pmarkerEnd->GetItem()) {
            if (pmarkerStart->GetIndex() == pmarkerEnd->GetIndex()) {
                return NULL;
            }
            bJoin = (pmarkerStart->GetIndex() != 0);
        }

        //
        // Make sure the markers point to the beginning of two items
        //

        if (pmarkerStart->GetIndex() != 0) {
            SplitItem(pmarkerStart);
            ZAssert(pmarkerStart->GetIndex() == 0);
        }

        if (pmarkerEnd->GetIndex() != 0) {
            SplitItem(pmarkerEnd);
            ZAssert(pmarkerEnd->GetIndex() == 0);
        }

        //
        // Cut out the deleted section
        //

        TRef<Item> pitemDelete = pmarkerStart->GetItem();
        TRef<Item> pitemPrev   = pitemDelete->GetPrev();

        pitemPrev->SetNext(pmarkerEnd->GetItem());

        //
        // Fix up markers
        //

        Marker::List::Iterator iter(m_listMarkers);

        while (!iter.End()) {
            Marker* pmarker = iter.Value();

            Item* pitem = pitemDelete;

            while (pitem != NULL) {
                if (pmarker->GetItem() == pitem) {
                    pmarker->Set(pmarkerEnd->GetItem(), 0);
                }
                pitem = pitem->GetNext();
            }

            iter.Next();
        }

        //
        // Join the two items that were next to the range together
        //

        if (bJoin) {
            JoinItems(pitemPrev, pitemPrev->GetLength());
        }

        //
        // Return the deleted section
        //

        return pitemDelete;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Insert a range of items at the end
    //
    //////////////////////////////////////////////////////////////////////////////

    void InsertAtEnd(Item* pitem, Item* pitemInsert)
    {
        if (pitem->GetNext()) {
            InsertAtEnd(pitem->GetNext(), pitemInsert);
        } else {
            //
            // Fix up markers
            //

            Marker::List::Iterator iter(m_listMarkers);

            while (!iter.End()) {
                Marker* pmarker = iter.Value();

                if (
                       pmarker->GetItem()  == pitem 
                    && pmarker->GetIndex() == pitem->GetLength()
                ) {
                    pmarker->Set(pitemInsert, 0);
                }

                iter.Next();
            }

            //
            // join them together
            //

            pitem->SetNext(pitemInsert);
            JoinItems(pitem, pitem->GetLength());
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Insert a range of items
    //
    //////////////////////////////////////////////////////////////////////////////

    void Insert(Marker* pmarker, Item* pitemInsert)
    {
        Item* pitem = pmarker->GetItem();
        int index = pmarker->GetIndex();

        if (index == 0) {
            TRef<Item> pprev = pitem->GetPrev();
            InsertAtEnd(pitemInsert, pitem);
            pprev->SetNext(pitemInsert);
        } else {
            TRef<Marker> pmarkerTemp = CreateMarker(pitem, pitem->GetLength());

            if (index >= pitem->GetLength()) {
                pmarker = pmarkerTemp;
            }    

            SplitItem(pmarker);
            pitem = pmarker->GetItem();
            TRef<Item> pprev = pitem->GetPrev();
            InsertAtEnd(pitemInsert, pitem);
            InsertAtEnd(pprev, pitemInsert);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Replace the selection with a range of Items
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<ReplaceUndo> ReplaceSelection(Item* pitemInsert)
    {
        TRef<Item> pitemReplace = Delete(m_pmarkerStart, m_pmarkerEnd);

        TRef<Marker> pmarkerOld = new Marker(NULL, m_pmarkerStart->GetItem(), m_pmarkerStart->GetIndex());

        if (pitemInsert != NULL) {
            Insert(m_pmarkerStart, pitemInsert);
        }

        return
            new ReplaceUndo(
                this,
                pmarkerOld,
                new Marker(NULL, m_pmarkerStart->GetItem(), m_pmarkerStart->GetIndex()),
                pitemReplace
            );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Insert a file
    //
    //////////////////////////////////////////////////////////////////////////////

    HRESULT Insert(ZFile* pfile)
    {
        TRef<Item> pitem =
            new Item(ZString((PCC)pfile->GetPointer(), pfile->Length()));

        AddUndo(ReplaceSelection(pitem));

        return S_OK;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Render the document
    //
    //////////////////////////////////////////////////////////////////////////////

    void RenderItem(
        Context2D* pcontext, 
        Item* pitem,
        int indexStart,
        int indexEnd
    ) {
        int indexSplit = pitem->GetLength();

        if (indexSplit > indexEnd) {
            indexSplit = indexEnd;
        }

        if (indexSplit < indexStart) {
            indexSplit = indexStart;
        }

        if (indexStart < indexSplit) {
            pcontext->DrawString(
                pitem->GetString().Middle(indexStart, indexSplit - indexStart)
            );
        }

        if (indexSplit < indexEnd) {
            pcontext->FillRect(
                WinRect(
                    (indexSplit - indexStart) * m_sizeChar.X(),
                    0,
                    (indexEnd - indexStart) * m_sizeChar.X(),
                    m_sizeChar.Y()
                ),
                m_colorCurrent
            );
        }
    }

    void Render(HDC hdc, const WinRect& rect)
    {
        TRef<Context2D> pcontext = new Context2D(hdc, m_pfont);

        //
        // Draw the text
        //

        pcontext->SetBackColor(m_colorBack);
        pcontext->SetTextColor(m_colorText);
        m_colorCurrent = m_colorBack;

        Item* pitem      = m_pmarkerTop->GetItem();
        int   indexFirst = m_pmarkerTop->GetIndex();
        int   yStart     = -1;

        for (int line = 0; line < m_pageHeight; line++) {
            int yLine = line * m_sizeChar.Y();

            if (pitem != NULL) {
                int indexStart = indexFirst;

                if (m_pmarkerStart->GetItem() == pitem) {
                    yStart = yLine;

                    indexStart = max(indexStart, m_pmarkerStart->GetIndex());

                    pcontext->SetOrigin(WinPoint(0, yLine));
                    RenderItem(pcontext, pitem, indexFirst, indexStart);

                    pcontext->SetBackColor(m_colorSelectionBack);
                    pcontext->SetTextColor(m_colorSelectionText);
                    m_colorCurrent = m_colorSelectionBack;
                }

                if (m_pmarkerEnd->GetItem() == pitem) {
                    int indexEnd = min(m_pageWidth + indexFirst, m_pmarkerEnd->GetIndex());

                    pcontext->SetOrigin(
                        WinPoint(
                            (indexStart - indexFirst) * m_sizeChar.X(),
                            yLine
                        )
                    );
                    RenderItem(pcontext, pitem, indexStart, indexEnd);

                    indexStart = indexEnd;
                    pcontext->SetBackColor(m_colorBack);
                    pcontext->SetTextColor(m_colorText);
                    m_colorCurrent = m_colorBack;
                }

                pcontext->SetOrigin(
                    WinPoint(
                        (indexStart - indexFirst) * m_sizeChar.X(),
                        yLine
                    )
                );
                RenderItem(pcontext, pitem, indexStart, m_pageWidth + indexFirst);

                pitem = pitem->GetNext();
            } else {
                pcontext->SetOrigin(WinPoint(0, yLine));
                pcontext->FillRect(
                    WinRect(
                        0,
                        0,
                        m_pageWidth * m_sizeChar.X(),
                        m_sizeChar.Y()
                    ),
                    m_colorBack
                );
            }

            yLine += m_sizeChar.Y();
        }
        pcontext->SetOrigin(WinPoint(0, 0));

        //
        // Calculate the selection line
        //

        int lines   = 0;
        int selline = 1;

        pitem = GetFirstItem();
        while (pitem != m_pmarkerCurrent->GetItem()) {
            selline++;
            lines++;
            pitem = pitem->GetNext();
        }

        while (pitem != NULL) {
            lines++;
            pitem = pitem->GetNext();
        }

        //
        // Draw the Information line
        //

        ZString strInfo = m_strFilename;

        if (!m_listUndos.IsEmpty()) {
            strInfo += "*";
        }

        strInfo += " Length=" + ZString(lines);
        strInfo += " Cursor=(" + ZString(m_pmarkerCurrent->GetIndex() + 1) + ", " + ZString(selline) + ")";

        if (m_bInsert) {
            strInfo += " insert";
        }

        pcontext->SetBackColor(m_colorInfoBack);
        pcontext->SetTextColor(m_colorInfoText);
        pcontext->DrawString(strInfo, WinPoint(0, m_pageHeight * m_sizeChar.Y()));
        pcontext->FillRect(
            WinRect(
                strInfo.GetLength() * m_sizeChar.X(),
                m_pageHeight * m_sizeChar.Y(),
                m_pageWidth * m_sizeChar.X(),
                (m_pageHeight + 1) * m_sizeChar.Y()
            ),
            m_colorInfoBack
        );

        //
        // Draw the Cursor
        //

        if (IsCursor() && yStart != -1) {
            int index = m_pmarkerStart->GetIndex();
            pcontext->FillRect(
                WinRect(
                    index * m_sizeChar.X(),
                    yStart,
                    index * m_sizeChar.X() + 3,
                    yStart + m_sizeChar.Y()
                 ), 
                 m_colorSelectionBack
            );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Commands
    //
    //////////////////////////////////////////////////////////////////////////////

    bool IsCursor()
    {
        return *m_pmarkerStart == *m_pmarkerEnd;
    }

    void SetSelection(Marker* pmarkerStart, Marker* pmarkerEnd)
    {
        m_pmarkerStart->Set(pmarkerStart->GetItem(), pmarkerStart->GetIndex());
        m_pmarkerEnd->Set(pmarkerEnd->GetItem(), pmarkerEnd->GetIndex());
    }

    void SetCursorPosition(Item* pitem, int index, bool bSelect)
    {
        m_pcharacterUndo = NULL;

        TRef<Marker> pmarker = CreateMarker(pitem, index);

        if (bSelect) {
            if (m_pmarkerCurrent == m_pmarkerEnd) {
                if (*pmarker < *m_pmarkerStart) {
                    m_pmarkerEnd->Set(*m_pmarkerStart);
                    m_pmarkerCurrent = m_pmarkerStart;
                }
            } else {
                if (*pmarker >= *m_pmarkerEnd) {
                    m_pmarkerStart->Set(*m_pmarkerEnd);
                    m_pmarkerCurrent = m_pmarkerEnd;
                }
            }
            m_pmarkerCurrent->Set(*pmarker);
        } else {
            m_pmarkerStart->Set(*pmarker);
            m_pmarkerEnd->Set(*pmarker);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Cursor Commands
    //
    //////////////////////////////////////////////////////////////////////////////

    void ScrollLeft()
    {
        if (m_pmarkerTop->GetIndex() > 0) {
            m_pmarkerTop->SetIndex(m_pmarkerTop->GetIndex() - 1);
        }
    }

    void ScrollRight()
    {
        m_pmarkerTop->SetIndex(m_pmarkerTop->GetIndex() + 1);
    }

    void ScrollUp()
    {
        if (m_pmarkerTop->GetItem() != GetFirstItem()) {
            m_pmarkerTop->SetItem(m_pmarkerTop->GetItem()->GetPrev());
        }
    }

    void ScrollDown()
    {
        if (m_pmarkerTop->GetItem()->GetNext() != NULL) {
            m_pmarkerTop->SetItem(m_pmarkerTop->GetItem()->GetNext());
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Cursor Commands
    //
    //////////////////////////////////////////////////////////////////////////////

    void Left(bool bSelect)
    {
        if (m_pmarkerCurrent->GetIndex() > 0) {
            SetCursorPosition(m_pmarkerCurrent->GetItem(), m_pmarkerCurrent->GetIndex() - 1, bSelect);
        }
    }

    void Right(bool bSelect)
    {
        SetCursorPosition(m_pmarkerCurrent->GetItem(), m_pmarkerCurrent->GetIndex() + 1, bSelect);
    }

    void Up(bool bSelect)
    {
        if (m_pmarkerCurrent->GetItem() != GetFirstItem()) {
            SetCursorPosition(m_pmarkerCurrent->GetItem()->GetPrev(), m_pmarkerCurrent->GetIndex(), bSelect);
        }
    }

    void Down(bool bSelect)
    {
        if (m_pmarkerCurrent->GetItem()->GetNext() != NULL) {
            SetCursorPosition(m_pmarkerCurrent->GetItem()->GetNext(), m_pmarkerCurrent->GetIndex(), bSelect);
        }
    }

    void PageUp(bool bSelect)
    {
        for(int index = 0; index < m_pageHeight; index++) {
            ScrollUp();
            Up(bSelect);
        }
    }

    void PageDown(bool bSelect)
    {
        for(int index = 0; index < m_pageHeight; index++) {
            ScrollDown();
            Down(bSelect);
        }
    }

    void GotoBeginingOfDocument(bool bSelect)
    {
        m_pmarkerTop->Set(GetFirstItem(), 0);
        SetCursorPosition(GetFirstItem(), 0, bSelect);
    }

    void GotoEndOfDocument(bool bSelect)
    {
        Item* pitem = GetFirstItem();
        while (pitem->GetNext() != NULL) {
            pitem = pitem->GetNext();
        }

        m_pmarkerTop->Set(pitem, 0);
        SetCursorPosition(pitem, 0, bSelect);
    }

    void GotoBeginingOfLine(bool bSelect)
    {
        const ZString& str = m_pmarkerCurrent->GetItem()->GetString();
        int length = str.GetLength();
        int index = 0;

        while (index < length && str[index] == ' ') {
            index++;
        }

        if (m_pmarkerCurrent->GetIndex() == index) {
            SetCursorPosition(m_pmarkerCurrent->GetItem(), 0, bSelect);
        } else {
            SetCursorPosition(m_pmarkerCurrent->GetItem(), index, bSelect);
        }
    }

    void GotoEndOfLine(bool bSelect)
    {
        SetCursorPosition(
            m_pmarkerCurrent->GetItem(),
            m_pmarkerCurrent->GetItem()->GetLength(),
            bSelect
        );
    }

    void Cancel()
    {
        if (m_pmarkerCurrent == m_pmarkerEnd) {
            SetCursorPosition(
                m_pmarkerStart->GetItem(),
                m_pmarkerStart->GetIndex(),
                false
            );
        } else {
            SetCursorPosition(
                m_pmarkerEnd->GetItem(),
                m_pmarkerEnd->GetIndex(),
                false
            );
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Commands
    //
    //////////////////////////////////////////////////////////////////////////////

    void Character(TCHAR ch)
    {
        TRef<Item> pitem = new Item(ZString(&ch, 1));

        if (!m_pcharacterUndo) {
            m_pcharacterUndo = ReplaceSelection(pitem);
            AddUndo(m_pcharacterUndo);
        } else {
            ReplaceSelection(pitem);
            m_pcharacterUndo->AddCharacter();
        }
    }

    void Split()
    {
        Delete(m_pmarkerStart, m_pmarkerEnd);
        SplitItem(m_pmarkerStart);
    }

    void Join()
    {
        JoinItems(m_pmarkerStart->GetItem(), 0);
    }

    void Tab()
    {
        if (IsCursor()) {
        } else {
        }
    }

    void BackTab()
    {
        if (IsCursor()) {
        } else {
        }
    }

    void Delete()
    {
        if (IsCursor()) {
            Item* pitem = m_pmarkerStart->GetItem();
            int   index = m_pmarkerStart->GetIndex();

            if (index < pitem->GetLength()) {
                TRef<Marker> pmarkerStart = CreateMarker(pitem, index);
                TRef<Marker> pmarkerEnd   = CreateMarker(pitem, index + 1);

                Delete(pmarkerStart, pmarkerEnd);
            } else {
                JoinItems(pitem, index);
            }
        } else {
            Delete(m_pmarkerStart, m_pmarkerEnd);
        }
    }

    void Backspace()
    {
        if (IsCursor()) {
            Item* pitem = m_pmarkerStart->GetItem();
            int   index = m_pmarkerStart->GetIndex();

            if (index == 0) {
                if (pitem != GetFirstItem()) {
                    JoinItems(pitem->GetPrev(), 0);
                }
            } else {
                m_pmarkerStart->SetIndex(index - 1);

                if (!m_pcharacterUndo) {
                    m_pcharacterUndo = ReplaceSelection(NULL);
                    AddUndo(m_pcharacterUndo);
                } else {
                    TRef<Item> pitem = Delete(m_pmarkerStart, m_pmarkerEnd);
                    m_pcharacterUndo->RemoveCharacter(pitem->GetString()[0]);
                }
            }
        } else {
            Delete();
        }
    }
    
    void AquireFindString()
    {
        if (m_pmarkerStart->GetItem() == m_pmarkerEnd->GetItem()) {
            m_strFind = 
                m_pmarkerStart->GetItem()->GetString().Middle(
                    m_pmarkerStart->GetIndex(),
                    m_pmarkerEnd->GetIndex() - m_pmarkerStart->GetIndex()
                );
        }
    }

    void FindNext()
    {
        if (!IsCursor()) {
            AquireFindString();
        }

        if (!m_strFind.IsEmpty()) {
            Item* pitem = m_pmarkerEnd->GetItem();
            int index   = m_pmarkerEnd->GetIndex();

            while (pitem != NULL) {
                int indexFind = pitem->GetString().RightOf(index).Find(m_strFind);

                if (indexFind != -1) {
                    m_pmarkerStart->Set(pitem, index + indexFind);
                    m_pmarkerEnd->Set(pitem, index + indexFind + m_strFind.GetLength());
                    return;
                }

                index = 0;
                pitem = pitem->GetNext();
            }
        }
    }

    void FindPrevious()
    {
        if (!IsCursor()) {
            AquireFindString();
        }

        if (!m_strFind.IsEmpty()) {
            Item* pitem = m_pmarkerStart->GetItem();
            int index   = m_pmarkerStart->GetIndex();

            while (pitem != GetHeadItem()) {
                int indexFind = pitem->GetString().Left(index).ReverseFind(m_strFind);

                if (indexFind != -1) {
                    m_pmarkerStart->Set(pitem, indexFind);
                    m_pmarkerEnd->Set(pitem, indexFind + m_strFind.GetLength());
                    return;
                }

                pitem = pitem->GetPrev();
                index = pitem->GetLength();
            }
        }
    }

    void Undo()
    {
        if (!m_listUndos.IsEmpty()) {
            m_pcharacterUndo = NULL;
            TRef<IUndo> pundo = m_listUndos.PopFront();
            m_listRedos.PushFront(pundo->Apply());
        }
    }

    void Redo()
    {
        if (!m_listRedos.IsEmpty()) {
            TRef<IUndo> pundo = m_listRedos.PopFront();
            m_listUndos.PushFront(pundo->Apply());
        }
    }

    void Cut()
    {
        if (!IsCursor()) {
            m_pitemClipboard = Delete(m_pmarkerStart, m_pmarkerEnd);
        }
    }

    void Copy()
    {
        if (!IsCursor()) {
            TRef<Item> pitem = Delete(m_pmarkerStart, m_pmarkerEnd);

            m_pitemClipboard = pitem->Copy();

            AddUndo(ReplaceSelection(pitem));
        }
    }

    void Paste()
    {
        if (m_pitemClipboard) {
            AddUndo(ReplaceSelection(m_pitemClipboard->Copy()));
        }
    }

    void ToggleInsert()
    {
        m_bInsert = !m_bInsert;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Editor Window
//
//////////////////////////////////////////////////////////////////////////////

class EditorWindow : public Window, public IKeyboardInput {
protected:
    TList<TRef<Document> > m_listDocuments;
    TRef<Document>        m_pdocumentCurrent;
    TRef<Font>            m_pfont;
    bool                  m_bControl;
    bool                  m_bShift;

public:
    EditorWindow() :
        Window(NULL, WinRect(0, 0, 640, 480), "Editor"),
        m_bControl(false),
        m_bShift(false)
    {
        SetKeyboardInput(IKeyboardInput::CreateDelegate(this));

        HDC hdc = GetDC();

        m_pfont = new Font(hdc, "Lucida Console", 11);

        ReleaseDC(hdc);
    }

    void RectChanged()
    {
        if (m_pdocumentCurrent != NULL) {
            m_pdocumentCurrent->SetSize(GetClientRect().Size());
        }

        Invalidate();
    }

    void NewDocument(const ZString& str, ZFile* pfile)
    {
        TRef<Document> pdocument = new Document(m_pfont, str, pfile);
        m_listDocuments.PushFront(pdocument);

        SetCurrentDocument(pdocument);
    }

    void SetCurrentDocument(Document* pdocument)
    {
        if (m_pdocumentCurrent != pdocument) {
            m_pdocumentCurrent = pdocument;
            m_pdocumentCurrent->SetSize(GetClientRect().Size());
            Invalidate();
        }
    }

    HRESULT Open(const ZString& str)
    {
        TRef<ZFile> pfile = new ZFile(str);

        if (pfile->IsValid()) {
            NewDocument(str, pfile);

            return S_OK;
        }

        return S_FALSE;
    }

    void Invalidate()
    {
        InvalidateRgn();
    }

    void OnPaint(HDC hdc, const WinRect& rect)
    {
        if (m_pdocumentCurrent) {
            m_pdocumentCurrent->Render(hdc, rect);
        } else {
            Window::OnPaint(hdc, rect);
        }
    }

    bool OnSysKey(IInputProvider* pprovider, int vk, bool bDown)
    {
        if (m_pdocumentCurrent) {
            if (bDown) {
                switch(vk) {
                    case VK_LEFT:
                        m_pdocumentCurrent->Left(true);
                        Invalidate();
                        return true;

                    case VK_RIGHT:
                        m_pdocumentCurrent->Right(true);
                        Invalidate();
                        return true;

                    case VK_UP:
                        m_pdocumentCurrent->Up(true);
                        Invalidate();
                        return true;

                    case VK_DOWN:
                        m_pdocumentCurrent->Down(true);
                        Invalidate();
                        return true;

                    case VK_PRIOR:
                        m_pdocumentCurrent->PageUp(true);
                        Invalidate();
                        return true;

                    case VK_NEXT:
                        m_pdocumentCurrent->PageDown(true);
                        Invalidate();
                        return true;

                    case VK_HOME:
                        m_pdocumentCurrent->GotoBeginingOfLine(true);
                        Invalidate();
                        return true;

                    case VK_END:
                        m_pdocumentCurrent->GotoEndOfLine(true);
                        Invalidate();
                        return true;
                }
            }
        }

        return false;
    }

    bool OnKey(IInputProvider* pprovider, int vk, bool bDown)
    {
        if (m_pdocumentCurrent) {
            switch (vk) {
                case VK_CONTROL:
                    m_bControl = bDown;
                    break;

                case VK_SHIFT:
                    m_bShift = bDown;
                    break;
            }

            if (m_bControl && m_bShift) {
                //
                // bShift && bControl
                //

                if (bDown) {
                    switch(vk) {
                        case VK_PRIOR:
                            m_pdocumentCurrent->GotoBeginingOfDocument(true);
                            Invalidate();
                            return true;

                        case VK_NEXT:
                            m_pdocumentCurrent->GotoEndOfDocument(true);
                            Invalidate();
                            return true;
                    }
                }
            } else if (m_bControl) {
                //
                // !bShift && bControl
                //

                if (bDown) {
                    switch(vk) {
                        case 'Z':
                            m_pdocumentCurrent->Undo();
                            Invalidate();
                            return true;

                        case 'Q':
                            m_pdocumentCurrent->Redo();
                            Invalidate();
                            return true;

                        case 'X':
                            m_pdocumentCurrent->Cut();
                            Invalidate();
                            return true;

                        case 'C':
                            m_pdocumentCurrent->Copy();
                            Invalidate();
                            return true;

                        case 'V':
                            m_pdocumentCurrent->Paste();
                            Invalidate();
                            return true;

                        case 'J':
                            m_pdocumentCurrent->Join();
                            Invalidate();
                            return true;

                        case VK_LEFT:
                            m_pdocumentCurrent->ScrollLeft();
                            Invalidate();
                            return true;

                        case VK_RIGHT:
                            m_pdocumentCurrent->ScrollRight();
                            Invalidate();
                            return true;

                        case VK_UP:
                            m_pdocumentCurrent->ScrollUp();
                            Invalidate();
                            return true;

                        case VK_DOWN:
                            m_pdocumentCurrent->ScrollDown();
                            Invalidate();
                            return true;

                        case VK_PRIOR:
                            m_pdocumentCurrent->GotoBeginingOfDocument(false);
                            Invalidate();
                            return true;

                        case VK_NEXT:
                            m_pdocumentCurrent->GotoEndOfDocument(false);
                            Invalidate();
                            return true;
                    }
                }
            } else if (m_bShift) {
                //
                // bShift && !bControl
                //

                if (bDown) {
                    switch(vk) {
                        case VK_TAB:
                            m_pdocumentCurrent->BackTab();
                            Invalidate();
                            return true;

                        case VK_LEFT:
                            m_pdocumentCurrent->Left(true);
                            Invalidate();
                            return true;

                        case VK_RIGHT:
                            m_pdocumentCurrent->Right(true);
                            Invalidate();
                            return true;

                        case VK_UP:
                            m_pdocumentCurrent->Up(true);
                            Invalidate();
                            return true;

                        case VK_DOWN:
                            m_pdocumentCurrent->Down(true);
                            Invalidate();
                            return true;

                        case VK_PRIOR:
                            m_pdocumentCurrent->PageUp(true);
                            Invalidate();
                            return true;

                        case VK_NEXT:
                            m_pdocumentCurrent->PageDown(true);
                            Invalidate();
                            return true;

                        case VK_HOME:
                            m_pdocumentCurrent->GotoBeginingOfLine(true);
                            Invalidate();
                            return true;

                        case VK_END:
                            m_pdocumentCurrent->GotoEndOfLine(true);
                            Invalidate();
                            return true;
                    }
                }
            } else {
                //
                // !bShift && !bControl
                //

                if (bDown) {
                    switch(vk) {
                        case VK_LEFT:
                            m_pdocumentCurrent->Left(false);
                            Invalidate();
                            return true;

                        case VK_RIGHT:
                            m_pdocumentCurrent->Right(false);
                            Invalidate();
                            return true;

                        case VK_UP:
                            m_pdocumentCurrent->Up(false);
                            Invalidate();
                            return true;

                        case VK_DOWN:
                            m_pdocumentCurrent->Down(false);
                            Invalidate();
                            return true;

                        case VK_PRIOR:
                            m_pdocumentCurrent->PageUp(false);
                            Invalidate();
                            return true;

                        case VK_NEXT:
                            m_pdocumentCurrent->PageDown(false);
                            Invalidate();
                            return true;

                        case VK_HOME:
                            m_pdocumentCurrent->GotoBeginingOfLine(false);
                            Invalidate();
                            return true;

                        case VK_END:
                            m_pdocumentCurrent->GotoEndOfLine(false);
                            Invalidate();
                            return true;

                        case VK_ESCAPE:
                            m_pdocumentCurrent->Cancel();
                            Invalidate();
                            return true;

                        case VK_BACK:
                            m_pdocumentCurrent->Backspace();
                            Invalidate();
                            return true;

                        case VK_DELETE:
                            m_pdocumentCurrent->Delete();
                            Invalidate();
                            return true;

                        case VK_RETURN:
                            m_pdocumentCurrent->Split();
                            Invalidate();
                            return true;

                        case VK_TAB:
                            m_pdocumentCurrent->Tab();
                            Invalidate();
                            return true;

                        case VK_INSERT:
                            m_pdocumentCurrent->ToggleInsert();
                            Invalidate();
                            return true;

                        case VK_F3:
                            m_pdocumentCurrent->FindNext();
                            Invalidate();
                            return true;

                        case VK_F4:
                            m_pdocumentCurrent->FindPrevious();
                            Invalidate();
                            return true;
                    }
                }
            }
        }

        return false;
    }

    bool OnChar(IInputProvider* pprovider, TCHAR ch)
    {
        if (m_pdocumentCurrent) {
            if (ch >= ' ' && ch <='~') {
                m_pdocumentCurrent->Character(ch);
                Invalidate();
                return true;
            }
        }

        return false;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Editor Application
//
//////////////////////////////////////////////////////////////////////////////

class EditApp : public Win32App {
protected:
    TRef<EditorWindow>    m_pwindow;

public:
    EditApp()
    {
    }

    void Error(const ZString& str)
    {
        m_pwindow->MessageBox(str, "Error", MB_ICONEXCLAMATION | MB_OK);
    }

    HRESULT Initialize(const ZString& strCommandLine)
    {
        m_pwindow = new EditorWindow();

        //
        // Parse the Command line
        //

        PCC pcc = strCommandLine;
        CommandLineToken token(pcc, strCommandLine.GetLength());

        while (token.MoreTokens()) {
            ZString str;

            if (token.Is(CommandLineToken::Minus, false)) {
                /*
                ZString str;

                if (token.IsSymbol(str, true)) {
                    if (str == "reduce") {
                        if (token.IsNumber(m_scale, true)) {
                            m_mode = ModeReduce;
                            continue;
                        }
                    } else if (str == "flatten") {
                        if (token.IsNumber(m_frame, true)) {
                            m_mode = ModeFlatten;
                            continue;
                        }
                    }
                }
                */
            } else if (token.IsPath(str, true)) {
                if (FAILED(m_pwindow->Open(str))) {
                    Error("Unable to open " + str + ".");
                }
                continue;
            }

            //
            // error occured
            //

            Error("Invalid command line.");
            break;
        }

        return S_OK;
    }

    HRESULT Terminate()
    {
        return S_OK;
    }
} g_app;
