#ifndef _efpopup_H_
#define _efpopup_H_

/////////////////////////////////////////////////////////////////////////////
//
// Message Box
//
/////////////////////////////////////////////////////////////////////////////

class IMessageBox : public IPopup
{
public:
    virtual IntegerEventSourceImpl* GetEventSource() = 0;
};

TRef<IMessageBox> CreateMessageBox(
    ITimerEventSource* ptimerEventSource,
    Modeler*           pmodeler, 
    const ZString&     str, 
    ButtonPane*        pbuttonIn, 
    bool               fAddDefaultButton,
    ButtonPane*        pbuttonCancel,
    float              paintDelay
);

#endif
