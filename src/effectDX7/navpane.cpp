#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// NavPane
//
//////////////////////////////////////////////////////////////////////////////

class NavPaneImpl : public NavPane {
private:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Types
    //
    //////////////////////////////////////////////////////////////////////////////

    class Topic : public Pane {
    public:
        TRef<Topic>                 m_ptopicParent;
        TRef<Pane>                  m_ppane;
        TRef<ModifiableString>      m_pstring;
        TRef<ModifiableColorValue>  m_pcolor;
        ZString                     m_str;
        ZString                     m_strMain;
        ZString                     m_strSecondary;
		ZString						m_strStartSub;
        int                         m_indent;
        Color                       m_color;
        Color                       m_colorHighlight;
        bool                        m_bInside;
        TRef<StringEventSourceImpl> m_peventSource;

        Topic(
            StringEventSourceImpl* peventSource,
            Topic*                 ptopicParent,
            IEngineFont*           pfont,
            int                    indent,
            int                    width,
            const ZString&         str,
            const ZString&         strMain,
            const ZString&         strSecondary,
            const Color&           colorHighlight
        ) :
            m_peventSource(peventSource),
            m_ptopicParent(ptopicParent),
            m_str(str),
            m_strMain(strMain),
            m_strSecondary(strSecondary),
            m_indent(indent * 8),
            m_colorHighlight(colorHighlight),
            m_bInside(false),
			m_strStartSub(ZString("hlpStartSub"))
        {
            m_ppane =
                new AnimatedImagePane(
                    CreateStringImage(
                        JustifyLeft(),
                        pfont,
                        m_pcolor = new ModifiableColorValue(Color::White()),
                        width - m_indent,
                        m_pstring = new ModifiableString(str),
                        4
                    )
                );

            InternalSetOffset(m_ppane, WinPoint(m_indent, 0));
            InsertAtBottom(m_ppane);
        }

        void SetColor(const Color& color)
        {
            m_color = color;
            if (!m_bInside) {
                m_pcolor->SetValue(color);
            }
        }

        Pane* GetPane()
        {
            return this;
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // Pane methods
        //
        //////////////////////////////////////////////////////////////////////////////

        void UpdateLayout()
        {
            Child()->UpdateLayout();
            InternalSetSize(WinPoint(
                Child()->XSize() + m_indent,
                Child()->YSize()
            ));
        }

        //////////////////////////////////////////////////////////////////////////////
        //
        // IMouseInput methods
        //
        //////////////////////////////////////////////////////////////////////////////

        void MouseEnter(IInputProvider* pprovider, const Point& point)
        {
            m_bInside = true;
            m_pcolor->SetValue(m_colorHighlight);
        }

        void MouseLeave(IInputProvider* pprovider)
        {
            m_bInside = false;
            m_pcolor->SetValue(m_color);
        }

        MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
        {
            if (button == 0 && bDown) {
                m_peventSource->Trigger(m_strMain);
            }

            return MouseResult();
        }
    };

