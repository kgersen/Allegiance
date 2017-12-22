#include "pch.h"
#include "trekmdl.h"
#include "consoledata.h"

//////////////////////////////////////////////////////////////////////////////
//
// SelectModelPane
//
//////////////////////////////////////////////////////////////////////////////

class SelectModelPane : public ValuePane
{
private:
        TRef<ModelData> m_pmodelData;

public:
    SelectModelPane(Pane* ppaneChild, ModelData* pmodelData) :
        ValuePane(ppaneChild),
        m_pmodelData(pmodelData)
    {
    }

    void UpdateLayout()
    {
        if (Child()) {
            InternalSetExpand(
                Child(),
                WinPoint(
                    XExpand() == 0 ? 0 : XExpand(),
                    YExpand() == 0 ? 0 : YExpand()
                )
            );
            Child()->UpdateLayout();
            InternalSetSize(WinPoint(
                Child()->XSize(),
                Child()->YSize()
            ));
            InternalSetOffset(Child(), WinPoint(0, 0));
        } else {
            InternalSetSize(
                WinPoint(
                    std::max(0, XExpand()),
                    std::max(0, YExpand())
                )
            );
        }
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        return MouseResult();
    }

};

TRef<IObject> SelectModelPaneFactory::Apply(ObjectStack& stack)
{
    TRef<Pane>      ppaneChild;  CastTo(ppaneChild, (IObject*)stack.Pop());
    TRef<ModelData> pmodelData;  CastTo(pmodelData, (IObject*)stack.Pop());

    return (Pane*)new SelectModelPane(ppaneChild, pmodelData);
}


//////////////////////////////////////////////////////////////////////////////
//
// SelectionPane
//
//////////////////////////////////////////////////////////////////////////////

class SelectionPane : public ValuePane, public EventTargetContainer<SelectionPane>
{
private:
    TRef<RowPane> m_prowPane;

public:

    SelectionPane(IEventSource* peventSourceOnSelChange)
    {
        InsertAtBottom(m_prowPane = new RowPane());
		// mdvalley: needs pointer and class.
        AddEventTarget(&SelectionPane::OnSelChange, peventSourceOnSelChange);
    }

    bool OnSelChange()
    {
        return true;
    }

    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }

};

TRef<IObject> SelectionPaneFactory::Apply(ObjectStack& stack)
{
    TRef<IEventSource> peventSourceOnSelChange; CastTo(peventSourceOnSelChange, (IObject*)stack.Pop());
    return (Pane*) new SelectionPane(peventSourceOnSelChange);
}