    typedef TList<TRef<Topic> > TopicList;

    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    TopicList                   m_listTopic;
    TRef<StringEventSourceImpl> m_peventSource;
    TRef<IEngineFont>           m_pfont;
    TRef<ColumnPane>            m_pcolumn;
    TRef<Topic>                 m_ptopicSelected;
    Color                       m_color;
    Color                       m_colorSelected;
    Color                       m_colorHighlight;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    NavPaneImpl(
        IObjectList* plist,
        const Point& size,
        const Color& color,
        const Color& colorSelected,
        const Color& colorHighlight,
        IEngineFont* pfont
    ) :
        m_color(color),
        m_colorSelected(colorSelected),
        m_colorHighlight(colorHighlight),
        m_pfont(pfont)
    {
        m_peventSource = new StringEventSourceImpl();

        InternalSetSize(WinPoint::Cast(size));
        m_pcolumn = new ColumnPane();
        InsertAtBottom(m_pcolumn);

        ReadTopicList(plist, 0, NULL);
        m_ptopicSelected = m_listTopic.GetFront();
        UpdatePanes();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Implementation methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void ReadTopicList(IObjectList* plist, int indent, Topic* ptopicParent)
    {
        plist->GetFirst();
        while (plist->GetCurrent() != NULL) {
            TRef<IObjectPair> ppair; CastTo(ppair, plist->GetCurrent());

            TRef<StringValue> pstring;          CastTo(pstring,          ppair->GetNth(0));
            TRef<StringValue> pstringMain;      CastTo(pstringMain,      ppair->GetNth(1));
            TRef<StringValue> pstringSecondary; CastTo(pstringSecondary, ppair->GetNth(2));
            TRef<IObjectList> plistSubTopic;    CastTo(plistSubTopic,    ppair->GetLastNth(3));

            TRef<Topic> ptopic =
                new Topic(
                    m_peventSource,
                    ptopicParent,
                    m_pfont,
                    indent,
                    XSize(),
                    pstring->GetValue(),
                    pstringMain->GetValue(),
                    pstringSecondary->GetValue(),
                    m_colorHighlight
                );

            m_listTopic.PushEnd(ptopic);

            if (plistSubTopic->GetCount() != 0) {
                ReadTopicList(plistSubTopic, indent + 1, ptopic);
            }

            plist->GetNext();
        }
    }

    bool IsTopicSelected(Topic* ptopicParent)
    {
        TRef<Topic> ptopic = m_ptopicSelected;

        while (ptopic != NULL) {
            if (ptopic == ptopicParent) {
                return true;
            }

            ptopic = ptopic->m_ptopicParent;
        }

        return false;
    }

    bool IsTopicOpen(Topic* ptopic)
    {
        Topic* ptopicParent = ptopic->m_ptopicParent;

        //
        // Top level topics are always open
        //

        if (ptopicParent == NULL) {
            return true;
        }

        //
        // If all of this topic parents are selected this topic is open
        //

        while (ptopicParent != NULL) {
            if (!IsTopicSelected(ptopicParent)) {
                return false;
            }
            ptopicParent = ptopicParent->m_ptopicParent;
        }

        return true;
    }

    void UpdatePanes()
    {
        m_pcolumn->RemoveAllChildren();

        TopicList::Iterator iter(m_listTopic);

        while (!iter.End()) {
            Topic* ptopic = iter.Value();

            if (IsTopicOpen(ptopic)) {
                ptopic->SetColor(
                      IsTopicSelected(ptopic)
                    ? m_colorSelected
                    : m_color
                );

                m_pcolumn->InsertAtBottom(ptopic->GetPane());
            }

            iter.Next();
        }
    }

    Topic* FindTopic(const ZString& str)
    {
        TopicList::Iterator iter(m_listTopic);

        while (!iter.End()) {
            Topic* ptopic = iter.Value();

            if (ptopic->m_strMain == str) {
                return ptopic;
            }

            iter.Next();
        }

        return NULL;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // NavPane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetTopic(const ZString& str)
    {
        m_ptopicSelected = FindTopic(str);
        UpdatePanes();
    }

    const ZString& GetSecondary(const ZString& str)
    {
        Topic* ptopic = FindTopic(str);

        if (ptopic) {
            return ptopic->m_strSecondary;
		} else if (str=="hlpStart") {
			ptopic = FindTopic("hlpa0");
			if (ptopic) {
				return ptopic->m_strStartSub;
			} else {
				return str;
			}
		}
		else {
            return str;
        }
    }

    virtual TRef<IStringEventSource> GetEventSource()
    {
        return m_peventSource;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pane methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateLayout()
    {
        m_pcolumn->UpdateLayout();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Factories
//
//////////////////////////////////////////////////////////////////////////////

class NavPaneFactory : public IFunction {
public:
    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<IObjectList> plist;           CastTo(plist,           (IObject*)stack.Pop());
        TRef<PointValue>  psize;           CastTo(psize,           (IObject*)stack.Pop());
        TRef<ColorValue>  pcolor;          CastTo(pcolor,          (IObject*)stack.Pop());
        TRef<ColorValue>  pcolorSelected;  CastTo(pcolorSelected,  (IObject*)stack.Pop());
        TRef<ColorValue>  pcolorHighlight; CastTo(pcolorHighlight, (IObject*)stack.Pop());
        TRef<FontValue>   pfont;           CastTo(pfont,           (IObject*)stack.Pop());

        return
            new NavPaneImpl(
                plist,
                psize->GetValue(),
                pcolor->GetValue(),
                pcolorSelected->GetValue(),
                pcolorHighlight->GetValue(),
                pfont->GetValue()
            );
    }
};

void AddNavPaneFactory(
    INameSpace* pns
) {
    pns->AddMember("NavPane",  new NavPaneFactory());
}
